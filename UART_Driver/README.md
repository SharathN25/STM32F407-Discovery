## USART(Universal Synchronous Asynchronous Receiver Transmitter) and UART(Universal Asynchronous Receiver Transmitter)
These are the hardware that converts parallel data into serial data. **UART** supports only asynchronous mode and **USART** supports both synchronous and asynchronous modes. All the modern MCUs come with USART. Unlike USB, Ethernet, there is no specific port for USART communication. They are commonly used in conjunction with protocols like RS-232, RS-432, USB, etc. In Asynchronous transmission, a clock is not sent along with the data, instead synchronous bits like **start** and **stop** bits are used whereas in synchronous transmission a separate clock is sent along with the data hence start and stop bits are not required. 

Typically USART hardware will have the following components

* Baud-Rate Generator
* TX and RX Shift Registers
* Transmit/Receive Control Block
* Transmit/Receive Buffers
* First-In-First-Out( FIFO ) Buffer Memory

### Understanding UART Pins
<img src = "UART_Images/FIgure_UART_Pins.PNG" width="700" height="230" hspace="50" >

The UART Bi-Directional communication requires at least 2 pins **TX** and **RX**(When hardware flow control is not used). If we are not transmitting anything then the  TX line is held High which is the idle state. To receive data on the RX line, the UART module(UART RX Engine) continuously samples this line to detect the start bit of the frame. When the start bit is detected, frame reception starts on the RX line. The **RTS(Request to send)** and **CTS(Clear to send)** are required when hardware flow control is used. The CTS is an active-low pin when hardware flow control is used data transmission on the TX pin happens only when the CTS pin is held low.  The RTS is also an active-low pin, the UART module uses this line to request data from other devices. Referring above figure, when UART-1 wants data from UART-2, RTS of UART-1 goes low which makes CTS of UART-2 to go low and data transmission occurs between these two.

### UART Frame format
<img src = "UART_Images/FIgure_UART_Frame.PNG" width="680" height="290" hspace="90" >

The frame refers to the entire data packet which is being sent or received during the communication. The typical frame format of the UART has this order of bits as shown above. The UART frame start with **start-bit** which is always low for the 1-bit duration. Then follows the data bits from LSB to MSB, this usually occupies 5 to 9 bits. We can decide how many data bits have to be there in a frame between 5 to 9 bits using configuration register in UART peripheral. Then follows the **parity-bit** which is optional. Finally, frame ends with the **stop-bit*** which is always high, we can configure stop bit duration to be either 1, 1.5 or 2-bit duration using configuration register.

### Baud Rate
The significance of the baud-rate is **how fast** the data is sent over the serial line. It's usually expressed in units of bits-per-second(bps). If you invert the baud-rate, you can find out, just how long it takes to transmit a single bit. This value determines how long the transmitter holds the serial line high or low.

<img src = "UART_Images/FIgure_UART_Baud_Rate.PNG" width="680" height="290" hspace="90" >

The baud rate can be just about any value, the only requirement is that both transmitting and receiving devices must operate at the same baud rate. Some of the commonly used baud rates are 2400,4800,9600,19200,38400, 57600 and 115200. Higher the baud-rate the faster is data sent/received, however, there are limits. The baud-rate is usually dependent on UART peripheral clock frequency. 

### UART Synchronization bits
<img src = "UART_Images/FIgure_UART_Synch_Bits.PNG" width="680" height="270" hspace="90" >

The start and stop bits mark the start and end of a frame. There is always one start bit but stop bit is configurable to 1 or 2 bits, in STM32F4xx MCU we can even choose 1.5 bits. The start bit is always indicated by an idle line going from high to low, Stop bit is indicated by the transition back to idle state by holding the line high.

### UART Parity
Adding the parity bit is the simplest method of error detection. The Parity is simply the number of One's(1's) present in the binary form of a number.

<img src ="UART_Images/Figure_UART_Parity.PNG" width="500" height="100" hspace="200">

As shown in the above figure, 55 in binary form has 5 One's in it hence parity is 5. There are two options in parity, one is Even parity and another is Odd Parity.

#### Even Parity
* If Even parity option is selected, then if the number has an odd number of 1's in the binary form, the parity bit is made 0, so that total number of bits including the parity bit has an even number of 1's as shown below.

<img src = "UART_Images/Figure_UART_Even_Parity1.PNG" width="600" height="220" hspace="150" >

* If the number has an even number of 1's in the binary form, then the parity bit is made 0, so that total number of 1's in the number including parity bit is even as shown below. 

<img src = "UART_Images/Figure_UART_Even_Parity2.PNG" width="600" height="220" hspace="150" >


#### Odd Parity
* If odd parity option is selected, then if the number has even number of 1's in its binary form, then the  parity bit is made 1 so that total number of bits including  parity bit becomes odd as shown below

<img src = "UART_Images/Figure_UART_Odd_Parity1.PNG" width="600" height="220" hspace="150" >

* If the number has an odd number of 1's in its binary form, then the parity bit is made 0 so that the total number of 1's including parity bit is odd as shown below.



<img src = "UART_Images/Figure_UART_Odd_Parity2.PNG" width="600" height="220" hspace="150" >

### USART Functional Block Diagram
**Refer : Figure 296. USART block diagram (Page 968 of RM0090)** 

<img src = "UART_Images/Figure_UART_Functional_Block_Diagram.PNG" width="700" height="700" hspace="80">

This hardware block can be used for both **synchronous** and **asynchronous** modes. The 4 important pins which are used in UART communication are TX, RX, RTS, and CTS. If this hardware block is used in synchronous mode then serial clock **CK** is used. The UART is full-duplex hence we can transmit and receive simultaneously.  For TX and RX functionality there are two data registers- **TDR Transmit data register)** and **RDR(receive data register)**. Each data register has its associated shift register. A couple of Control registers used to control the TX and RX block. At the bottom section of the figure, we can see the baud-rate generator. USART_BRR register must be configured with correct DIV_Mantissa and DIV_Fraction to produce the desired baud rate.

#### UART Peripheral Clock
Referring [STM32F407VGT Block Diagram](https://github.com/SharathN25/STM32F407-Discovery#overview-of-stm32f407vgt6-microcontroller),
we can see that **USART1** and **USART6** are connected to **APB2(Max 84Mhz)** Bus, ideally these two peripheral should be able to run at 84Mhz, but when the microcontroller is powered by internally RC oscillator of 16Mhz, maximum peripheral clock that the USART hardware get is 16Mhz as shown below.

<img src = "UART_Images/Figure_UART_Peripheral_Clock.PNG" width="580" height="280" hspace="150" >

The **USART2**, **USART3**, **UART4** and **UART5** are connected to **APB1(Max 42Mhz)** Bus. This peripheral clock frequency is used by the UART baud-rate generation block to produce different baud-rates.
 
 
### UART Transmitter
<img src = "UART_Images/Figure_UART_Transmitter.PNG" 
