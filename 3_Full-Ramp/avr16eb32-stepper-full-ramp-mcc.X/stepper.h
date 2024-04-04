#ifndef STEPPER_H
#define STEPPER_H


#include <stdint.h>
#include <avr/io.h>


/* Total number of steps a motor spins in a duration */
typedef int32_t  stepper_position_t;

/* Set ADC input */
#define VBUS_ADC                                ADC_MUXPOS_AIN20_gc

/* USER DEFINE CONFIGS*/
/* MOTOR SPECIFIC*/
#define R                  2.6                  /* Motor Windings Resistance  [ohm] */
#define I_OUT              500.0                /* Motor Output Current Limit [mA] */
#define STEP_SIZE          1.8                  /* Motor degrees / full-step */
#define KV                 5.6                  /* Proportionality constant for BEMF compensation 1.0 ... 10.0 */
#define RELEASE_IN_IDLE    true                 /* True: for power savings, the current through the coils is stopped. */


/* Select the desired stepping mode(only one of them) */
//#define STEPPING_MODE FULL_STEP
//#define STEPPING_MODE HALF_STEP
#define STEPPING_MODE MICRO_STEP


/* BOARD SPECIFIC*/
#define K_VBUS          16.0                    /* Board voltage divider ratio */
#define ADC_VREF        3.3                     /* ADC Voltage Reference */


/* Sets the amplitude of the sine wave signals, and thus the scaling values of duty cycle in U.Q.1.15 format, ranging from 0 to 1.00 
 * Duty cycle scaling is done in hardware using the hardware accelerator of TCE.*/
#define AMP_TO_U16(X)                           (uint16_t)(32768.0*(X) + 0.5)


#define DRIVE_FULL                              AMP_TO_U16(1.0)
#define DRIVE_HALF                              AMP_TO_U16(0.707)
#define DRIVE_ZERO                              AMP_TO_U16(0.0)


/* Definitions for stepper drive modes */
#define HALF_STEP   1
#define FULL_STEP   2
#define MICRO_STEP  3


#if STEPPING_MODE == FULL_STEP
#define K_MODE         1
#define STRING         "Full-Step"
#endif 

#if STEPPING_MODE == HALF_STEP
#define K_MODE         2
#define STRING         "Half-Step"
#endif 

#if STEPPING_MODE == MICRO_STEP
#define K_MODE         32
#define STRING         "Microstep"
#endif 


/*PWM Interrupt Interval */
#define TICK_INTERVAL   50.0                    /* Microseconds */


/* DEGPS - degrees per second */
/* Converts degrees per second into 16 bit integer */
#define DEGPS_TO_U16(dps)                       (uint16_t)(((dps) * 65536.0 * TICK_INTERVAL * K_MODE) / (STEP_SIZE * 1000000.0) + 0.5)

/* Converts 16 bit integer into degrees per second */
#define U16_TO_DEGPS(u16)                       (float)((STEP_SIZE * (u16) * 1000000.0) / (65536.0 * TICK_INTERVAL * K_MODE))

/* Speed limit */
#define  SPEED_LIMIT(SPEED_U16)                 (uint16_t)(((SPEED_U16) > 32768) ? (32768) : (SPEED_U16))

/* Converts steps into substeps */
#define STEPS_TO_SUBSTEPS(STEPS)                (stepper_position_t)((STEPS)*(float)K_MODE)
#define SUBSTEPS_TO_STEPS(SUBST)                ((float)(SUBST)/(float)K_MODE)


/* Function Prototypes*/
/* params:
    steps: if negative, then go CCW, if positive - go CW
    acceleration, deceleration: steps / s^2
    speed: steps/s
    vbus: vbus expressed in mV

  returns: new position of the stepper motor
*/
stepper_position_t Stepper_Move(stepper_position_t, stepper_position_t, uint16_t, uint16_t, uint16_t, uint16_t);
void               Stepper_TimeTick(void);  /* Called periodically from interrupt context */
void               Stepper_Init(void);

#endif /*  STEPPER_H  */