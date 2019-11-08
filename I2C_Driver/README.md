# I2C(Inter-Integrated Circuit)
<img src = "I2C_Images/Figure_I2C.PNG" width="800" height="340" hspace="30">

The **I2C** protocol, pronounced as an "I squared" or "I two C" is a **two-wire** serial bus protocol developed by Philips Semiconductor in 1982. Currently, I2C is widely used in interconnection of single-chip Microcontroller, Memory chips, ADC, DAC, Pressure/Temperature sensors as well as other low-speed peripherals. The I2C protocol requires  2 lines for data communication, they are **SDA(Serial Data Line)** and **SCA(Serial Clock Line)**. Each slave is identified with a **7-bit/10-bit** address, the master must know these addresses to communicate with the slave. Another interesting fact about I2C is that slave at any time become master and master can become a slave (if they incorporate the necessary hardware). The I2C is half-duplex and can transfer data up to 2 to 3Mhz.

## I2C Hardware Setup
<img src = "I2C_Images/Figure_I2C_Hardware_Setup.PNG" width="600" height="350" hspace="150">

The above figure shows the hardware setup of the I2C. Here both the pin **SDA** and **SCA** are in **open-drain** configuration. But the open-drain configuration is useless unless we provide a **pull-up/pull-down** resistor (To know more Click **[Here](https://github.com/SharathN25/STM32F407-Discovery/tree/master/GPIO_Driver#gpio-output-mode-with-open-drain-configuration)**).
In this case, it is pulled up using **pull-up resistor**, whose value may range between 4 to 10K Ohm. There is a formula to calculate these resistor's values based on factors like speed, etc. However, the programmer can activate the internal pull-up transistor of the IO pin instead of using the external transistor.

## I2C Basic Operation
<img src = "I2C_Images/Figure_I2C_Basic_operation.PNG"  width="800" height="340" hspace="50">

* Master always triggers the data communication first by generating the **START** Condition. At this point, the bus is under the control of the Master.
* Now the Master sends out a **7-Bit Slave address** along with the **one Read/Write Bit**. This is called the **address phase**. If **R/W = 1**, the master wants to read data.  If **R/W = 0**, then the master wants to write data.
* Here let us assume that R/W =0 (i.e master wants to write data). When the address phase finishes, the slave will send **ACK**(Acknowledgement) back if its address matches
with the address sent out by the master. 
* After Receiving the ACK, the master will send the data byte and for each byte slave receives, the slave will send back ACK.
* After this at any point if the master wants to stop communication it will raise the **STOP** condition, where the bus will be released.

### Address Phase
<img src = "I2C_Images/Figure_I2C_Address_Phase.PNG" width="500" height="250" hspace="150" >

Afte the **START** bit, the **Address Phase** comes as discussed before.  During the address phase the **SDA** will make the transition when the clock is low.  After 7 Clock-Cycle the address is sent and at 8th Clock-Cycle master sends out R/W bit. At the 8th Clock-Cycle if SDA is high then the master wants to read and if SDA  is low then the master wants to write. This is how the address phase works in accordance with the clock.
