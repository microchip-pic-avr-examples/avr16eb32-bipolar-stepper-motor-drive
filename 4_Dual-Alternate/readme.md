[![MCHP](../images/microchip.png)](https://www.microchip.com)

## Dual Alternate

<br><img src="../images/stepper_symbol.png">
<br>Bipolar Stepper Motor

<br>This example is showing how to drive alternatively two bipolar stepper motors using a single AVR microcontroller and two power boards. The stepper motors do not run at the same time.

<br>The bipolar stepper motor will be driven in three modes:

## Full-Step Mode
<br>In Full-Step mode, the motor rotates complete mechanical steps at once - for a standard 200 full-steps/revolution stepper motor, this would be 1.8 degrees for a sub-step. The motor's two coils are powered sequentially in four full-steps. The sequence determines the stepper motor's direction of rotation. Full-Step mode is simple to control, but can cause vibration and noise due to the larger step angle. Another drawback of using this mode is that the stepper motor's current consumption is at the highest level, having the lowest efficiency. This mode is often used in applications where the drive algorithm simplicity is more important than the smoothness of operation or precision.

## Half-Step Mode
<br>In Half-Step mode, four extra steps are inserted between the four initial full-steps, which makes the stepper motor rotate by half of its full-step angle. This effectively doubles the number of mechanical steps per revolution - for a standard 200 full-steps/revolution stepper motor, this would be 0.9 degrees for a complete step, resulting in a smoother operation and less vibration compared to the Full-Step mode. 
<br>The benefits of having a balance between simplicity, efficiency, smoothness of operation
and a lower current consumption outweigh the increased complexity of the control algorithm needed for this
mode. It also provides double resolution of the Full-Step mode.


## Microstep Mode
<br>The Microstep mode divides the full-step angle into even smaller steps, providing even smoother operation and higher precision. The number of microsteps per full-step can vary, but common values are 8, 16, 32, or even 256 microsteps per full-step. Even though this mode requires the most complex control algorithms, the current consumption is lower than in Full-Step and Half-Step modes, providing the highest precision, the least vibration and less noise. Microstep mode is often used in applications where precision and smoothness of operation are paramount, such as in CNC machines.


## Related Documentation

More details and code examples on the AVR16EB32 can be found at the following links:

- [AVR<sup>®</sup> EB Product Page](https://www.microchip.com/en-us/product/AVR16EB32)
- [AVR<sup>®</sup> EB Code Examples on GitHub](https://github.com/microchip-pic-avr-examples?q=AVR16EB32)


## Software Used

- [MPLAB® X IDE v6.20 or newer](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)
- [AVR-Ex DFP-2.9.197 or newer Device Pack](https://packs.download.microchip.com/)
- [MPLAB® XC8 compiler v2.46](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers/downloads-documentation#XC8)
- [MPLAB® Code Configurator (MCC) v 5.3.7](https://www.microchip.com/en-us/tools-resources/configure/mplab-code-configurator)
- [MPLAB® Code Configurator (MCC) Melody Core v 2.7.0 or newer](https://www.microchip.com/en-us/tools-resources/configure/mplab-code-configurator)


## Hardware Used

- [AVR<sup>®</sup> EB Curiosity Nano](https://www.microchip.com/en-us/product/AVR16EB32)
- [2 x Multi-Phase Power Board (MPPB)](www.microchip.com/en-us/development-tool/EV35Z86A)
- [AVR-EB Curiosity Nano Adaptor to MPPB](www.microchip.com/en-us/development-tool/EV88N31A)
- 2 x Motor used in the application: 17HS8401S Stepper Motor, but not mandatory
- A Voltage Power Supply (6-50V and 1-3A current capability, depending on the chosen motor)


## Setup

<br>The AVR16EB32 Curiosity Nano Development Board is used along with the MPPB, AVR-EB Cnano to MPPB Adaptor Board, two stepper motors and a voltage power supply.
<br>It is possible to use a single or double power supply to power the two MPPB's.
## Adjust the supply voltage and the current limit according to the stepper motors used. Preferably the stepper motors should be of the same model and the two MPPB's should be powered from the same voltage source.

<br><img src="../images/2stepper_commutation.png">
## !!!Caution: The signal names in the left column do not reflect the real functionality. 


<br><img src="../images/dual_setup_overview.png" height="500">
<br>The setup


## Functionality

## !!!Caution: Do not change voltage during stepper movement. Application assumes the supply voltage is stable.
## Supply voltage is checked and current adjusted only before movement.

<br>The application is periodically calling the ```Stepper_Move``` function with the parameters:
initial position, steps(to go), acceleration, deceleration, speed and vbus(bus voltage). In this implementation, the application automatically adjusts the drive amplitude according the the power supply voltage, trying to keep the current constant through the coils.

<br>The function precalculates the acceleration and deceleration time based on the speed and the number of steps the end-user wants the motor to move. 
After the computation has been finsished, the ```StepAdvance``` function is called, which controls the movement of the motor. The stepper drive schema is controlled by the ```StepAdvance``` function. ```StepAdvance``` is generating a wave 90 electrical degrees shifted.

<br>After movement completion, the ```Stepper_Move``` returns the final position.
<br>The drive is updated at every PWM cycle, once every 50 µs. 

<br>Depending on the number of steps requested and the acceleration/deceleration values, the motor may reach (figure1)or not reach(figure2) the desired speed limit. If the distance to reach the speed is too small, the motor will accelerate and then start decelerating without reaching the limit speed.

<br><img src="../images/stepper_acc_decel.png">
<br>Figure 1. The stepper motor has time to accelerate, reach and cruise at the speed limit, followed by the deceleration period.

<br><img src="../images/stepper_acc_decel_triangle.png">
<br>Figure 2. The distance to reach the speed is too small. The stepper motor will accelerate and then start decelerating without reaching the limit speed.

<br>The commutation between the two steppers is done in main.c.

<br>Stepper A is selected.
<br><img src="../images/start_stepper_a.png">

<br>Stepper B is selected.
<br><img src="../images/start_stepper_b.png">

### Full-Step
<br><img src="../images/full_step.png">
<br>Ideal waveforms of the current that flows through the coils. Each full-step represents a stepper motor mechanical movement of 1.8 degrees.

<br><img src="../images/full_step_sequence_table.png">
<br>Full-Step - 4 full-steps sequence.

<br>For a better aproximation, we are inserting intermediary steps, called half-steps.
### Half-Step
<br><img src="../images/half_step.png">
<br>Ideal waveforms of the current that flows through the coils. Each half-step represents a stepper motor mechanical movement of 0.9 degrees.

<br><img src="../images/half_step_sequence_table.png">
<br>Note: 0.707 represents sin(45deg) or cos(45deg).
<br>Half-Step - 8 half-steps sequence.

## Microstep
<br>To obtain a smoother movement and better positioning, the driving waveform is an approximation of a sinewave.
<br>In the current implementation, the 1.8 degrees precision of the full-step are divided by 32 microstep, resulting in a microstepping precision of less then 0.06 degrees.

<br><img src="../images/microstep.png">
<br>Ideal waveforms of the current that flows through the coils.


## Configuration

<br> Note: for a standard 200 full-steps/revolution stepper motor 1.8 mechanical degrees represent 360 electrical degrees (360 electrical degrees/200-steps is 1.8 mechanical degrees).

<br>The motor parameters can be configured in ```stepper.h```. The parameters that can be configured are the mechanical full-step angle and the mode in which the stepper should move(Full-Step, Half-Step or Microstep).

<br>The ```STEP_SIZE``` parameter is fixed and it represents the step angle of the stepper used in Full-Step mode. For this application, a stepper motor with 200 steps / mechanical revolution is used. 360 mechanical degrees/revolution divided by 200 steps results in 1.8 mechanical degrees/step.

<br>The ```R``` parameter is fixed and represents the motor windings resistance, expressed in [ohm].
<br>The ```I_OUT``` parameter is fixed and represents the motor peak current, expressed in [mA]
<br>The ```KV``` parameter is fixed and represents the BEMF current compensation. At higher speed, the BEMF increases significantly and needs to be compensated, in order to maintain a constant torque, by proportionally increasing the drive amplitude.

<br>The application contains an option that allows the stepper coils to still remain energised after the steper motor has finished the movement. The user can enable/disable this functionality with the help of the ```RELEASE_IN_IDLE``` flag.
<br>The flag is by default ```true```, which means that after every movement the current through the coils is stopped. If the user needs the coils to remain energised while the motor is staying, the ```RELEASE_IN_IDLE``` flag should be set to ```false```.
<br><img src="../images/user_defines_three.png">

<br>To change the stepping mode, uncomment the corresponding macro in ```stepper.h```.
<br>
<br><img src="../images/select_mode_three.png">


## Flowchart

<br>Flowchart for ```Stepper_Move``` function.
<br><img src="../images/stepper_move.png">

<br>Sub-step division:
<br>1 Full-Step = 1 sub - step
<br>1 Half-Step = 2 sub - steps
<br>1 Microstep = 32 sub - steps

<br>The ```CheckSteps``` function is called by ```Stepper_Move``` function. The function is needed to provide variable length delay inverse proportionally to the momentary speed of the stepper motor. It is using a fractional computation to avoid divisions.
<br>```time_flag``` is a shared boolean variable which is set by the TCE interrupt, every 50 microseconds.

<br>Flowchart for ```CheckSteps``` function.
<br><img src="../images/check_steps.png">


<br>To generate this project using MPLAB X IDE and the MPLAB X Code Configurator (MCC Melody, MCC Clasic is not supported on this device), follow the next steps:

<br>1. Open MPLAB X IDE and create a new project for the AVR16EB32 device.

<br>2. Open MCC from the toolbar (more information on how to install the MCC plug-in can be found [here](https://onlinedocs.microchip.com/pr/GUID-1F7007B8-9A46-4D03-AEED-650357BA760D-en-US-6/index.html?GUID-D98198EA-93B9-45D2-9D96-C97DBCA55267)).

<br>3. From the **MCC Content Manager** tab click the **Select MCC Melody** button and then click **Finish**.<br><img src="../images/add_mcc.png">
<br><img src="../images/select_mcc.png">
<br><img src="../images/mcc_finish.png">

<br>4. Click _Project>Resources>System>CLKCTRL_, then do the following configuration:
<br><img src="../images/select_clk.png">
    <br> - Disable the Prescaler enable button
<br><img src="../images/clckctrl.png">

<br>5. Click _Project>Resources>System>Interrupt Manager_, then do the following configuration:
<br><img src="../images/select_interrupt_manager.png">
    <br> - Enable Global Interrupts
<br><img src="../images/global_interrupts.png">
 
<br>6. To add the TCE module, go to _Device Resources>Drivers>Timer>TCE0_, then do the following configuration:
<br><img src="../images/add_tce.png">
    <br>  - Module Enable: Must be enabled by default, if not just toggle the button (it turns blue if enabled)
    <br>  - Clock Selection: System clock (by default the divider must be 1 - System clock)
    <br>  - Counter Direction: UP
    <br>  - Waveform Generation Mode: Single-Slope PWM mode with overflow on TOP
    <br>  - Requested Period [s]: 0.00005
    <br>  - Duty Cycle 0 [%]: 0
    <br>  - Duty Cycle 1 [%]: 0
    <br>  - Duty Cycle 2 [%]: 0
    <br>  - Duty Cycle 3 [%]: 0
    <br>  - Waveform Output n: Check the boxes from the Enable column for Waveform Output 0, 1, 2, 3
    <br>  - Scale mode: CMP values are scaled from Bottom, 50% DC (duty cycle)
    <br>  - Scaled Writing to registers: Normal
    <br>  - Amplitude Control Enable: Toggle the button (it turns blue if enabled)
    <br>  - Amplitude Value: 1
    <br>  - Generate ISR: Toggle the button (it turns blue if enabled)
    <br>  -Overflow Interrupt Enable: Toggle the button (it turns blue if enabled)
<br><img src="../images/tce1.png">
<br><img src="../images/tce2.png">

<br>7. To add the WEX module, go to _Device Resources>Drivers>WEX>WEX0_, then do the following configuration:
<br><img src="../images/add_wex.png">
  <br> - Input Matrix: Direct
  <br> - Update Source: TCE (the update condition for the output signals will be dictated by TCE)
  <br> - Override Settings: Check all the boxes from the Output Enable column for the Waveform Output [0-7]
  <br> - Dead-time Insertion Channel 0 Enable: Toggle the button (it turns blue if enabled)
  <br> - Dead-time Insertion Channel 1 Enable: Toggle the button (it turns blue if enabled)
  <br> - Dead-time Insertion Channel 2 Enable: Toggle the button (it turns blue if enabled)
  <br> - Dead-time Insertion Channel 3 Enable: Toggle the button (it turns blue if enabled)
  <br> - Requested Dead-time Low Side (μs) : 0.2
  <br> - Requested Dead-time High Side (μs) : 0.2
<br><img src="../images/wex.png">

<br>8. To add the ADC module, go to _Device Resources>Drivers>ADC>ADC0, then do the following configuration:
<br><img src="../images/add_adc.png">
  <br> - Prescaler Value: must be "System clock divided by 2" by default; if not select "System clock divided by 2"
  <br> - Sample Duratiom: 255
  <br> - ADC Enable: Toggle the button (it turns blue if enabled)
  <br> - Lef Adjust
  <br> - Free-Running mode Enable: Toggle the button (it turns blue if enabled)
  <br> - Reference select: VDD
  <br> - Analog Positive Channel Selection: ADC input pin 20
  <br> - Analog Negative Channel Selection: Ground
<br><img src="../images/adc.png">

<br>9. In the **Pin Grid View** tab check if the WEX_WO [0-7] pins are locked as outputs on PORTA. When the boxes from the Enable column in the Output Settings of WEX are checked, the pins are also locked. To change the PORT simply click a pin from another PORT in **Pin Grid View**. 

 |            Pin           |     Configuration     |
 | :---------------------:  | :----------------:    |
 |            PA0           |   TCE and WEX WO0     |
 |            PA1           |   TCE and WEX WO1     |
 |            PA2           |   TCE and WEX WO2     |
 |            PA3           |   TCE and WEX WO3     |
 |            PA4           |   TCE and WEX WO4     |
 |            PA5           |   TCE and WEX WO5     |
 |            PA7           |   TCE and WEX WO6     |
 |            PA7           |   TCE and WEX WO7     |
 |            PF4           |   ADC                 |


<br>10. In the **Pin Grid View**, in the ADC0 row, click the PC0 pin.
<br><img src="../images/pin_grid_view_dual.png">

<br>11. To add the BOD module, go to _Device Resources>System>BOD, then do the following configuration:
<br><img src="../images/add_bod.png">
<br> -BOD Operation ub Active Mode: Enabled in continuous mode
<br> -BOD Level: 2.7V
<br> -BOD Operation in Sleep Mode: Disabled
<br><img src="../images/bod.png">

<br>12. To add the UART module, go to _Device Resources>Drivers>UART>UART0, then do the following configuration:
<br><img src="../images/add_uart.png">
<br>- Requested Baudrate: 115200
<br> -Redirect Printf to UART Enable: Toggle the button (it turns blue if enabled)
<br><img src="../images/uart.png">

<br>13. In the **Project Resources** tab, click the **Generate** button so that MCC will generate all the specified drivers and configurations.
<br><img src="../images/generate_code.png">

<br>14. Add the ```main.c```, ```stepper.c``` and ```stepper.h``` files to the project.


## Operation

## !!!Caution: Do not change power supply voltage during stepper movement. Application assumes the supply voltage is stable.
## Supply voltage is checked and the current is adjusted only before movement.

1. Be sure the power supply is turned off.

2. Adjust the voltage of the power supply according to the stepper motor specs.

3. Adjust the current limit of the power supply according to the stepper motor specs.

4. Connect the power supply to the MPBB.

5. Connect the AVR16EB32 board the the MPBB Adapter.

6. Connect the MPBB Adapter to the MPBB.

7. Connect the AVR16EB32 board to the PC.

8. Open the ```avr16eb32-stepper-full-ramp-dual-mcc.X``` or solution in MPLAB X IDE.

9. Right click the project and select Set as main project.

<br><img src="../images/set_main_project_dual.png">

10. Build the ```avr16eb32-stepper-full-ramp-dual-mcc.X``` project by clicking **Clean and Build Project**.

<br><img src="../images/clean_build_project.png">

11. Click **Make and Program Device** to program the project to the board.

<br><img src="../images/program_project.png">

12. Start the power supply.


## Results

<br>On the terminal, the information about the movement of the stepper is shown.
<br><img src="../images/terminal_dual.png">
<br>Screen capture from microstep and full-ramp configuration.
<br>
<br><img src="../images/full_step_capture.png">
<br>Full-Step
<br>
<br>
<br><img src="../images/half_step_capture.png">
<br>Half-Step
<br>
<br>
<br><img src="../images/microstep_capture.png">
<br>Microstep
<br>
<br>Colours in the figures:
<br>Yellow and blue represent the voltages at the two ends of coil A,
<br>Green represents the current through coil A.
<br>
<br>On all three traces a low pass filtering from the oscilloscope is applied. This removes the PWM artifacts and makes visible the short time average values.


## Summary

<br>This application shows how to drive two stepper motors in Full-Step or Half-Step or Microstep, while controlling the acceeleration and deceleration, two stepper motors using AVR16EB32, MPBB, MPBB Adapter and a power supply.