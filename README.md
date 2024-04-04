[![MCHP](images/microchip.png)](https://www.microchip.com)

## Driving a Bipolar Stepper Motor
<br><img src="images/stepper_symbol.png">
<br>Bipolar Stepper Motor

<br>This series of examples presents how to drive a bipolar stepper motor using an AVR microcontroller, including the power stage.


## Hardware Used

- [AVR<sup>®</sup> EB Curiosity Nano](https://www.microchip.com/en-us/product/AVR16EB32)
- [Multi-Phase Power Board (MPPB)](www.microchip.com/en-us/development-tool/EV35Z86A) (* At the fourth application two MPPBs are used)
- [AVR-EB Curiosity Nano Adaptor to MPPB](www.microchip.com/en-us/development-tool/EV88N31A)
- Motor used in the application: 17HS8401S Stepper Motor, but not mandatory
- A Voltage Power Supply (6-50V and 1-3A current capability, depending on the chosen motor) (* At the fourth application two bipolar stepper motors are used)


<br>In these applications, the bipolar stepper motor will be driven in three modes:
* Full-Step
* Half-Step
* Microstep


<br> These modes will be explained in more detail in the following 4 applications. The complexity of the application will gradually increase.

## Application scenarios:
* [Basic example showing the stepper motor in Full-Step and Half-Step modes](1_Half-Step_Full-Step\readme.md)
* [Adding the microstepping feature for precision increase](2_Microstep\readme.md)
* [Adding ramps(acceleration and deceleration) for better dynamic response](3_Full-Ramp\readme.md)
* [Adding a second motor to demonstrate the PORTMUX peripheral. This scenario uses two motors alternatively. The motors do not rotate at the same time](4_Dual-Alternate\readme.md)

