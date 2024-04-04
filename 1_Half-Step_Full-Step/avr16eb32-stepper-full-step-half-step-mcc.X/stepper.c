#include <stdbool.h>
#include <stdint.h>
#include "mcc_generated_files/timer/tce0.h"

#define F_CPU 20000000UL
#include "util/delay.h"
#include "stepper.h"


typedef enum
{
    CW_STEP_0 = 0,
    CW_STEP_45 = 1,
    CW_STEP_90 = 2,
    CW_STEP_135 = 3,
    CW_STEP_180 = 4,
    CW_STEP_225 = 5,
    CW_STEP_270 = 6,
    CW_STEP_315 = 7,
    
    
    CCW_STEP_0 = 8,
    CCW_STEP_45 = 9,
    CCW_STEP_90 = 10,
    CCW_STEP_135 = 11,
    CCW_STEP_180 = 12,
    CCW_STEP_225 = 13,
    CCW_STEP_270 = 14,
    CCW_STEP_315 = 15
} step_t;

/* This function controls the speed of the stepper */
static void Variable_Delay(uint16_t count)
{
    volatile uint16_t _count = count;
    
    while(_count >  0)
    {
        _delay_us(Q_DELAY);
        _count--;
    }
}

/* If parameter direction is True means that motor will spin in CCW */
static void StepAdvance(bool direction)
{  
    uint16_t a = 0, b = 0, c = 0, d = 0;

#if STEPPING_MODE == FULL_STEP    
    static step_t step = 0;
    /*These masks are correlated with step_t */
    step &= 7;
    if(direction) step |= 8;

    switch(step)
    {
        case CW_STEP_270:   a = DRIVE_FULL; b = DRIVE_ZERO; c = DRIVE_ZERO; d = DRIVE_FULL; step = CW_STEP_180;     break;
        case CW_STEP_180:   a = DRIVE_FULL; b = DRIVE_ZERO; c = DRIVE_FULL; d = DRIVE_ZERO; step = CW_STEP_90;      break;
        case CW_STEP_90:    a = DRIVE_ZERO; b = DRIVE_FULL; c = DRIVE_FULL; d = DRIVE_ZERO; step = 0;               break;
        case CW_STEP_0:     a = DRIVE_ZERO; b = DRIVE_FULL; c = DRIVE_ZERO; d = DRIVE_FULL; step = CW_STEP_270;     break;

        case CCW_STEP_0:    a = DRIVE_ZERO; b = DRIVE_FULL; c = DRIVE_ZERO; d = DRIVE_FULL; step = CCW_STEP_90;     break;                  
        case CCW_STEP_90:   a = DRIVE_ZERO; b = DRIVE_FULL; c = DRIVE_FULL; d = DRIVE_ZERO; step = CCW_STEP_180;    break;
        case CCW_STEP_180:  a = DRIVE_FULL; b = DRIVE_ZERO; c = DRIVE_FULL; d = DRIVE_ZERO; step = CCW_STEP_270;    break;
        case CCW_STEP_270:  a = DRIVE_FULL; b = DRIVE_ZERO; c = DRIVE_ZERO; d = DRIVE_FULL; step = CCW_STEP_0;      break;

        default:            a = DRIVE_ZERO; b = DRIVE_ZERO; c = DRIVE_ZERO; d = DRIVE_ZERO; step = CW_STEP_0;       break; 
    }   
#endif /* STEPPING_MODE==FULL_STEP */

#if STEPPING_MODE == HALF_STEP
    static step_t step = 0;
    /*These masks are correlated with step_t */
    step &= 7;
    if(direction) step |= 8;
    
    switch(step)
    {
        case CW_STEP_315:  a = DRIVE_ZERO; b = DRIVE_HALF; c = DRIVE_ZERO; d = DRIVE_HALF; step = CW_STEP_270;      break;  
        case CW_STEP_270:  a = DRIVE_ZERO; b = DRIVE_ZERO; c = DRIVE_ZERO; d = DRIVE_FULL; step = CW_STEP_225;      break;
        case CW_STEP_225:  a = DRIVE_HALF; b = DRIVE_ZERO; c = DRIVE_ZERO; d = DRIVE_HALF; step = CW_STEP_180;      break;
        case CW_STEP_180:  a = DRIVE_FULL; b = DRIVE_ZERO; c = DRIVE_ZERO; d = DRIVE_ZERO; step = CW_STEP_135;      break;
        case CW_STEP_135:  a = DRIVE_HALF; b = DRIVE_ZERO; c = DRIVE_HALF; d = DRIVE_ZERO; step = CW_STEP_90;       break;
        case CW_STEP_90:   a = DRIVE_ZERO; b = DRIVE_ZERO; c = DRIVE_FULL; d = DRIVE_ZERO; step = CW_STEP_45;       break;
        case CW_STEP_45:   a = DRIVE_ZERO; b = DRIVE_HALF; c = DRIVE_HALF; d = DRIVE_ZERO; step = CW_STEP_0;        break;
        case CW_STEP_0:    a = DRIVE_ZERO; b = DRIVE_FULL; c = DRIVE_ZERO; d = DRIVE_ZERO; step = CW_STEP_315;      break;  
        
        case CCW_STEP_0:   a = DRIVE_ZERO; b = DRIVE_FULL; c = DRIVE_ZERO; d = DRIVE_ZERO; step = CCW_STEP_45;      break;                  
        case CCW_STEP_45:  a = DRIVE_ZERO; b = DRIVE_HALF; c = DRIVE_HALF; d = DRIVE_ZERO; step = CCW_STEP_90;      break;
        case CCW_STEP_90:  a = DRIVE_ZERO; b = DRIVE_ZERO; c = DRIVE_FULL; d = DRIVE_ZERO; step = CCW_STEP_135;     break;
        case CCW_STEP_135: a = DRIVE_HALF; b = DRIVE_ZERO; c = DRIVE_HALF; d = DRIVE_ZERO; step = CCW_STEP_180;     break;
        case CCW_STEP_180: a = DRIVE_FULL; b = DRIVE_ZERO; c = DRIVE_ZERO; d = DRIVE_ZERO; step = CCW_STEP_225;     break;                  
        case CCW_STEP_225: a = DRIVE_HALF; b = DRIVE_ZERO; c = DRIVE_ZERO; d = DRIVE_HALF; step = CCW_STEP_270;     break;
        case CCW_STEP_270: a = DRIVE_ZERO; b = DRIVE_ZERO; c = DRIVE_ZERO; d = DRIVE_FULL; step = CCW_STEP_315;     break;
        case CCW_STEP_315: a = DRIVE_ZERO; b = DRIVE_HALF; c = DRIVE_ZERO; d = DRIVE_HALF; step = CCW_STEP_0;       break;
        
        default:           a = DRIVE_ZERO; b = DRIVE_ZERO; c = DRIVE_ZERO; d = DRIVE_ZERO; step = CW_STEP_0;        break;               
    }    
#endif /* STEPPING_MODE==HALF_STEP */
    TCE0_CompareAllChannelsBufferedSet(a, b, c, d);
}


void Stepper_Init(void)
{
    /* Enable hardware scaling accelerator after initialization */
    TCE0_ScaleEnable(true);
    TCE0_AmplitudeSet(DRIVE_ZERO);
    TCE0_CompareAllChannelsBufferedSet(DRIVE_ZERO, DRIVE_ZERO, DRIVE_ZERO, DRIVE_ZERO);
}

static inline void AmplitudeSet(uint16_t amplitude)
{
    if(amplitude > 32768)
        amplitude = 32768;
    TCE0_AmplitudeSet(amplitude);
}

stepper_position_t Stepper_Move(stepper_position_t initial_position, stepper_position_t steps, uint16_t delay)
{  
    stepper_position_t actual_position = initial_position;
    bool direction;
    uint32_t steps_to_go;

    if(steps < 0)
    {
        direction = true;
        steps_to_go = (uint32_t)(0 - steps);
    }
    else
    {
        direction = false;
        steps_to_go = (uint32_t)steps;
    }
        
    AmplitudeSet(AMPLITUDE_DRIVE);
    
    /* Now start moving */
    while(steps_to_go != 0)
    {
        Variable_Delay(delay);

        steps_to_go--;
        StepAdvance(direction);
        if(direction) actual_position--;
        else          actual_position++;
    }
    /* Movement completed. Now the motor is stopped. */
    AmplitudeSet(AMPLITUDE_STALL);
    
#if (RELEASE_IN_IDLE == true)
    /* Release the current through coils */
    TCE0_CompareAllChannelsBufferedSet(DRIVE_ZERO, DRIVE_ZERO, DRIVE_ZERO, DRIVE_ZERO);
#endif /* RELEASE_IN_IDLE */
    
    return actual_position;
}