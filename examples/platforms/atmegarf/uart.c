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
 *   This file implements the OpenThread platform abstraction for UART communication.
 *
 */

#include <stddef.h>

#include <openthread-types.h>
#include <common/code_utils.hpp>
#include <platform/uart.h>
#include "platform-atmegarf.h"

#include <stdio_serial.h>
#include <serial.h>
#include "conf_sio2host.h"

static const uint8_t *sTransmitBuffer = NULL;
static uint16_t sTransmitLength = 0;

static uint8_t sReceiveBuffer[SERIAL_RX_BUF_SIZE_HOST];
static uint16_t sReceiveHead = 0;
static uint16_t sReceiveLength = 0;

ThreadError otPlatUartEnable(void)
{
   	const usart_serial_options_t usart_serial_options =
   	{
	   	.baudrate				= USART_HOST_BAUDRATE,
	   	.charlength				= USART_HOST_CHAR_LENGTH,
	   	.paritytype				= USART_HOST_PARITY,
	   	.stopbits				= USART_HOST_STOP_BITS
   	};

   	stdio_serial_init(USART_HOST, &usart_serial_options);

#if (PLATFORM == mega)
   	usart_double_baud_enable(USART_HOST);
   	usart_set_baudrate_precalculated(USART_HOST, USART_HOST_BAUDRATE, sysclk_get_source_clock_hz());
#endif

    return kThreadError_None;
}

ThreadError otPlatUartDisable(void)
{
    return kThreadError_None;
}

ThreadError otPlatUartSend(const uint8_t *aBuf, uint16_t aBufLength)
{
    ThreadError error = kThreadError_None;

    VerifyOrExit(sTransmitBuffer == NULL, error = kThreadError_Busy);

    sTransmitBuffer = aBuf;
    sTransmitLength = aBufLength;
	USART_HOST_TX_ISR_ENABLE();

exit:
    return error;
}

void atmegarfUartProcess(void)
{
    uint16_t remaining;

    VerifyOrExit(sReceiveLength > 0, ;);

    remaining = SERIAL_RX_BUF_SIZE_HOST - sReceiveHead;

    if (sReceiveLength >= remaining)
    {
	    otPlatUartReceived(sReceiveBuffer + sReceiveHead, remaining);
	    sReceiveHead = 0;
	    sReceiveLength -= remaining;
    }

    if (sReceiveLength > 0)
    {
	    otPlatUartReceived(sReceiveBuffer + sReceiveHead, sReceiveLength);
	    sReceiveHead += sReceiveLength;
	    sReceiveLength = 0;
    }

exit:
    return;
}

USART_HOST_ISR_VECT()
{
    uint16_t tail;
    uint8_t byte = usart_get(USART_HOST);

    if (sReceiveLength < SERIAL_RX_BUF_SIZE_HOST)
    {
        tail = (sReceiveHead + sReceiveLength) % SERIAL_RX_BUF_SIZE_HOST;
        sReceiveBuffer[tail] = byte;
        sReceiveLength++;
    }
}

USART_HOST_ISR_TX_VECT()
{
	if(sTransmitLength > 0)
    {
		sTransmitLength--;
	    usart_put(USART_HOST, *sTransmitBuffer++);
    }
	else
	{
		sTransmitBuffer = NULL;
		otPlatUartSendDone();
		USART_HOST_TX_ISR_DISABLE();
	}
}