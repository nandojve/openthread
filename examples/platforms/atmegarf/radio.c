/*
 *  Copyright (c) 2016, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file implements the OpenThread platform abstraction for radio communication.
 *
 */

#include <openthread-types.h>
#include <openthread.h>
#include <openthread-config.h>

#include <common/code_utils.hpp>
#include <platform/platform.h>
#include <platform/radio.h>
#include <platform/diag.h>
#include "platform-atmegarf.h"

#include "delay.h"

#ifndef ANTENNA_DIVERSITY
	#define ANTENNA_DIVERSITY 0
#endif
#ifndef ANTENNA_DEFAULT
	#define ANTENNA_DEFAULT	  0
#endif
#ifndef EXT_RF_FRONT_END_CTRL
	#define EXT_RF_FRONT_END_CTRL 0
#endif

#define IRQ_CLEAR_VALUE 0xFF
//#define TRX_RPC_REG_VALUE 0xeb
#define TRX_RPC_REG_VALUE 0x00
#define PHY_RSSI_BASE_VAL (-90)

enum
{
    IEEE802154_MIN_LENGTH = 5,
    IEEE802154_MAX_LENGTH = 127,
    IEEE802154_ACK_LENGTH = 5,
	IEEE802154_FCS_LENGTH = 2,
    IEEE802154_FRAME_TYPE_MASK = 0x7,
    IEEE802154_FRAME_TYPE_ACK = 0x2,
    IEEE802154_FRAME_PENDING = 1 << 4,
    IEEE802154_ACK_REQUEST = 1 << 5,
    IEEE802154_DSN_OFFSET = 2,
};

enum
{
	CC_BAND_IEEE = 0x00,
	CC_BAND_2322_2433 = 0x08,
	CC_BAND_2434_2527 = 0x09,
};

static RadioPacket sTransmitFrame;
static RadioPacket sReceiveFrame;
static ThreadError sTransmitError;
static ThreadError sReceiveError;

static uint8_t sTransmitPsdu[IEEE802154_MAX_LENGTH];
static uint8_t sReceivePsdu[IEEE802154_MAX_LENGTH + 1 /* LQI data*/];

static PhyState sState = kStateDisabled;
static int8_t last_valid_rssi;
static bool sIsReceiverEnabled = false;
static uint64_t eui64;

static void phy_trxSetState(uint8_t state)
{
	do
	{
		TRX_STATE_REG = TRX_CMD_FORCE_TRX_OFF;
	} while (TRX_STATUS_TRX_OFF != TRX_STATUS_REG_s.trxStatus);

	do
	{
		TRX_STATE_REG = state;
	} while (state != TRX_STATUS_REG_s.trxStatus);
}
static void phy_sleep(void)
{
#if (ANTENNA_DIVERSITY == 1)
	ANT_DIV_REG_s.antDivEn = 0;
	ANT_DIV_REG_s.antExtSwEn = 0;
	#if (ANTENNA_DEFAULT != 0)
		ANT_DIV_REG_s.antCtrl = 0;
	#endif // ANTENNA_DEFAULT
#endif // ANTENNA_DIVERSITY
#ifdef EXT_RF_FRONT_END_CTRL
	TRX_CTRL_1_REG_s.paExtEn = 0;
#endif // EXT_RF_FRONT_END_CTRL

	phy_trxSetState(TRX_CMD_TRX_OFF);
	TRXPR_REG_s.slptr = 1;
}
static void phy_wakeup(void)
{
	TRXPR_REG_s.slptr = 0;

#if (ANTENNA_DIVERSITY == 1)
	ANT_DIV_REG_s.antDivEn = 1;
	ANT_DIV_REG_s.antExtSwEn = 1;
	#if (ANTENNA_DEFAULT != 0)
		ANT_DIV_REG_s.antCtrl = ANTENNA_DEFAULT;
	#endif // ANTENNA_DEFAULT
#endif // ANTENNA_DIVERSITY
#ifdef EXT_RF_FRONT_END_CTRL
	TRX_CTRL_1_REG_s.paExtEn = 1;
#endif // EXT_RF_FRONT_END_CTRL
}

ThreadError atmegarfGenerateRandom(uint16_t aInputLength, uint8_t *aOutput, uint16_t *aOutputLength)
{
	(void) aOutputLength;
	if(sState == kStateReceive && IRQ_STATUS_REG_s.rxStart) return(kThreadError_Busy);
	if(sState == kStateTransmit) return(kThreadError_Busy);

	// Do the job only for sleep or disable states

	if(sState == kStateDisabled) TRXPR_REG_s.slptr = 0;

	phy_trxSetState(TRX_CMD_FORCE_TRX_OFF);
	phy_trxSetState(TRX_CMD_RX_ON);
	TRX_RPC_REG = 0x00;

	for (uint16_t index = 0; index < aInputLength; index++)
	{
		aOutput[index] = 0;

		for (uint8_t offset = 0; offset < 8 /* 1 byte = 8 bits */; offset += 2)
		{
			delay_us(RANDOM_NUMBER_UPDATE_INTERVAL);
			aOutput[index] |= PHY_RSSI_REG_s.rndValue << offset;
		}
	}

	TRX_RPC_REG = TRX_RPC_REG_VALUE;
	phy_trxSetState(TRX_CMD_TRX_OFF);
	IRQ_STATUS_REG = IRQ_CLEAR_VALUE;
	if(sState == kStateDisabled) TRXPR_REG_s.slptr = 1;

	return(kThreadError_None);
}
static void enableReceiver(void)
{
    if (!sIsReceiverEnabled)
    {
		phy_trxSetState(TRX_CMD_TRX_OFF);
		IRQ_STATUS_REG = IRQ_CLEAR_VALUE;
		phy_trxSetState(TRX_CMD_RX_AACK_ON);
        sIsReceiverEnabled = true;
    }
}

static void disableReceiver(void)
{
    if (sIsReceiverEnabled)
    {
		phy_trxSetState(TRX_CMD_TRX_OFF);
		IRQ_STATUS_REG = IRQ_CLEAR_VALUE;

        sIsReceiverEnabled = false;
    }
}

static void setChannel(uint8_t channel)
{
	CC_CTRL_1_REG_s.ccBand = CC_BAND_IEEE;

	if (CC_BAND_IEEE)
	{
		CC_CTRL_0_REG = channel;
	}
	else
	{
		PHY_CC_CCA_REG_s.channel = channel;
	}
}

void otPlatRadioGetIeeeEui64(otInstance *aInstance, uint8_t *aIeeeEui64)
{
    (void)aInstance;

	uint8_t* eui64_ptr = (uint8_t *) &eui64;

    for (uint8_t i = 0; i < sizeof(eui64); i++)
    {
        aIeeeEui64[i] = eui64_ptr[7 - i];
    }
}

ThreadError otPlatRadioSetPanId(otInstance *aInstance, uint16_t panid)
{
    ThreadError error = kThreadError_Busy;
    (void)aInstance;

    if (sState != kStateTransmit)
    {
		uint8_t *d = (uint8_t *)&panid;

		PAN_ID_0_REG = d[0];
		PAN_ID_1_REG = d[1];

        error = kThreadError_None;
    }

    return error;
}

ThreadError otPlatRadioSetExtendedAddress(otInstance *aInstance, uint8_t *address)
{
    ThreadError error = kThreadError_Busy;
    (void)aInstance;

    if (sState != kStateTransmit)
    {
		uint8_t *ptr_to_reg = address;
		IEEE_ADDR_0_REG = *ptr_to_reg++;
		IEEE_ADDR_1_REG = *ptr_to_reg++;
		IEEE_ADDR_2_REG = *ptr_to_reg++;
		IEEE_ADDR_3_REG = *ptr_to_reg++;
		IEEE_ADDR_4_REG = *ptr_to_reg++;
		IEEE_ADDR_5_REG = *ptr_to_reg++;
		IEEE_ADDR_6_REG = *ptr_to_reg++;
		IEEE_ADDR_7_REG = *ptr_to_reg;

        error = kThreadError_None;
    }

    return error;
}

ThreadError otPlatRadioSetShortAddress(otInstance *aInstance, uint16_t address)
{
    ThreadError error = kThreadError_Busy;
    (void)aInstance;

    if (sState != kStateTransmit)
    {
		uint8_t *d = (uint8_t *)&address;

		SHORT_ADDR_0_REG = d[0];
		SHORT_ADDR_1_REG = d[1];

        error = kThreadError_None;
    }

    return error;
}

void atmegarfRadioInit(void)
{
    sTransmitFrame.mLength = 0;
    sTransmitFrame.mPsdu = sTransmitPsdu;
    sReceiveFrame.mLength = 0;
    sReceiveFrame.mPsdu = sReceivePsdu;

	sysclk_enable_peripheral_clock(&TRX_CTRL_0);

	TRXPR_REG_s.trxrst = 1;

	phy_trxSetState(TRX_CMD_TRX_OFF);

	TRX_RPC_REG = TRX_RPC_REG_VALUE;

	TRX_CTRL_2_REG_s.rxSafeMode = 1;

	//#ifdef PHY_ENABLE_RANDOM_NUMBER_GENERATOR
	//CSMA_SEED_0_REG = (uint8_t)PHY_RandomReq();
	//#endif

#if (ANTENNA_DIVERSITY == 1)
	ANT_DIV_REG_s.antCtrl = 2;
	RX_CTRL_REG_s.pdtThres = 0x03;
	ANT_DIV_REG_s.antDivEn = 1;
	ANT_DIV_REG_s.antExtSwEn = 1;
#else
	#if (ANTENNA_DEFAULT != 0)
		ANT_DIV_REG_s.antExtSwEn = 1;
		ANT_DIV_REG_s.antCtrl = ANTENNA_DEFAULT;
	#endif // ANTENNA_DEFAULT
#endif // ANTENNA_DIVERSITY
#ifdef EXT_RF_FRONT_END_CTRL
	TRX_CTRL_1_REG_s.paExtEn = 1;
#endif // EXT_RF_FRONT_END_CTRL

	atmegarfGenerateRandom(sizeof(eui64), &eui64, NULL);
}

ThreadError otPlatRadioEnable(otInstance *aInstance)
{
    ThreadError error = kThreadError_Busy;
    (void)aInstance;

    if (sState == kStateSleep)
	{
		error = kThreadError_None;
	}
	else if(sState == kStateDisabled)
    {
        error = kThreadError_None;
        sState = kStateSleep;
		phy_wakeup();
    }

    return error;
}

ThreadError otPlatRadioDisable(otInstance *aInstance)
{
    ThreadError error = kThreadError_Busy;
    (void)aInstance;

    if (sState == kStateDisabled)
	{
		error = kThreadError_None;
	}
	else if(sState == kStateSleep)
    {
        error = kThreadError_None;
        sState = kStateDisabled;
		phy_sleep();
    }

    return error;
}

bool otPlatRadioIsEnabled(otInstance *aInstance)
{
    (void)aInstance;
    return (sState != kStateDisabled) ? true : false;
}

ThreadError otPlatRadioSleep(otInstance *aInstance)
{
    ThreadError error = kThreadError_Busy;
    (void)aInstance;

    if (sState == kStateSleep || sState == kStateReceive)
    {
        error = kThreadError_None;
        sState = kStateSleep;
        disableReceiver();
    }

    return error;
}

ThreadError otPlatRadioReceive(otInstance *aInstance, uint8_t aChannel)
{
    ThreadError error = kThreadError_Busy;
    (void)aInstance;

    if (sState != kStateDisabled)
    {
        error = kThreadError_None;
        sState = kStateReceive;
        setChannel(aChannel);
        sReceiveFrame.mChannel = aChannel;
        enableReceiver();
    }

    return error;
}

ThreadError otPlatRadioTransmit(otInstance *aInstance)
{
    ThreadError error = kThreadError_Busy;
    (void)aInstance;

    if (sState == kStateReceive)
    {
        error = kThreadError_None;
        sState = kStateTransmit;
        sTransmitError = kThreadError_None;

		phy_trxSetState(TRX_CMD_TX_ARET_ON);

		IRQ_STATUS_REG = IRQ_CLEAR_VALUE;

		TRX_FRAME_BUFFER(0) = sTransmitFrame.mLength + IEEE802154_FCS_LENGTH;
		for (int16_t i = 0; i < sTransmitFrame.mLength; ++i)
		{
			TRX_FRAME_BUFFER(i + 1) = sTransmitFrame.mPsdu[i];
		}

		setChannel(sTransmitFrame.mChannel);

		TRX_STATE_REG = TRX_CMD_TX_START;
    }

	return(error);
}

RadioPacket *otPlatRadioGetTransmitBuffer(otInstance *aInstance)
{
    (void)aInstance;
    return &sTransmitFrame;
}

int8_t otPlatRadioGetRssi(otInstance *aInstance)
{
    (void)aInstance;
    return last_valid_rssi;
}

otRadioCaps otPlatRadioGetCaps(otInstance *aInstance)
{
    (void)aInstance;
    return kRadioCapsNone;
}

bool otPlatRadioGetPromiscuous(otInstance *aInstance)
{
    (void)aInstance;
    return(XAH_CTRL_1_REG_s.aackPromMode == 1);
}

void otPlatRadioSetPromiscuous(otInstance *aInstance, bool aEnable)
{
    (void)aInstance;

	if(aEnable)
	{
		uint8_t ieee_address[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

		otPlatRadioSetShortAddress(aInstance, 0x0000);
		otPlatRadioSetPanId(aInstance, 0x0000);
		otPlatRadioSetExtendedAddress(aInstance, ieee_address);

		// AACK_UPLD_RES_FT = 1, AACK_FLT_RES_FT = 0:
		//	Any non-corrupted frame with a reserved frame type is indicated by a
		//	TRX24_RX_END interrupt. No further address filtering is applied on those frames.
		//	A TRX24_AMI interrupt is never generated and the acknowledgment subfield is
		//	ignored.

		XAH_CTRL_1_REG_s.aackPromMode = 1;	// Enable promiscuous mode
		XAH_CTRL_1_REG_s.aackUpldResFt = 1;	// Enable reserved frame type reception
		XAH_CTRL_1_REG_s.aackFltrResFt = 0;	// Disable filter of reserved frame types
		CSMA_SEED_1_REG_s.aackDisAck = 1;	// Disable generation of acknowledgment
	}
	else
	{
		XAH_CTRL_1_REG = 0;
		CSMA_SEED_1_REG_s.aackDisAck = 0;
	}
}

void atmegarfRadioProcess(otInstance *aInstance)
{
	if (sState == kStateDisabled || sState == kStateSleep) return;

	if (IRQ_STATUS_REG_s.rxEnd)
	{
		uint8_t size = TST_RX_LENGTH_REG;

		for (uint8_t i = 0; i < size + 1 /*lqi*/; i++) {
			sReceiveFrame.mPsdu[i] = TRX_FRAME_BUFFER(i);
		}

		sReceiveFrame.mLength = size - IEEE802154_FCS_LENGTH;
		sReceiveFrame.mLqi = sReceiveFrame.mPsdu[size];
		last_valid_rssi = (int8_t)PHY_ED_LEVEL_REG + PHY_RSSI_BASE_VAL;

#if OPENTHREAD_ENABLE_DIAG
		if (otPlatDiagModeGet())
		{
			otPlatDiagRadioReceiveDone(aInstance, &sReceiveFrame, sReceiveError);
		}
		else
#endif
		{
			otPlatRadioReceiveDone(aInstance, &sReceiveFrame, sReceiveError);
		}

		while (TRX_STATUS_RX_AACK_ON != TRX_STATUS_REG_s.trxStatus);

		IRQ_STATUS_REG_s.rxEnd = 1;
		TRX_CTRL_2_REG_s.rxSafeMode = 0;
		TRX_CTRL_2_REG_s.rxSafeMode = 1;

		sReceiveFrame.mLength = 0;
	}
	else if (IRQ_STATUS_REG_s.txEnd)
	{
		if (TRX_STATUS_TX_ARET_ON == TRX_STATUS_REG_s.trxStatus)
		{
			uint8_t status = TRX_STATE_REG_s.tracStatus;

			if (TRAC_STATUS_SUCCESS == status)
			{
				sTransmitError = kThreadError_None;
			}
			else if (TRAC_STATUS_CHANNEL_ACCESS_FAILURE == status)
			{
				sTransmitError = kThreadError_ChannelAccessFailure;
			}
			else if (TRAC_STATUS_NO_ACK == status)
			{
				sTransmitError = kThreadError_NoAck;
			}
			else
			{
				sTransmitError = kThreadError_Abort;
			}

			sState = kStateReceive;
			bool frame_pending = false;

			if(sTransmitFrame.mPsdu[0] & IEEE802154_ACK_REQUEST)
			{
				if (sReceiveFrame.mLength == IEEE802154_ACK_LENGTH &&
				   (sReceiveFrame.mPsdu[0] & IEEE802154_FRAME_TYPE_MASK) == IEEE802154_FRAME_TYPE_ACK &&
				   (sReceiveFrame.mPsdu[IEEE802154_DSN_OFFSET] == sTransmitFrame.mPsdu[IEEE802154_DSN_OFFSET]))
				{
					frame_pending = (sReceiveFrame.mPsdu[0] & IEEE802154_FRAME_PENDING) != 0;
				}
			}

#if OPENTHREAD_ENABLE_DIAG
			if (otPlatDiagModeGet())
			{
				otPlatDiagRadioTransmitDone(aInstance, frame_pending, sTransmitError);
			}
			else
#endif
			{
				otPlatRadioTransmitDone(aInstance, frame_pending, sTransmitError);
			}
		}

		IRQ_STATUS_REG_s.txEnd = 1;
	}
}