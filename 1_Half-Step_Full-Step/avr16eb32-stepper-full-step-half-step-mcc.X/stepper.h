#ifndef STEPPER_H
#define STEPPER_H


#include <stdint.h>


/* Total number of steps a motor spins in a duration */
typedef int32_t  stepper_position_t;


/* USER DEFINE CONFIGS*/
/* MOTOR SPECIFIC*/
#define AMPLITUDE_DRIVE                         AMP_TO_U16(0.25)    /* adjust these values according to your motor and supply voltage */
#define AMPLITUDE_STALL                         AMP_TO_U16(0.05)

#define STEP_SIZE          1.8                  /* Motor degrees / full-step */
#define RELEASE_IN_IDLE    true                 /* True: for power savings, the current through the coils is stopped. */

/* Select the desired stepping mode(only one of them) */
//#define STEPPING_MODE FULL_STEP
#define STEPPING_MODE HALF_STEP


/* Sets the amplitude of the sine wave signals, and thus the scaling values of duty cycle in U.Q.1.15 format, ranging from 0 to 1.00 
 * Duty cycle scaling is done in hardware using the hardware accelerator of TCE.*/
#define AMP_TO_U16(X)                           (uint16_t)(32768.0*(X) + 0.5)


#define DRIVE_FULL                              AMP_TO_U16(1.0)
#define DRIVE_HALF                              AMP_TO_U16(0.707)
#define DRIVE_ZERO                              AMP_TO_U16(0.0)


/* Definitions for stepper drive modes */
#define HALF_STEP   1
#define FULL_STEP   2


#if STEPPING_MODE == FULL_STEP
#define K_MODE         1
#define STRING         "Full-Step"
#endif 

#if STEPPING_MODE == HALF_STEP
#define K_MODE         2
#define STRING         "Half-Step"
#endif 


#define Q_DELAY         24                      /* Microseconds */
#define OVERHEAD_DELAY  3


/* Convert revolutions/second into count and inverse */
#define RPS_TO_COUNT(REVPS)                     (uint16_t)((1000000.0 / ((Q_DELAY + OVERHEAD_DELAY) * (360.0 / STEP_SIZE) * K_MODE * (REVPS))) + 0.5) /* count */
#define COUNT_TO_RPS(COUNT)                                (1000000.0 / ((Q_DELAY + OVERHEAD_DELAY) * (360.0 / STEP_SIZE) * K_MODE * (COUNT)))        /* rev/s */


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
stepper_position_t Stepper_Move(stepper_position_t, stepper_position_t, uint16_t);
void               Stepper_Init(void);

#endif /*  STEPPER_H  */