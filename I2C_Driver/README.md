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
### START Condition
<img src = "I2C_Images/Figure_I2C_Start.PNG" width="260" height="130" hspace="300">

If teh **SDA** line goes ***LOW*** when the Clock is ***HIGH*** then it is interpreted as **START Condition**.

### Address Phase
<img src = "I2C_Images/Figure_I2C_Address_Phase.PNG" width="500" height="250" hspace="150" >

Afte the **START** bit, the **Address Phase** comes as discussed before.  During the address phase the **SDA** will make the transition when the clock is low.  After 7 Clock-Cycle the address is sent and at 8th Clock-Cycle master sends out R/W bit. At the 8th Clock-Cycle if SDA is high then the master wants to read and if SDA  is low then the master wants to write. This is how the address phase works in accordance with the clock.

### ACK and NACK
<img src = "I2C_Images/Figure_I2C_ACKNACK.PNG"  >

When the master sends a 7-Bit address, if it matches the slave address then the slave will pull the SDA to low at 9th Clock-Cycle.
If **SDA** is ***LOW*** at the 9th Clock-Cycle then it is interpreted as **ACK**. If **SDA** is ***HIGH*** at the 9th Clock-Cycle then
it is interpreted as **NACK**.

### STOP Condition
<img src = "I2C_Images/Figure_I2C_Stop.PNG" width="220" height="140" hspace="300">

When the Clock is ***HIGH***, if the **SDA** makes ***LOW to HIGH*** transition, then it is interpreted as **STOP** Condition.

##   I2C Clock Stretching
* Clock stretching is one of the most powerful features of I2C protocol which is used to slow down the communication. Clock Stretching simply means that holding the clock to ***Low*** Level (Ground Level).
* The Moment the clock is held low, the entire I2C interface pauses until the clock is given back to normal condition.
* In I2C Master determines the clock speed. There are situations where I2C-Slave is unable to cooperate with clock speed given by the master and needs to slow down. In such cases, the slave takes the advantage of **Clock Stretching** to slow down by pulling the clock to ***Low*** level.

<img src = "I2C_Images/Figure_I2C_Clock_Stretching.PNG" width ="600" height="300" hspace="150">

 * Referring to the above figure, at the 8th Clock-Cycle it stretches the clock as it is busy. And at the 9th clock, it sends ACK. If Clock stretching is not enabled then the Master would have interpreted the High(NACK) at the 9th clock Cycle(i.e immediately after the 8th clock cycle).
* I2C hardware automatically does clock stretching, the programmer just needs to enable this feature.

## I2C Operating Modes
### 1. Fast Mode
The Fast Mode is an operating mode in I2C protocol in which the device can transmit and receive data up to **400Kbps**. The Fast Mode devices are **downward-compatible** and hence it can
communicate with **Standard Mode** devices in **0 to 100Kbps** speed I2C bus system. The STM32F407 MCU supports only Fast mode and standard mode. Some devices support Fast Mode+.
### 2. Standard Mode
In standard mode, the data transfer can reach speed up to **100Kbps**. The standard mode devices are not upward-compatible.

## I2C Duty Cycle
Most of the modern MCUs will allow varying the duty cycle of the I2C Clock. The Duty Cycle is significant and different I2C modes have a slightly different duty cycle.

**Refer: Page 48 of UM10204 I2C-bus specification and user manual**

<img src ="I2C_Images/Figure_I2C_Duty_Cycle_Table.PNG">

From the I2C specification we can see that for **Standard Mode** minimum values are ***tLOW = 4.7us** and ***tHIGH = 4.0us***. So to achieve 100KHz in standard mode we can keep ***tLOW = tHIGH = 5us*** (Which will pass the I2C specification). And for the **Fast Mode** minimum values are ***tLOW = 1.3us** and ***tHIGH = 0.6us***.

<img src ="I2C_Images/Figure_I2C_Duty_Cycle_STM.png" width="700" height="300" hspace="100">

In STM32F4xx MCUs there is two option for Fast Mode, one is FM(1:2) and FM(16:9) as shown above. 

## I2C Addressing Modes
There are two Addressing Modes in I2C :

1. 7-Bit Addressing Mode
2. 10-Bit Addressing Mode

Nowadays almost all the MCUs support both the addressing modes.

### 7-Bit Addressing Mode
<img src = "I2C_Images/Figure_I2C_7bit.PNG" width="800" height="150" hspace="40">

Here each slave is identified with **7-bit address**. As shown above, the  7-bit slave address when combined with **r/w bit *
becomes an **8-bit write address** or **8-bit read address**.

### 10-Bit Addressing Mode
<img src = "I2C_Images/Figure_I2C_10bit.PNG" width="750" height="180" hspace="70">

* To prevent the address clashes of the limited range of 7-bit address a new scheme of 10-bit addressing is used. 
* After the start bit, a leading **11110** pattern introduces a 10-bit addressing scheme.
* When the slave detects this bit pattern it understands that the master is using 10-bit addressing mode, and hence 7-bit address should not be included in this bit pattern. Ths slaves who use 7-bit addressing mode will ignore this phase.
* As shown in the above figure, the first byte contains the last two bits of the 10-bit address(i.e A9 and A8)
* The Second byte contains the remaining 8-bits of 10bit-address.

## I2C Functional Block Diagram
<img src = "I2C_Images/Figure_I2C_FunctionalBlock.PNG" width="550" height="600" hspace="150">

From the above figure,  we can see that 3 pins that are coming out are **SDA**, **SCL** and **SMBA**. The SMBA is used in SMB(System management bus) Protocol, which is almost identical to the I2C bus. Initially, Philips developed I2C, years later Intel came up with SMB which is an extension of I2C. From the functional block we can see there is one **Shift Register** and one associated **Data Register**(In case of SPI there were two buffer TX and Rx since SPI was full-duplex). Since I2C  is half-duplex one data register is sufficient. Also, there are **Address Registers** along with **Comparator**. Address comparison is done in the comparator during the address phase. There are two **Control Registers(CR1 and CR2)** and two **Status Registers(SR1&SR2)**. There is a **Clock Control Register(CCR)**,  which controls the serial clock coming out of the Pin SCL.
                                                                                        

