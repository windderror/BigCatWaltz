/**
 * \file IfxIom.c
 * \brief IOM  basic functionality
 *
 * \version iLLD_1_0_1_11_0
 * \copyright Copyright (c) 2019 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Use of this file is subject to the terms of use agreed between (i) you or 
 * the company in which ordinary course of business you are acting and (ii) 
 * Infineon Technologies AG or its licensees. If and as long as no such 
 * terms of use are agreed, use of this file is subject to following:


 * Boost Software License - Version 1.0 - August 17th, 2003

 * Permission is hereby granted, free of charge, to any person or 
 * organization obtaining a copy of the software and accompanying 
 * documentation covered by this license (the "Software") to use, reproduce,
 * display, distribute, execute, and transmit the Software, and to prepare
 * derivative works of the Software, and to permit third-parties to whom the 
 * Software is furnished to do so, all subject to the following:

 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer, must
 * be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are
 * solely in the form of machine-executable object code generated by a source
 * language processor.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.

 *
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxIom.h"
#include "Scu/Std/IfxScuCcu.h"

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

float32 IfxIom_getFrequency(Ifx_IOM *iom)
{
    IFX_UNUSED_PARAMETER(iom)
    float32 fspb;
    float32 fgtm;

    fspb = IfxScuCcu_getSpbFrequency();
    fgtm = IfxScuCcu_getGtmFrequency();

    return fspb > fgtm ? fspb : fgtm;
}


void IfxIom_resetModule(Ifx_IOM *iom)
{
    uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();

    IfxScuWdt_clearCpuEndinit(passwd);
    iom->KRST1.B.RST = 1;      /* Only if both Kernel reset bits are set a reset is executed */
    iom->KRST0.B.RST = 1;
    IfxScuWdt_setCpuEndinit(passwd);

    while (iom->KRST0.B.RSTSTAT == 0)  /* Wait until reset is executed */

    {}

    IfxScuWdt_clearCpuEndinit(passwd);
    iom->KRSTCLR.B.CLR = 1;    /* Clear Kernel reset status bit */
    IfxScuWdt_setCpuEndinit(passwd);
}
