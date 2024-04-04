/**
 * System Driver Source File
 * 
 * @file system.c
 * 
 * @ingroup systemdriver
 * 
 * @brief This file contains the API implementation for the System driver.
 *
 * @version Driver Version 1.0.2
 *
 * @version Package Version 4.2.13
*/
/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#include "../system.h"

int8_t BOD_Initialize();

void SYSTEM_Initialize(void)
{
    PIN_MANAGER_Initialize();
    ADC0_Initialize();
    BOD_Initialize();
    CLOCK_Initialize();
    TCE0_Initialize();
    USART0_Initialize();
    WEX0_Initialize();
    CPUINT_Initialize();
}

int8_t BOD_Initialize()
{
    //SLEEP Disabled; 
    ccp_write_io((void*)&(BOD.CTRLA),0x4);
    //
    BOD.CTRLB = 0x2;
    //VLMCFG VDD falls below VLM threshold; VLMIE disabled; 
    BOD.INTCTRL = 0x0;
    //VLMIF disabled; 
    BOD.INTFLAGS = 0x0;
    //
    BOD.STATUS = 0x0;
    //VLMLVL VLM Disabled; 
    BOD.VLMCTRLA = 0x0;

    return 0;
}

ISR(BOD_VLM_vect)
{
	/* Insert your AC interrupt handling code here */

	/* The interrupt flag has to be cleared manually */
	BOD.INTFLAGS = BOD_VLMIE_bm;
}

