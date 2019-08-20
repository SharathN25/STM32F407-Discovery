# Understanding STM32F407 Discovery Board
The main purpose of this project is to get an insight into the STM32F407 Discovery Board, which is an ARM Cortex M4 based Microcontroller. As I started working on STM32F07 Discovery Board, initially it was difficult and confusing to understand and program this microcontroller because understanding internal structures and working of the microcontroller using datasheet of STM32F407VGT chipset is difficult especially if one is a beginner.

This project gives almost all the basic information needed to get started with STM32F407 Discovery Board and also development of driver code.

**Hardware Used** : STM32F4 DISCOVERY kit, for more information visit https://www.st.com/en/evaluation-tools/stm32f4discovery.html

**Software Tool** : Kiel MDK V5, for more information visit http://www2.keil.com/mdk5/. 
                For installation of Keil MDK V5 refer https://youtu.be/d_O2tu5CMbQ

**Note** : As this microcontroller has many advanced features and the main aim of this project is to get all basic insights, during the driver development only the required functionalities are added and other advanced functionality is not added. I may update the driver and other functionality in the future.

Please find the STM32F4 Discovery User Manual,STM32F4xxx Reference Manual (RM0090) and other related documents inside a folder called Documents. I will be referring to these documents for information such as block diagrams, register details ect.


# Overview of STM32F407VGT6 Microcontroller
#### Please refer : Figure 6. STM32F407VGT6 block diagram from STM32F4 Discovery User Manual (Page 12).

The STM32F407 Discovery board uses STM32F407VGT6 Microcontroller which has **ARM Cortex-M4F** Processor, which is capable of running upto **168Mhz**. This MCU has many peripherals such as GPIO ports, TIMERS, ADCs, DACs, Flash Memory, SRAM, SPI, UART ect. The processor and peripherals talk via **BUS-Interface**.  There are three busses available :-
1. **I-BUS** (Instruction Bus)   
2. **D-BUS** (Data Bus)
3. **S-BUS** (System Bus)

**I-BUS**
This bus connects the Instruction bus of the Cortex®-M4 with FPUFloating point unit) core to the BusMatrix. This bus is used by the core to fetch instructions. The target of this bus is a memory containing code (internal Flash memory/SRAM or external memories through the FSMC/FMC).

**D-BUS**
This bus connects the databus of the Cortex®-M4 with FPU to the 64-Kbyte CCM data RAM to the BusMatrix. This bus is used by the core for literal load and debug access. The target of this bus is a memory containing code or data (internal Flash memory or external memories through the FSMC/FMC).

**S-BUS**
This bus connects the system bus of the Cortex®-M4 with FPU core to a BusMatrix. This bus is used to access data located in a peripheral or in SRAM. Instructions may also be fetched on this bus (less efficient than ICode). The targets of this bus are the internal SRAM1, SRAM2 and SRAM3, the AHB1 peripherals including the APB peripherals, the AHB2 peripherals and the external memories through the FSMC/FMC.

So instructions and data uses I-bus and D-bus respectively, All the other peripheral uses System bus. The Cortex-M4 processor contains three external Advanced High-performance Bus (AHB)-Lite bus interfaces(AHB1,AHB2a and AHB3) and one Advanced Peripheral Bus (APB) interface. The AHB1 bus 
