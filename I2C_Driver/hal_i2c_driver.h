/**************************************************************************************************************************
 * @file     hal_i2c_driver.h
 * @author   Sharath N 
 * @brief    Header file for I2C Driver of STM32F407 Discovery Baord.
 **************************************************************************************************************************/
 
#ifndef __HAL_I2C_DRIVER_H
#define __HAL_I2C_DRIVER_H

/*MCU specific header file for stm32f407vgt6 base discovery board */
#include "stm32f407xx.h"
#include <stdint.h>

/******************************************************************************************************************************/
/*                                                                                                                            */
/*                                            1. I2C Register bit definition                                                  */
/*                                                                                                                            */
/******************************************************************************************************************************/

/***********************************Bit Definition for I2C_CR1 Register********************************************************/

/*Acknowledge/PEC Position (for data reception)*/
#define I2C_REG_CR1_POS                                                ((uint32_t) 1 << 11)

/*Acknowledge enable */
#define I2C_REG_CR1_ACK                                                ((uint32_t) 1 << 10)
#define I2C_ACK_ENABLE                                                 1
#define I2C_ACK_DISABLE                                                0

/* I2C start and stop generation */
#define I2C_REG_CR1_STOP_GEN                                           ((uint32_t) 1 << 9)
#define I2C_REG_CR1_START_GEN                                          ((uint32_t) 1 << 8)

/* Clock stretching disable (Slave mode)*/
#define I2C_REG_CR1_NOSTRETCH                                          ((uint32_t) 1 << 7)
#define I2C_ENABLE_CLK_STRETCH                                         0
#define I2C_DISABLE_CLK_STRETCH                                        0

/* I2C Peripheral enable*/
#define I2C_REG_CR1_ENABLE_I2C                                         ((uint32_t) 1 << 0)


/***********************************Bit Definition for I2C_CR2 Register********************************************************/

/* Enable the interrupt of buffer, event and error */
#define I2C_REG_CR2_BUF_INT_ENABLE                                      ((uint32_t) 1 << 10)
#define I2C_REG_CR2_EVT_INT_ENABLE                                      ((uint32_t) 1 << 9)
#define I2C_REG_CR2_ERR_INT_ENABLE                                      ((uint32_t) 1 << 8)

/*I2C Peripheral clock frequency */
#define I2C_PERIPHERAL_CLK_FREQ_2MHZ                                    ((uint32_t) 2)
#define I2C_PERIPHERAL_CLK_FREQ_3MHZ                                    ((uint32_t) 3)
#define I2C_PERIPHERAL_CLK_FREQ_4MHZ                                    ((uint32_t) 4)
#define I2C_PERIPHERAL_CLK_FREQ_5MHZ                                    ((uint32_t) 5)
#define I2C_PERIPHERAL_CLK_FREQ_6MHZ                                    ((uint32_t) 6)
#define I2C_PERIPHERAL_CLK_FREQ_7MHZ                                    ((uint32_t) 7)
#define I2C_PERIPHERAL_CLK_FREQ_8MHZ                                    ((uint32_t) 8)
#define I2C_PERIPHERAL_CLK_FREQ_9MHZ                                    ((uint32_t) 9)
#define I2C_PERIPHERAL_CLK_FREQ_10MHZ                                   ((uint32_t) 10)
#define I2C_PERIPHERAL_CLK_FREQ_11MHZ                                   ((uint32_t) 11)
#define I2C_PERIPHERAL_CLK_FREQ_12MHZ                                   ((uint32_t) 12)
#define I2C_PERIPHERAL_CLK_FREQ_13MHZ                                   ((uint32_t) 13)
#define I2C_PERIPHERAL_CLK_FREQ_14MHZ                                   ((uint32_t) 14)
#define I2C_PERIPHERAL_CLK_FREQ_15MHZ                                   ((uint32_t) 15)
#define I2C_PERIPHERAL_CLK_FREQ_16MHZ                                   ((uint32_t) 16)



/***********************************Bit Definition for I2C_OAR1 Register********************************************************/

/*Addressing mode (slave mode)*/
#define I2C_REG_OAR1_ADDRMODE                                           ((uint32_t) 1 << 15)
#define I2C_ADDRMODE_7BIT                                               0
#define I2C_ADDRMODE_10BIT                                              1

/*Bit 14 Should always be kept at 1 by software*/
#define I2C_REG_OAR1_14TH_BIT                                           ((uint32_t) 1 << 14)

#define I2C_REG_OAR1_7BIT_ADDRESS_POS                                   1



/***********************************Bit Definition for I2C_SR1 Register*********************************************************/

#define I2C_REG_SR1_TIMEOUT_FLAG                                       ((uint32_t) 1 << 14)
#define I2C_REG_SR1_OVR_FLAG                                           ((uint32_t) 1 << 11)
#define I2C_REG_SR1_AF_FAILURE_FLAG                                    ((uint32_t) 1 << 10)
#define I2C_REG_SR1_ARLO_FLAG                                          ((uint32_t) 1 << 9)
#define I2C_REG_SR1_BUSS_ERROR_FLAG                                    ((uint32_t) 1 << 8)
#define I2C_REG_SR1_TXE_FLAG                                           ((uint32_t) 1 << 7)
#define I2C_REG_SR1_RXNE_FLAG                                          ((uint32_t) 1 << 6)
#define I2C_REG_SR1_ARLO_FLAG                                          ((uint32_t) 1 << 9)
#define I2C_REG_SR1_STOP_DETECTION_FLAG                                ((uint32_t) 1 << 4) //For Slave
#define I2C_REG_SR1_BTF_FLAG                                           ((uint32_t) 1 << 2)
#define I2C_REG_SR1_ADDR_FLAG                                          ((uint32_t) 1 << 1)
#define I2C_REG_SR1_ADDR_SENT_FLAG                                     ((uint32_t) 1 << 1) // For master
#define I2C_REG_SR1_ADDR_MATCHED_FLAG                                  ((uint32_t) 1 << 1) // For slave
#define I2C_REG_SR1_SB_FLAG                                            ((uint32_t) 1 << 0)



/***********************************Bit Definition for I2C_SR2 Register*********************************************************/

/* Bus busy*/
#define I2C_REG_SR2_BUS_BUSY_FLAG                                      ((uint32_t) 1 << 1)
#define I2C_BUS_IS_BUSY                                                1
#define I2C_BUS_IS_FREE                                                0

/* Master/slave */
#define I2C_REG_SR2_MSL_FLAG                                           ((uint32_t) 1 << 0)
#define I2C_MASTER_MODE                                                1
#define I2C_SLAVE_MODE                                                 0

/* Transmitter/receiver */
#define I2C_REG_SR2_TRA_FLAG                                           ((uint32_t) 1 << 2)
#define I2C_RX_MODE                                                    0
#define I2C_TX_MODE                                                    1


/***********************************Bit Definition for I2C_CCR Register*********************************************************/

/* I2C master mode selection*/
#define I2C_REG_CCR_ENABLE_FM                                          ((uint32_t) 1 << 15)
#define I2C_ENABLE_SM                                                  0
#define I2C_ENABLE_FM                                                  1

/* Fm mode duty cycle */
#define I2C_REG_CCR_DUTY                                               ((uint32_t) 1 << 14)
#define I2C_FM_DUTY_16BY9                                              1
#define I2C_FM_DUTY_2                                                  0

/***********************************I2C Peripheral Base addresses****************************************************************/

#define I2C_1                                                          I2C1
#define I2C_2                                                          I2C2
#define I2C_3                                                          I2C3


/*********************************Macros to enable the clock for different I2C Peripheral****************************************/

#define _HAL_RCC_I2C1_CLK_ENABLE()                                    (RCC->APB1ENR |= (1 << 21))
#define _HAL_RCC_I2C2_CLK_ENABLE()                                    (RCC->APB1ENR |= (1 << 22))
#define _HAL_RCC_I2C3_CLK_ENABLE()                                    (RCC->APB1ENR |= (1 << 23))



#define RESET                                                         0
#define SET                                                           !RESET

/*********************************************************************************************************************************/
/*                                                                                                                               */
/*                                          2. Data structure used by I2C Driver                                                 */
/*                                                                                                                               */
/*********************************************************************************************************************************/

/**
  * @brief HAL I2C state structre definition 
	*/
typedef enum
{
	HAL_I2C_STATE_RESET        = 0x00,    /* I2C is not yet initialized or disabled     */
	HAL_I2C_STATE_READY        = 0x01,    /* I2C initialized and ready to use           */
	HAL_I2C_STATE_BUSY         = 0x02,    /* I2C internal process is on going           */
	HAL_I2C_STATE_BUSY_TX      = 0x03,    /* I2C data transmission process is on going  */
	HAL_I2C_STATE_BUSY_RX      = 0x04,    /* I2C data reception process is on going     */
	HAL_I2C_STATE_ERROR        = 0x05     /* I2C Error state                            */
} hal_i2c_state_t;



/**
  * @brief I2C Error Code
	*/
#define HAL_I2C_ERROR_NONE             ((uint32_t) 0x00000000)     // No error    
#define HAL_I2C_ERROR_BERR             ((uint32_t) 0x00000001)     // Bus Error 
#define HAL_I2C_ERROR_ARLO             ((uint32_t) 0x00000002)     // Arbitration lost (master mode) Error
#define HAL_I2C_ERROR_AF               ((uint32_t) 0x00000004)     // Acknowledge failure error
#define HAL_I2C_ERROR_OVR              ((uint32_t) 0x00000008)     // Overrun/Underrun error
#define HAL_I2C_ERROR_DMA              ((uint32_t) 0x00000010)     // DMA transfer error
#define HAL_I2C_ERROR_TIMEOUT          ((uint32_t) 0x00000020)     // Timeout or Tlow error



/**
  * @brief I2C Configuration structure definition 
	*/
typedef struct
{
	uint32_t ClockSpeed;               /* Specifies the clock frequency */
	uint32_t DutyCycle;                /* Specifies the I2C fast mode duty cycle*/
  uint32_t OwnAddress1;              /* Specifies the 1st device own address */
  uint32_t AddressingMode;           /* Specifies if the 7-bit / 10-bit addressing mode is selected */
  uint32_t DualAddressMode;          /* Specifies if the dual addressing is slected */
  uint32_t OwnAddress2;              /* Specifies the 2nd device own address if the dual addressing is selected*/
  uint32_t GeneralCallMode;          /* Specifies if general call mode is selected*/
  uint32_t NoStretchMode;            /* Specifies if the no stretch mode is selected*/
  uint32_t Ack_Enable;               /* Handles ack enable/disable */	
}i2c_init_t;



/**
  * @brief I2C Handle structure definition 
	*/
typedef struct
{
	I2C_TypeDef         *Instance;     /* I2C Register base address */
	i2c_init_t          Init;          /* I2C Communication parameter */
	uint8_t             *pBuffPtr;     /* Pointer to I2C transfer buffer */
	uint32_t            XferSize;      /* I2C transfer size */
  uint32_t            XferCount;     /* I2C transfer count */
	hal_i2c_state_t     State;         /* I2C communication state */
	uint32_t            ErrorCode;     /* Used to hold error code status */
} i2c_handle_t;




/*********************************************************************************************************************************/
/*                                                                                                                               */
/*                                                   3. Driver Exposed APIs                                                      */
/*                                                                                                                               */
/*********************************************************************************************************************************/


/**
  * @brief  Initializes the Given I2C peripherl
  * @param  *handle : Handle to i2c peripheral, which the application wants to initialize.
  * @retval  none
 */
void hal_i2c_init(i2c_handle_t *handle);


/**
  * @brief API to do master data transmission
  * @param *handle: pointer to handle structure of I2C peripheral
  * @param slave_address: address to whuch we want to TX.
  * @param *buffer: hold the pointer to tx buffer 
	* @param len: length of the data to be transmitted
  * @retval none
 */
 void hal_i2c_master_tx(i2c_handle_t *handle, uint8_t slave_address, uint8_t *buffer, uint32_t len);
 
 
 /**
  * @brief API to do master data reception
  * @param *handle: pointer to handle structure of I2C peripheral
  * @param slave_addr: slave address who sends data
  * @param *buffer: hold the pointer to Rx buffer 
	* @param len: length of the data to be received
  * @retval none
 */
 void hal_i2c_master_rx(i2c_handle_t *handle, uint8_t slave_addr, uint8_t *buffer, uint32_t len);


 /**
  * @brief API to do slave data transmission
  * @param *handle: pointer to handle structure of I2C peripheral
  * @param *buffer: hold the pointer to tx buffer 
	* @param len: length of the data to be transmitted
  * @retval none
 */
void hal_i2c_slave_tx(i2c_handle_t *handle, uint8_t *buffer, uint32_t len);


 /**
  * @brief API to do slave data reception
  * @param *handle: pointer to handle structure of I2C peripheral
  * @param *buffer: hold the pointer to rx buffer 
	* @param len: length of the data to be received 
  * @retval none
 */
void hal_i2c_slave_rx(i2c_handle_t *handle, uint8_t *buffer, uint32_t len);


 /**
  * @brief This function handles I2C error interrupt request
  * @param hi2c: pointer to i2c_handle_t structure which contains I2C configuration information of I2C module.
  * @retval none
 */
 void hal_i2c_handle_error_interrupt(i2c_handle_t *hi2c);
 
 
  /**
  * @brief This function handles I2C event interrupt request
  * @param hi2c: pointer to i2c_handle_t structure which contains I2C configuration information of I2C module.
  * @retval none
 */
 void hal_i2c_handle_evt_interrupt(i2c_handle_t *hi2c);

#endif
