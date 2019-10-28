/**************************************************************************************************************************
 * @file     hal_uart_driver.h
 * @author   Sharath N 
 * @brief    Header file for UART Driver of STM32F407 Discovery Baord.
 **************************************************************************************************************************/
 
#ifndef __HAL_UART_DRIVER_H
#define __HAL_UART_DRIVER_H

/*MCU specific header file for stm32f407vgt6 base discovery board */
#include "stm32f407xx.h"
#include <stdint.h>

/**
  *@brief HAL UART State structures definition
	*/
typedef enum
{
	HAL_UART_STATE_RESET        = 0x00,    /* Peripheral is not yet initialized or disabled    */
	HAL_UART_STATE_READY        = 0x01,    /* Peripheral initialized and ready to use          */
	HAL_UART_STATE_BUSY         = 0x02,    /* An internal process is on going                  */
	HAL_UART_STATE_BUSY_TX      = 0x12,    /* Data transmission process is on going            */
	HAL_UART_STATE_BUSY_RX      = 0x22,    /* Data reception process is on going               */
	HAL_UART_STATE_BUSY_TX_RX   = 0x32     /* Data Transmission and reception process going on */
} hal_uart_state_t ;



/**
  *@brief UART Possible Error Codes
	*/
#define HAL_UART_ERROR_NONE             ((uint32_t) 0x00000000)     // No error    
#define HAL_UART_ERROR_PE               ((uint32_t) 0x00000001)     // Parity error 
#define HAL_UART_ERROR_NE               ((uint32_t) 0x00000002)     // Noise Error
#define HAL_UART_ERROR_FE               ((uint32_t) 0x00000004)     // Frame error
#define HAL_UART_ERROR_ORE              ((uint32_t) 0x00000008)     // Overrun error
#define HAL_UART_ERROR_DMA              ((uint32_t) 0x00000010)     // DMA transfer error


/***********************************USART and UART Peripheral Base addresses****************************************************/

#define USART_1                                                       USART1
#define USART_2                                                       USART2
#define USART_3                                                       USART3
#define UART_4                                                        UART4
#define UART_5                                                        UART6
#define USART_6                                                       USART6

/*********************************Macros to enable the clock for different I2C Peripheral****************************************/

#define _HAL_RCC_USART1_CLK_ENABLE()                                  (RCC->APB2ENR |= (1 << 4))
#define _HAL_RCC_USART2_CLK_ENABLE()                                  (RCC->APB1ENR |= (1 << 17))
#define _HAL_RCC_USART3_CLK_ENABLE()                                  (RCC->APB1ENR |= (1 << 18))
#define _HAL_RCC_UART4_CLK_ENABLE()                                   (RCC->APB1ENR |= (1 << 19))
#define _HAL_RCC_UART5_CLK_ENABLE()                                   (RCC->APB1ENR |= (1 << 20))
#define _HAL_RCC_USART6_CLK_ENABLE()                                  (RCC->APB2ENR |= (1 << 5))

/*******************************************************************************************************************************/
/*                                                                                                                             */
/*                                             USART Register bit definition                                                    */
/*                                                                                                                             */
/*******************************************************************************************************************************/


/***********************************Bit Definition for USART_SR Register********************************************************/

#define USART_REG_SR_TXE_FLAG                                           ((uint32_t) 1 << 7)
#define USART_REG_SR_TC_FLAG                                            ((uint32_t) 1 << 6)
#define USART_REG_SR_RXNE_FLAG                                          ((uint32_t) 1 << 5)
#define USART_REG_SR_IDLE_FLAG                                          ((uint32_t) 1 << 4)
#define USART_REG_SR_ORE_FLAG                                           ((uint32_t) 1 << 3)
#define USART_REG_SR_NE_FLAG                                            ((uint32_t) 1 << 2)
#define USART_REG_SR_FE_FLAG                                            ((uint32_t) 1 << 1)
#define USART_REG_SR_PE_FLAG                                            ((uint32_t) 1 << 0)

/***********************************Bit Definition for USART_BRR Register********************************************************/

#define USART_REG_BRR_MANTISSA                                          ((uint32_t) 1 << 4)
#define USART_REG_BRR_FRACTION                                          ((uint32_t) 1 << 0)

/***********************************Bit Definition for USART_CR1 Register********************************************************/

/* Oversampling mode */
#define USART_REG_CR1_OVER8                                            ((uint32_t) 1 << 15)
#define USART_OVER8_ENABLE                                             1
#define USART_OVER16_ENABLE                                            0

/*USART enable*/
#define USART_REG_CR1_USART_EN                                         ((uint32_t) 1 << 13)  

/* Word length */
#define USART_REG_CR1_USART_WL                                         ((uint32_t) 1 << 12)
#define USART_WL_1S8B                                                  0
#define USART_WL_1S9B                                                  1

/* TXE, TCIE ,PEIE and RXNE Interrupt enable */
#define USART_REG_CR1_PEIE_INT_ENABLE                                  ((uint32_t) 1 << 8)
#define USART_REG_CR1_TXE_INT_ENABLE                                   ((uint32_t) 1 << 7)
#define USART_REG_CR1_TCIE_INT_ENABLE                                  ((uint32_t) 1 << 6)
#define USART_REG_CR1_RXNE_INT_ENABLE                                  ((uint32_t) 1 << 5)

/* Transmitter and Receiver enable */
#define USART_REG_CR1_TE                                               ((uint32_t) 1 << 3)
#define USART_REG_CR1_RE                                               ((uint32_t) 1 << 2)

/***********************************Bit Definition for USART_CR3 Register********************************************************/

/* Error interrupt enable */
#define USART_REG_CR3_ERR_INT_ENABLE                                   ((uint32_t) 1 << 0)

/***********************************Bit Definition for USART_CR2 Register********************************************************/

#define USART_REG_CR2_STOP_BITS                                       ((uint32_t) 12)
#define UART_STOPBIT_1                                                ((uint32_t) 0x00)
#define UART_STOPBIT_HALF                                             ((uint32_t) 0x01)
#define UART_STOPBIT_2                                                ((uint32_t) 0x02)
#define UART_STOPBIT_ONEHALF                                          ((uint32_t) 0x03)

/********************************************************************************************************************************/
#define UART_PARITY_NONE                                              ((uint32_t) 0x00000000)
#define UART_HWCONTROL_NONE                                           ((uint32_t) 0x00000000)

#define UART_MODE_TX_RX                                               ((uint32_t) (USART_REG_CR1_TE | USART_REG_CR1_RE))
#define UART_MODE_TX                                                  ((uint32_t) USART_REG_CR1_TE )

#define USART_BAUD_RATE_9600                                          ((uint32_t) 9600)
#define USART_BAUD_RATE_115200                                        ((uint32_t) 115200)
#define USART_BAUD_RATE_2000000                                       ((uint32_t) 2000000)


/*********************************************************************************************************************************/
/*                                                                                                                               */
/*                                            Data structure used by UART Driver                                                 */
/*                                                                                                                               */
/*********************************************************************************************************************************/

/**
  *@brief UART Init Structure definition
	*/
	typedef struct
{
	uint32_t BaudRate;        /* Configures the UART Communication Baud Rate */
	uint32_t WordLength;      /* Specifies the number bits transmitted or received in a frame */
	uint32_t StopBits;        /* Specifies the number of stop bits transmitted */
	uint32_t Parity;          /* Specifies parity mode */
	uint32_t Mode;            /* Specifies whether recive/transmit mode is enabled or disabled */
	uint32_t OverSampling;    /* Specifies whether oversampling8 is enabled or disabled*/
} uart_init_t ;



/*Application callback typedef */
typedef void(TX_COMP_CB_t) (void *ptr);
typedef void(RX_COMP_CB_t) (void *ptr);


/**
  *@brief UART handle structure definition
  */
typedef struct
{
	USART_TypeDef          *Instance;        /* UART Register Base Address */
	uart_init_t            Init;             /* UART Communication parameter */
	uint8_t                *pTxBufferPtr;    /* Pointer to UART Tx Transmit buffer */
	uint16_t               TxXferSize;       /* UART Tx Transfer Size */
	uint16_t               TxXferCount;      /* UART Tx Transfer Count */
  uint8_t                *pRxBufferPtr;    /* Pointer to UART Rx Transmit buffer */
	uint16_t               RxXferSize;       /* UART Rx Transfer Size */
	uint16_t               RxXferCount;      /* UART Rx Transfer Count */
	hal_uart_state_t       rx_state;         /* UART Communication state */
	hal_uart_state_t       tx_state;         /* UART Communication state */   
	uint32_t               ErrorCode;        /* UART Error code */
	TX_COMP_CB_t           *tx_comp_cb;      /* Application call back when tx is completed */
	RX_COMP_CB_t           *rx_comp_cb;      /* Application call back when rx is completed */
} uart_handle_t;
	





/*********************************************************************************************************************************/
/*                                                                                                                               */
/*                                                   Driver Exposed APIs                                                         */
/*                                                                                                                               */
/*********************************************************************************************************************************/

/**
  * @brief  Initializes the Given UART peripherl
  * @param  *uart_handle : pointer to handle structure of UART peripheral
  * @retval  none
 */
void hal_uart_init(uart_handle_t *uart_handle);


/**
  * @brief API to do UART data transmission
  * @param *uart_handle: pointer to handle structure of UART peripheral
  * @param *buffer: hold the pointer to tx buffer 
	* @param len: length of the data to be transmitted
  * @retval none
 */
void hal_hal_uart_tx(uart_handle_t *uart_handle, uint8_t *buffer, uint32_t len);


/**
  * @brief API to do UART data reception
  * @param *uart_handle: pointer to handle structure of UART peripheral
  * @param *buffer: hold the pointer to rx buffer 
	* @param len: length of the data to be received 
  * @retval none
 */
void hal_hal_uart_rx(uart_handle_t *uart_handle, uint8_t *buffer, uint32_t len);



/**
  * @brief This API handles the UART interrupt request
  * @param *huart: pointer to uart_handle_t structure that contains the configuration for the specified UART module.
  * @retval none
 */
void hal_uart_handle_interrupt(uart_handle_t *huart);

#endif
