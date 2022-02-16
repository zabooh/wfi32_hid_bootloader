/*
 * Software License Agreement
 *
 * Copyright © 2011 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED ?AS IS? WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// *****************************************************************************
// *****************************************************************************
// Function: Core Timer Read
// *****************************************************************************
// *****************************************************************************
uint32_t __attribute__((nomips16)) APP_CoreTimer_Read()
{
    volatile uint32_t timer;

    // get the count reg
    asm volatile("mfc0   %0, $9" : "=r"(timer));

    return(timer);
}

// *****************************************************************************
// *****************************************************************************
// Function: Core Timer Sart
// *****************************************************************************
// *****************************************************************************
void __attribute__((nomips16)) APP_CoreTimer_Start(uint32_t period)
{
    /* Reset the coutner */
    volatile uint32_t loadZero = 0;

    asm volatile("mtc0   %0, $9" : "+r"(loadZero));
    asm volatile("mtc0   %0, $11" : "+r" (period));
}

// *****************************************************************************
// *****************************************************************************
// Function: Core Timer Delay (Blocking))
// *****************************************************************************
// *****************************************************************************
void __attribute__((nomips16)) APP_CoreTimer_Delay(uint32_t delayValue)
{
    while ((APP_CoreTimer_Read() <= delayValue))
    {
        asm("nop");
    }
}

// *****************************************************************************
// *****************************************************************************
// Function: Core Timer Software Timeout Check (Non Blocking))
// *****************************************************************************
// *****************************************************************************
bool __attribute__((nomips16)) APP_CoreTimer_IsTimeout(uint32_t *ptime, int32_t timeout)
{
    int32_t diff;
    int32_t timer_value;

    timer_value = APP_CoreTimer_Read();
    diff = timer_value - *ptime;
    if (diff < 0) diff += 0xFFFFFFFF;

    if (diff > timeout)
    {
        *ptime = timer_value;
        return true;
    }
    else
    {
        return false;
    }
}

