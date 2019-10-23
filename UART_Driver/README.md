## USART(Universal Synchronous Asynchronous Receiver Transmitter) and UART(Universal Asynchronous Receiver Transmitter)
These are the hardware that converts parallel data into serial data. **UART** supports only asynchronous mode and **USART** supports both synchronous and asynchronous modes. All the modern MCUs come with USART. Unlike USB, Ethernet, there is no specific port for USART communication. They are commonly used in conjunction with protocols like RS-232, RS-432, USB, etc. In Asynchronous transmission, a clock is not sent along with the data, instead synchronous bits like **start** and **stop** bits are used whereas in synchronous transmission a separate clock is sent along with the data hence start and stop bits are not required. 

Typically USART hardware will have the following components

* Baud-Rate Generator
* TX and RX Shift Registers
* Transmit/Receive Control Block
* Transmit/Receive Buffers
* First-In-First-Out( FIFO ) Buffer Memory

### Understanding UART Pins
<img src = "UART_Images/FIgure_UART_Pins.PNG">
