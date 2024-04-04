/**
 * Timer/Counter E Embedded Driver API Source File
 *
 * @file tce0.c
 *
 * @defgroup tce0 TCE0
 *
 * @brief This document contains the implementation of public and private functions for the Timer Counter E module.
 *
 * @version TCE0 Driver Version 1.0.0
 *
 * @copyright (c) 2024 Microchip Technology Inc. and its subsidiaries.
 *
 * Subject to your compliance with these terms, you may use Microchip software
 * and any derivatives exclusively with Microchip products. You're responsible
 * for complying with 3rd party license terms applicable to your use of 3rd
 * party software (including open source software) that may accompany
 * Microchip software.
 *
 * SOFTWARE IS "AS IS." NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF
 * NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS
 * RELATED TO THE SOFTWARE WILL NOT EXCEED AMOUNT OF FEES, IF ANY, YOU PAID
 * DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 **/


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stddef.h>

#include "../tce0.h"



/**
 * @ingroup tce0
 * @brief Boolean that is true when timer is active.
 */
static volatile bool timerActive = false;

/**
 * @ingroup tce0
 * @brief 8-bit variable that replicates TCE_WGMODE_t enum to reflect the active mode.
 */
static volatile uint8_t timerMode = TCE_WGMODE_FRQ_gc;



void TCE0_Initialize(void)
{
    timerMode = TCE_WGMODE_SINGLESLOPE_gc;


    TCE0.CTRLA = 0x00;
    // CMP0 disabled; CMP1 disabled; CMP2 disabled; CMP3 disabled; OVF disabled; 
    TCE0.INTCTRL = 0x0;
    // ALUPD disabled; CMP0EN enabled; CMP1EN enabled; CMP2EN enabled; CMP3EN enabled; WGMODE SINGLESLOPE; 
    TCE0.CTRLB = 0xF3;
    // CMP0OV disabled; CMP0POL disabled; CMP1OV disabled; CMP1POL disabled; CMP2OV disabled; CMP2POL disabled; CMP3OV disabled; CMP3POL disabled; 
    TCE0.CTRLC = 0x0;
    // AMPEN enabled; HREN OFF; SCALE NORMAL; SCALEMODE BOTTOM; 
    TCE0.CTRLD = 0x18;
    // CMD NONE; DIR disabled; LUPD disabled; 
    TCE0.CTRLECLR = 0x0;
    // CMD NONE; DIR UP; LUPD disabled; 
    TCE0.CTRLESET = 0x0;
    // CMP0BV disabled; CMP1BV disabled; CMP2BV disabled; CMP3BV disabled; PERBV disabled; 
    TCE0.CTRLFCLR = 0x0;
    // CMP0BV disabled; CMP1BV disabled; CMP2BV disabled; CMP3BV disabled; PERBV disabled; 
    TCE0.CTRLFSET = 0x0;
    // CMP0EV PULSE; CMP1EV PULSE; CMP2EV PULSE; CMP3EV PULSE; 
    TCE0.EVGENCTRL = 0x0;
    // CNTAEI disabled; CNTBEI disabled; EVACTA CNT_POSEDGE; EVACTB NONE; 
    TCE0.EVCTRL = 0x0;
    // DBGRUN disabled; 
    TCE0.DBGCTRL = 0x0;
    // Temporary Register
    TCE0.TEMP = 0x0;
    // Counter Register
    TCE0.CNT = 0x0;
    // Amplitude Register
    TCE0.AMP = 0x8000;
    // Offset Register
    TCE0.OFFSET = 0x0;
    // Period Register
    TCE0.PER = 0x3E7;
    // Capture/Compare 0 Register
    TCE0.CMP0 = 0x0;
    // Capture/Compare 1 Register
    TCE0.CMP1 = 0x0;
    // Capture/Compare 2 Register
    TCE0.CMP2 = 0x0;
    // Capture/Compare 3 Register
    TCE0.CMP3 = 0x0;
    // Interrupt Flags
    TCE0.INTFLAGS = TCE0.INTFLAGS;
    // CLKSEL DIV1; ENABLE enabled; RUNSTDBY disabled; 
    TCE0.CTRLA = 0x1;

    timerActive = true;
}


void TCE0_Deinitialize(void)
{
    TCE0.CTRLA     = 0x00;
    TCE0.INTCTRL   = 0x00;
    TCE0.CTRLB     = 0x00;
    TCE0.CTRLC     = 0x00;
    TCE0.CTRLD     = 0x00;
    TCE0.CTRLECLR  = 0x00;
    TCE0.CTRLESET  = 0x00;
    TCE0.CTRLFCLR  = 0x00;
    TCE0.CTRLFSET  = 0x00;
    TCE0.EVGENCTRL = 0x00;
    TCE0.EVCTRL    = 0x00;
    TCE0.DBGCTRL   = 0x00;
    TCE0.TEMP      = 0x00;
    TCE0.CNT       = 0x0000;
    TCE0.AMP       = 0x0000;
    TCE0.OFFSET    = 0x0000;
    TCE0.PER       = 0xFFFF;
    TCE0.CMP0      = 0x0000;
    TCE0.CMP1      = 0x0000;
    TCE0.CMP2      = 0x0000;
    TCE0.CMP3      = 0x0000;


    timerActive = false;

    TCE0.INTFLAGS = TCE0.INTFLAGS;
}

void TCE0_Start(void)
{
    TCE0.CTRLA |= TCE_ENABLE_bm;
    timerActive = true;
}

void TCE0_Stop(void)
{
    TCE0.CTRLA &= ~TCE_ENABLE_bm;
    timerActive = false;
}

TCE0_status_t TCE0_StatusGet(void)
{
    TCE0_status_t retval;

    if ((TCE0.CTRLA & TCE_ENABLE_bm) != 0)
    {
        retval = TCE_STATUS_RUNNING;
    }
    else
    {
        retval = TCE_STATUS_IDLE;
    }

    return retval;
}

void TCE0_ModeSet(TCE_WGMODE_t mode)
{
    uint8_t temp;

    if ( (mode == TCE_WGMODE_NORMAL_gc)      ||
         (mode == TCE_WGMODE_FRQ_gc)         ||
         (mode == TCE_WGMODE_SINGLESLOPE_gc) ||
         (mode == TCE_WGMODE_DSTOP_gc)       ||
         (mode == TCE_WGMODE_DSBOTH_gc)      ||
         (mode == TCE_WGMODE_DSBOTTOM_gc) )
    {
        if (timerActive)
        {
            TCE0.CTRLA &= ~TCE_ENABLE_bm;

            temp = (TCE0.CTRLB & ~TCE_WGMODE_gm) |
                   (  mode     &  TCE_WGMODE_gm);
            TCE0.CTRLB = temp;
            timerMode = mode;

            TCE0.CTRLA |= TCE_ENABLE_bm;
        }
        else
        {
            temp = (TCE0.CTRLB & ~TCE_WGMODE_gm) |
                   (  mode     &  TCE_WGMODE_gm);
            TCE0.CTRLB = temp;
            timerMode = mode;
        }
    }
}

void TCE0_Interrupts_Enable(uint8_t interrupts)
{
    TCE0.INTCTRL |= interrupts & (TCE_OVF_bm | TCE_CMP0_bm | TCE_CMP1_bm | TCE_CMP2_bm | TCE_CMP3_bm);
}

void TCE0_Interrupts_FlagsClear(uint8_t interrupts)
{
    TCE0.INTFLAGS = interrupts & (TCE_OVF_bm | TCE_CMP0_bm | TCE_CMP1_bm | TCE_CMP2_bm | TCE_CMP3_bm);
}

uint8_t TCE0_Interrupts_FlagsGet(void)
{
    return TCE0.INTFLAGS;
}

void TCE0_Interrupts_Disable(uint8_t interrupts)
{
    TCE0.INTCTRL &= ~(interrupts & (TCE_OVF_bm | TCE_CMP0_bm | TCE_CMP1_bm | TCE_CMP2_bm | TCE_CMP3_bm));
}

void TCE0_Event_OutputMode(uint8_t mode)
{
    uint8_t temp;

    if (timerActive)
    {
        TCE0.CTRLA &= ~TCE_ENABLE_bm;

        temp = (TCE0.CTRLB & ~(TCE_CMP0EV_bm | TCE_CMP1EV_bm | TCE_CMP2EV_bm | TCE_CMP3EV_bm)) |
               ( mode      &  (TCE_CMP0EV_bm | TCE_CMP1EV_bm | TCE_CMP2EV_bm | TCE_CMP3EV_bm));
        TCE0.CTRLB = temp;

        TCE0.CTRLA |= TCE_ENABLE_bm;
    }
    else
    {
        temp = (TCE0.CTRLB & ~(TCE_CMP0EV_bm | TCE_CMP1EV_bm | TCE_CMP2EV_bm | TCE_CMP3EV_bm)) |
               ( mode      &  (TCE_CMP0EV_bm | TCE_CMP1EV_bm | TCE_CMP2EV_bm | TCE_CMP3EV_bm));
        TCE0.CTRLB = temp;
    }
}

void TCE0_Event_InputConfig(uint8_t value)
{
    if (timerActive)
    {
        TCE0.CTRLA &= ~TCE_ENABLE_bm;
        TCE0.EVCTRL = value;
        TCE0.CTRLA |= TCE_ENABLE_bm;
    }
    else
    {
        TCE0.EVCTRL = value;
    }
}

void TCE0_SoftwareCommand(TCE_CMD_t command)
{
    uint8_t temp;
    temp = (TCE0.CTRLESET & ~TCE_CMD_gm) |
           ( command      &  TCE_CMD_gm);
    TCE0.CTRLESET = temp;
}

void TCE0_StandBySleep(bool state)
{
    if (state == true)
    {
        TCE0.CTRLA |= TCE_RUNSTDBY_bm;
    }
    else
    {
        TCE0.CTRLA &= ~TCE_RUNSTDBY_bm;
    }
}

void TCE0_DebugRun(bool state)
{
    if (state == true)
    {
        TCE0.DBGCTRL |= TCE_DBGRUN_bm;
    }
    else
    {
        TCE0.DBGCTRL &= ~TCE_DBGRUN_bm;
    }
}

uint16_t TCE0_CounterGet(void)
{
    return TCE0.CNT;
}

void TCE0_CounterSet(uint16_t value)
{
    TCE0.CNT = value;
}

void TCE0_PrescalerSet(TCE_CLKSEL_t prescaler)
{
    uint8_t temp;
    temp = (TCE0.CTRLA & ~TCE_CLKSEL_gm) |
           ( prescaler &  TCE_CLKSEL_gm);
    TCE0.CTRLA = temp;
}

void TCE0_Compare0Set(uint16_t value)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        TCE0.CMP0 = value;
    }
}

void TCE0_Compare1Set(uint16_t value)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        TCE0.CMP1 = value;
    }
}

void TCE0_Compare2Set(uint16_t value)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        TCE0.CMP2 = value;
    }
}

void TCE0_Compare3Set(uint16_t value)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        TCE0.CMP3 = value;
    }
}

void TCE0_CompareAllChannelsSet(uint16_t value0, uint16_t value1, uint16_t value2, uint16_t value3)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        TCE0.CMP0 = value0;
        TCE0.CMP1 = value1;
        TCE0.CMP2 = value2;
        TCE0.CMP3 = value3;
    }
}

void TCE0_CompareChannels012Set(uint16_t value0, uint16_t value1, uint16_t value2)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        TCE0.CMP0 = value0;
        TCE0.CMP1 = value1;
        TCE0.CMP2 = value2;
    }
}

void TCE0_CompareChannels123Set(uint16_t value1, uint16_t value2, uint16_t value3)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        TCE0.CMP1 = value1;
        TCE0.CMP2 = value2;
        TCE0.CMP3 = value3;
    }
}

void TCE0_CompareAllChannelsBufferedSet(uint16_t value0, uint16_t value1, uint16_t value2, uint16_t value3)
{
    TCE0.CMP0BUF = value0;
    TCE0.CMP1BUF = value1;
    TCE0.CMP2BUF = value2;
    TCE0.CMP3BUF = value3;
}

void TCE0_CompareChannels012BufferedSet(uint16_t value0, uint16_t value1, uint16_t value2)
{
    TCE0.CMP0BUF = value0;
    TCE0.CMP1BUF = value1;
    TCE0.CMP2BUF = value2;
}

void TCE0_CompareChannels123BufferedSet(uint16_t value1, uint16_t value2, uint16_t value3)
{
    TCE0.CMP1BUF = value1;
    TCE0.CMP2BUF = value2;
    TCE0.CMP3BUF = value3;
}

void TCE0_OutputsEnable(uint8_t value)
{
    uint8_t temp;
    temp = (TCE0.CTRLB & ~(TCE_CMP0EN_bm| TCE_CMP1EN_bm | TCE_CMP2EN_bm | TCE_CMP3EN_bm)) |
           ( value     &  (TCE_CMP0EN_bm| TCE_CMP1EN_bm | TCE_CMP2EN_bm | TCE_CMP3EN_bm));
    TCE0.CTRLB = temp;
}

void TCE0_OutputsValueSet(uint8_t value)
{
    TCE0.CTRLC = value;
}

uint8_t TCE0_OutputsValueGet(void)
{
    return TCE0.CTRLC;
}

void TCE0_HighResSet(TCE_HREN_t resolution)
{
    uint8_t temp;
    temp = (TCE0.CTRLD  & ~TCE_HREN_gm) |
           ( resolution &  TCE_HREN_gm);
    TCE0.CTRLD = temp;
}


void TCE0_ScaleModeSet(TCE_SCALEMODE_t mode)
{
    uint8_t temp;
    temp = (TCE0.CTRLD & ~TCE_SCALEMODE_gm) |
           ( mode      &  TCE_SCALEMODE_gm);
    TCE0.CTRLD = temp;
}

void TCE0_ScaleEnable(bool state)
{
    if (state == true)
    {
        TCE0.CTRLD |= TCE_SCALE_bm;
    }
    else
    {
        TCE0.CTRLD &= ~TCE_SCALE_bm;
    }
}

void TCE0_AmplitudeControlEnable(bool state)
{
    if (state == true)
    {
        TCE0.CTRLD |= TCE_AMPEN_bm;
    }
    else
    {
        TCE0.CTRLD &= ~TCE_AMPEN_bm;
    }
}

void TCE0_AmplitudeSet(uint16_t value)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
      TCE0.AMP = value;
    }
}

uint16_t TCE0_AmplitudeGet(void)
{
    return TCE0.AMP;
}

void TCE0_OffsetSet(uint16_t value)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        TCE0.OFFSET = value;
    }
}

uint16_t TCE0_OffsetGet(void)
{
    return TCE0.OFFSET;
}

void TCE0_PeriodSet(uint16_t period)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        TCE0.PER = period;
    }
}

void TCE0_CountDirectionSet(void)
{
    TCE0.CTRLESET = TCE_DIR_bm;
}

void TCE0_CountDirectionClear(void)
{
    TCE0.CTRLECLR = TCE_DIR_bm;
}

void TCE0_LockUpdateSet(void)
{
    TCE0.CTRLESET = TCE_LUPD_bm;
}

void TCE0_LockUpdateClear(void)
{
    TCE0.CTRLECLR = TCE_LUPD_bm;
}

void TCE0_AutoLockUpdateSet(void)
{
    TCE0.CTRLB |= TCE_ALUPD_bm;
}

void TCE0_AutoLockUpdateClear(void)
{
    TCE0.CTRLB &= ~TCE_ALUPD_bm;
}

void TCE0_PWM_BufferedDutyCycle0Set(uint16_t value)
{
    TCE0.CMP0BUF = value;
}

void TCE0_PWM_BufferedDutyCycle1Set(uint16_t value)
{
    TCE0.CMP1BUF = value;
}

void TCE0_PWM_BufferedDutyCycle2Set(uint16_t value)
{
    TCE0.CMP2BUF = value;
}

void TCE0_PWM_BufferedDutyCycle3Set(uint16_t value)
{
    TCE0.CMP3BUF = value;
}

void TCE0_PeriodBufferSet(uint16_t value)
{
    TCE0.PERBUF = value;
}
