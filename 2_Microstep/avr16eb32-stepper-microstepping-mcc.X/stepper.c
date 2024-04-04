#include <stdbool.h>
#include <stdint.h>
#include "mcc_generated_files/timer/tce0.h"

#define F_CPU 20000000UL
#include "util/delay.h"
#include "stepper.h"


#if STEPPING_MODE == MICROSTEP
static const uint16_t sine_lookup_table[32] = 
{
    AMP_TO_U16(0.024541229),
    AMP_TO_U16(0.073564564),
    AMP_TO_U16(0.122410675),
    AMP_TO_U16(0.170961889),
    AMP_TO_U16(0.21910124),
    AMP_TO_U16(0.266712757),
    AMP_TO_U16(0.31368174),
    AMP_TO_U16(0.359895037),
    AMP_TO_U16(0.405241314),
    AMP_TO_U16(0.44961133),
    AMP_TO_U16(0.492898192),
    AMP_TO_U16(0.53499762),
    AMP_TO_U16(0.575808191),
    AMP_TO_U16(0.615231591),
    AMP_TO_U16(0.653172843),
    AMP_TO_U16(0.689540545),
    AMP_TO_U16(0.724247083),
    AMP_TO_U16(0.757208847),
    AMP_TO_U16(0.788346428),
    AMP_TO_U16(0.817584813),
    AMP_TO_U16(0.844853565),
    AMP_TO_U16(0.870086991),
    AMP_TO_U16(0.893224301),
    AMP_TO_U16(0.914209756),
    AMP_TO_U16(0.932992799),
    AMP_TO_U16(0.949528181),
    AMP_TO_U16(0.963776066),
    AMP_TO_U16(0.97570213),
    AMP_TO_U16(0.985277642),
    AMP_TO_U16(0.992479535),
    AMP_TO_U16(0.997290457),
    AMP_TO_U16(0.999698819)
};
#else /* STEPPING_MODE */
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
#endif /* STEPPING_MODE  */


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
    
#if STEPPING_MODE == MICROSTEP
    static uint8_t step = 0;
    uint8_t x = (step & 0x60) | direction;
       
    switch(x)
    {
        /* CW */
        case 0x00:  a = 0;
                    b = sine_lookup_table[31 - (step & 0x1F)];
                    c = 0;
                    d = sine_lookup_table[step & 0x1F];;
                    break;
        case 0x20: 
                    a = sine_lookup_table[step & 0x1F];
                    b = 0;
                    c = 0;
                    d = sine_lookup_table[31 - (step & 0x1F)];
                    break;
        case 0x40:            
                    a = sine_lookup_table[31 - (step & 0x1F)];
                    b = 0;
                    c = sine_lookup_table[step & 0x1F];;
                    d = 0;
                    break;     
        case 0x60: 
                    a = 0;
                    b = sine_lookup_table[step & 0x1F];
                    c = sine_lookup_table[31 - (step & 0x1F)];
                    d = 0;                  
                    break;
        
        /* CCW */
        case 0x01: 
                    a = 0;
                    b = sine_lookup_table[31 - (step & 0x1F)];
                    c = sine_lookup_table[step & 0x1F];
                    d = 0;                  
                    break;
        case 0x21: 
                    a = sine_lookup_table[step & 0x1F];
                    b = 0;
                    c = sine_lookup_table[31 - (step & 0x1F)];
                    d = 0;          
                    break;
        case 0x41: 
                    a = sine_lookup_table[31 - (step & 0x1F)];
                    b = 0;
                    c = 0;
                    d = sine_lookup_table[step & 0x1F];            
                    break;
        case 0x61:           
                    a = 0;
                    b = sine_lookup_table[step & 0x1F];
                    c = 0;
                    d = sine_lookup_table[31 - (step & 0x1F)];            
                    break;
                    
        default:   a = 0; b = 0; c = 0; d = 0; break;           
    }
    step++; step &= 0x7F;
#endif /* STEPPING_MODE == MICRO_STEP */
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