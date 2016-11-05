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

#include <openthread-types.h>
#include <platform/misc.h>
#include "platform-atmegarf.h"

#include <mega_reset_cause.h>
#include <wdt_megarf.h>

void otPlatReset(otInstance *aInstance)
{
    (void)aInstance;
	wdt_reset_mcu();
}

otPlatResetReason otPlatGetResetReason(otInstance *aInstance)
{
    (void)aInstance;

	otPlatResetReason reason = kPlatResetReason_PowerOn;
	reset_cause_t cause = reset_cause_get_causes();
	reset_cause_clear_causes(cause);

	if(cause & CHIP_RESET_CAUSE_EXTRST)  reason |= kPlatResetReason_External;
	if(cause & CHIP_RESET_CAUSE_BOD_CPU) reason |= kPlatResetReason_Unknown;
	if(cause & CHIP_RESET_CAUSE_POR)     reason |= kPlatResetReason_Unknown;
	if(cause & CHIP_RESET_CAUSE_WDT)     reason |= kPlatResetReason_Watchdog;
#if !MEGA_XX8 && !MEGA_XX8_A
	if(cause & CHIP_RESET_CAUSE_JTAG)    reason |= kPlatResetReason_Other;
#endif

    return(cause);
}
