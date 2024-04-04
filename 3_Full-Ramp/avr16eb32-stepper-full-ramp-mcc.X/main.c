 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
*/

/*
© [2023] Microchip Technology Inc. and its subsidiaries.

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
#include <stdio.h>
#include "mcc_generated_files/system/system.h"
#include "util/delay.h"
#include "stepper.h"

/*  Function that returns the voltage expressed in mV */
uint16_t Get_VBus(adc_0_channel_t channel)
{
    uint16_t vbus_mv;
    ADC0_StartConversion(channel);
    while(ADC0_IsConversionDone() == 0);
    _delay_us(50);
    vbus_mv = (uint16_t)(K_VBUS * ADC_VREF * (float)ADC0_GetConversionResult() / 65.536);   
    return vbus_mv;
}

stepper_position_t MainMove(stepper_position_t position, stepper_position_t displacement, uint16_t speed)
{
    uint16_t acc   = DEGPS_TO_U16(0.3);
    uint16_t decc  = DEGPS_TO_U16(0.3);
    uint16_t vbus;

    vbus = Get_VBus(VBUS_ADC);
    printf("\n\rSupply voltage: \t%.2f V", 0.001*(float)vbus);
    printf("\n\rInitial position:\t%.2f steps / %ld sub-steps", SUBSTEPS_TO_STEPS(position), position);
    printf("\n\rMoving with speed:\t%.3f degrees/second", U16_TO_DEGPS(speed));
    position = Stepper_Move(position, displacement, acc, decc, speed, vbus);
    printf("\n\rFinal position: \t%.2f steps / %ld sub-steps", SUBSTEPS_TO_STEPS(position), position);
    printf("\n\r");
    return position;
}

int main(void)
{
    stepper_position_t stepper_position = 0;

    /* System initialize */
    SYSTEM_Initialize();

    /*Register the Stepper_TimeTick as a callback*/
    TCE0_OverflowCallbackRegister(Stepper_TimeTick);

    Stepper_Init();
    
    _delay_ms(2000);
    printf("\n\r-----------------------------------------------");
    printf("\n\rStepping Mode: %s, 1 step = %d sub-steps", STRING, K_MODE);
    printf("\n\r");

    while(1)
    {
        stepper_position_t sub_steps;
        uint16_t           speed;

        sub_steps = STEPS_TO_SUBSTEPS(400);        /* Positive: CW, Negative: CCW */
        speed = SPEED_LIMIT(DEGPS_TO_U16(360));    /* Degrees per second */

        stepper_position = MainMove(stepper_position,
                                    sub_steps,
                                    speed);
        _delay_ms(500);
        
        sub_steps = -STEPS_TO_SUBSTEPS(200);       /* Positive: CW, Negative: CCW */
        speed = SPEED_LIMIT(DEGPS_TO_U16(180));    /* Degrees per second */

        stepper_position = MainMove(stepper_position,
                                    sub_steps,
                                    speed);
        _delay_ms(500);
    }
}

