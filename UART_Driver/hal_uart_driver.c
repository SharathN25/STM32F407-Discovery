/***************************************************************************************************************************
  * @file    hal_uart_driver.c
  * @author  Sharath N
  * @brief   I2C HAL module driver,
             This file provides firmware functions to manage the functionality of UART in STM32F407 Discovery Board.
***************************************************************************************************************************/

#include <stdint.h>
#include "hal_uart_driver.h"
#include "led.h"

/***************************************************************************************************************************/
/*                                                                                                                         */
/*                                               Helper functions                                                          */
/*                                                                                                                         */
/***************************************************************************************************************************/

/**
  * @brief  Enable the given UART peripheral
  * @param  *uartx : Base address of UART or USART peripheral 
  * @retval  none
 */
static void hal_uart_enable(USART_TypeDef *uartx)
{
	uartx->CR1 |= USART_REG_CR1_USART_EN;
}



/**
  * @brief  disable the given UART peripheral
  * @param  *uartx : Base address of UART or USART peripheral
  * @retval  none
 */
static void hal_uart_disable(USART_TypeDef *uartx)
{
	uartx->CR1 &= ~USART_REG_CR1_USART_EN;
}



/**
  * @brief  Enable/Disable the transmitter block of UART peripheral
  * @param  *uartx : Base address of UART or USART peripheral
  * @param  TE: if TE = 1, then enable the trasnmit block
  * @retval  none
 */
static void hal_uart_enable_disable_tx(USART_TypeDef *uartx, uint32_t TE)
{
	if(TE & USART_REG_CR1_TE)
	{
		uartx->CR1 |= USART_REG_CR1_TE;
	}
	else
	{
		uartx->CR1 &= ~USART_REG_CR1_TE;
	}
}



/**
  * @brief  Enable/Disable the receiver block of UART peripheral
  * @param  *uartx : Base address of UART or USART peripheral
  * @param  TE: if TE = 1, then enable the trasnmit block
  * @retval  none
 */
static void hal_uart_enable_disable_rx(USART_TypeDef *uartx, uint32_t RE)
{
	if(RE & USART_REG_CR1_RE)
	{
		uartx->CR1 |= USART_REG_CR1_RE;
	}
	else
	{
		uartx->CR1 &= ~USART_REG_CR1_RE;
	}
}




/**
  * @brief  Configures the word length for data transmission and reception
  * @param  *uartx : Base address of UART or USART peripheral
  * @param   WL : if WL = 1, then word length = 1s,9bits,nstop.
                     WL = 1, then word length = 1s,8bits,nstop.
  * @retval  none   
 */
static void hal_uart_configure_word_length(USART_TypeDef *uartx, uint32_t WL)
{
	if(WL)
	{
		uartx->CR1 |= USART_REG_CR1_USART_WL; //9 data bits
	}
	else
	{
		uartx->CR1 &= ~USART_REG_CR1_USART_WL; //8 data bits
	}
}




/**
  * @brief  Configures the number of stop bits
  * @param  *uartx : Base address of UART or USART peripheral
  * @param   nstop: this value configures the stop bit
  * @retval  none   
 */
static void hal_uart_configure_stop_bits(USART_TypeDef *uartx, uint32_t nstop)
{
	//Clear the 12th and 13th bit of USART_CR2
	uartx->CR2 &= ~(0x03 << USART_REG_CR2_STOP_BITS);
	
	if(nstop == UART_STOPBIT_HALF)
	{
		uartx->CR2 |= (0x01 <<USART_REG_CR2_STOP_BITS);//0.5bits
	}
	else if(nstop == UART_STOPBIT_2)
	{
		uartx->CR2 |= (0x02 <<USART_REG_CR2_STOP_BITS);//2bits
	}
	else if(nstop == UART_STOPBIT_ONEHALF)
	{
		uartx->CR2 |= (0x03 <<USART_REG_CR2_STOP_BITS);//1.5bits
	}
	else
	{
		uartx->CR2 |= (0x00 <<USART_REG_CR2_STOP_BITS);//1bits
	}
}




/**
  * @brief  Configures the baud rate
  * @param  *uartx : Base address of UART or USART peripheral
  * @param   baud : baudrate value to be programmed
  * @retval  none   
 */
static void hal_uart_set_baud_rate(USART_TypeDef *uartx, uint32_t baud)
{
	uint32_t val;
	
	if(baud == USART_BAUD_RATE_9600)
	{
		val = 0x683;
	}
	else if(baud == USART_BAUD_RATE_115200)
	{
		val = 0x8A;
	}
	else
	{
		val = 0x8A;
	}
	
	uartx->BRR = val;
}




/**
  * @brief  Configures the oversampling rate of the UART peripheral 
  * @param  *uartx : Base address of UART or USART peripheral
  * @param   over8 : if over8 = 1, oversampling by8 is used, otherwise oversampling by 16 is used
  * @retval  none   
 */
static void hal_uart_configure_over_sampling(USART_TypeDef *uartx, uint32_t over8)
{
	if(over8)
	{
		uartx->CR1 |= USART_REG_CR1_OVER8;
	}
}




/**
  * @brief  Enable/Disable TXE interrupt
  * @param  *uartx : Base address of UART or USART peripheral
  * @param   txe_en: if txe_en = 1, the enable the TXE interrupt
  * @retval  none   
 */
static void hal_uart_configure_txe_interrup(USART_TypeDef *uartx, uint32_t txe_en)
{
	if(txe_en)
	{
		uartx->CR1 |= USART_REG_CR1_TXE_INT_ENABLE;
	}
	else
	{
		uartx->CR1 &= ~USART_REG_CR1_TXE_INT_ENABLE;
	}
}




/**
  * @brief  Enable/Disable RXNE interrupt
  * @param  *uartx : Base address of UART or USART peripheral
  * @param   txe_en: if rxne_en = 1, the enable the RXNE interrupt
  * @retval  none   
 */
static void hal_uart_configure_rxne_interrup(USART_TypeDef *uartx, uint32_t rxne_en)
{
	if(rxne_en)
	{
		uartx->CR1 |= USART_REG_CR1_RXNE_INT_ENABLE;
	}
	else
	{
		uartx->CR1 &= ~USART_REG_CR1_RXNE_INT_ENABLE;
	}
}



/**
  * @brief  Enable/Disable Error interrupt
  * @param  *uartx : Base address of UART or USART peripheral
  * @param   err_en: if err_en = 1, the enable the Error interrupt
  * @retval  none   
 */
static void hal_uart_configure_error_interrup(USART_TypeDef *uartx, uint32_t err_en)
{
	if(err_en)
	{
		uartx->CR3 |= USART_REG_CR3_ERR_INT_ENABLE;
	}
	else
	{
		uartx->CR3 &= ~USART_REG_CR3_ERR_INT_ENABLE;
	}
}




/**
  * @brief  Enable/Disable Parity Error interrupt
  * @param  *uartx : Base address of UART or USART peripheral
  * @param   pe_en: if pe_en = 1, the enable the Error interrupt
  * @retval  none   
 */
static void hal_uart_configure_parity_error_interrup(USART_TypeDef *uartx, uint32_t pe_en)
{
	if(pe_en)
	{
		uartx->CR1 |= USART_REG_CR1_PEIE_INT_ENABLE;
	}
	else
	{
		uartx->CR1 &= ~USART_REG_CR1_PEIE_INT_ENABLE;
	}
}




/**
  * @brief  UART Error callback
  * @param  *huart: pointer to uart_handle_t structure that contains the configuration for the specified UART module
  * @retval  none   
 */
static void hal_uart_error_cb(uart_handle_t *huart)
{
	while(1)
	{
		led_turn_on(GPIOD, LED_RED);
	}
}




/**
  * @brief  Handle the TXE interrupt
  * @param  *huart: pointer to uart_handle_t structure that contains the configuration for the specified UART module
  * @retval  none   
 */
static void hal_uart_handle_TXE_interrupt(uart_handle_t *huart)
{
	uint32_t temp = 0;
	uint8_t val;
	
	temp = huart->tx_state;
	if(temp == HAL_UART_STATE_BUSY_TX)
	{
		val = (uint8_t)(*huart->pTxBufferPtr++ & (uint32_t)0x00FF);
		huart->Instance->DR = val;
		
		if(--huart->TxXferCount == 0)
		{
			/*Disable the UART TXE interrupt*/
			huart->Instance->CR1 &= ~USART_REG_CR1_TXE_INT_ENABLE;
			
			/*Enable the UART Transmit complete interrupt */
			huart->Instance->CR1 |= USART_REG_CR1_TCIE_INT_ENABLE;
		}

	}
	
}




/**
  * @brief  Handle the RXNE interrupt
  * @param  *huart: pointer to uart_handle_t structure that contains the configuration for the specified UART module
  * @retval  none   
 */
static void hal_uart_handle_RXNE_interrupt(uart_handle_t *huart)
{
	uint32_t temp = 0;
	uint8_t val;
	
	temp = huart->rx_state;
	if(temp == HAL_UART_STATE_BUSY_RX)
	{
		/*If application is using parity? */
		if(huart->Init.Parity == UART_PARITY_NONE)
		{
			//No Parity
			*huart->pRxBufferPtr++ = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);
		}
		else
		{
			//Parity =YES, dont read MSB its a parity bit
			*huart->pRxBufferPtr++ = (uint8_t)(huart->Instance->DR & (uint8_t)0x007F);
		}
		
		
		if(--huart->RxXferCount == 0)
		{
			/*Disable RXNE Interrupt*/
			huart->Instance->CR1 &= ~USART_REG_CR1_RXNE_INT_ENABLE;
			
			/*Disable Parity Error interrupt */
			huart->Instance->CR1 &= ~USART_REG_CR1_PEIE_INT_ENABLE;
			
			/*Disable the UART error interrupt */
			huart->Instance->CR3 &= ~USART_REG_CR3_ERR_INT_ENABLE;
			
			/*make state ready for this handle */
			huart->rx_state = HAL_UART_STATE_READY;
			
			/*Call the call back funtion */
	      if(huart->rx_comp_cb)
		      huart->rx_comp_cb(&huart->RxXferSize);
			}
		}
}




/**
  * @brief  Handle the TC interrupt
  * @param  *huart: pointer to uart_handle_t structure that contains the configuration for the specified UART module
  * @retval  none   
 */
static void hal_uart_handle_TC_interrupt(uart_handle_t *huart)
{
	/*Disable the uart TC interrupt */
	huart->Instance->CR1 &= ~ USART_REG_CR1_TCIE_INT_ENABLE;
	huart->tx_state =  HAL_UART_STATE_READY;
	
	/* Call the application call back */
	if(huart->tx_comp_cb)
		huart->tx_comp_cb(&huart->TxXferSize);
	
}






/**
  * @brief  Clart Error flag
  * @param  *huart: pointer to uart_handle_t structure that contains the configuration for the specified UART module
  * @retval  none   
 */
static void hal_uart_clear_error_flag(uart_handle_t *huart)
{
	/* It is cleared by a software sequence (an read to the USART_SR register followed by a read to the USART_DR register).*/
	uint32_t temp = 0x00;
	temp = huart->Instance->SR;
	temp = huart->Instance->DR;
}




/***********************************************************************************************************************************/
/*                                                                                                                                 */
/*                                               Driver Exposed APIs                                                               */
/*                                                                                                                                 */
/***********************************************************************************************************************************/

/**
  * @brief  Initializes the Given UART peripherl
  * @param  *uart_handle : pointer to handle structure of UART peripheral
  * @retval  none
 */
void hal_uart_init(uart_handle_t *uart_handle)
{
	/*Configure the word length */
	hal_uart_configure_word_length(uart_handle->Instance, uart_handle->Init.WordLength);
	
	/*Configure the number of stop bit */
	hal_uart_configure_stop_bits(uart_handle->Instance, uart_handle->Init.StopBits);
	
	/*Configure the oversampling rate for receiver block */
	hal_uart_configure_over_sampling(uart_handle->Instance, uart_handle->Init.OverSampling);
	
	/*Set the baud rate */
	hal_uart_set_baud_rate(uart_handle->Instance, uart_handle->Init.BaudRate);
	
	/*Enable the transmit block of the UART peripheral */
	hal_uart_enable_disable_rx(uart_handle->Instance, uart_handle->Init.Mode);
	
	/*Enable the receive block of the UART peripheral */
	hal_uart_enable_disable_rx(uart_handle->Instance, uart_handle->Init.Mode);
	
	/*Enable the UART peripheral */
	hal_uart_enable(uart_handle->Instance);
	
	uart_handle->rx_state = HAL_UART_STATE_READY;
	uart_handle->tx_state = HAL_UART_STATE_READY;
	uart_handle->ErrorCode = HAL_UART_ERROR_NONE;
	
	
}




/**
  * @brief API to do UART data transmission
  * @param *uart_handle: pointer to handle structure of UART peripheral
  * @param *buffer: hold the pointer to tx buffer 
	* @param len: length of the data to be transmitted
  * @retval none
 */
void hal_hal_uart_tx(uart_handle_t *uart_handle, uint8_t *buffer, uint32_t len)
{
	/* Populate the application given information into the UART handle structure */
	uart_handle->pTxBufferPtr = buffer;
	uart_handle->TxXferCount = len;
	uart_handle->TxXferSize = len;
	
	/*This handle is busy in transmission*/
	uart_handle->tx_state = HAL_UART_STATE_BUSY_TX;
	
	/*Enable the UART peripheral*/
	hal_uart_enable(uart_handle->Instance);
	
	/* Enable the TXE interrupt */
	hal_uart_configure_txe_interrup(uart_handle->Instance, 1);
	
}



/**
  * @brief API to do UART data reception
  * @param *uart_handle: pointer to handle structure of UART peripheral
  * @param *buffer: hold the pointer to rx buffer 
	* @param len: length of the data to be received 
  * @retval none
 */
void hal_hal_uart_rx(uart_handle_t *uart_handle, uint8_t *buffer, uint32_t len)
{
	uint32_t val;
	/* Populate the application given information into the UART handle structure */
	uart_handle->pRxBufferPtr = buffer;
	uart_handle->RxXferCount = len;
	uart_handle->RxXferSize = len;
	
	/*This handle is busy in transmission*/
	uart_handle->tx_state = HAL_UART_STATE_BUSY_RX;
	
	/*Enable the UART Parity interrupt error */
	hal_uart_configure_parity_error_interrup(uart_handle->Instance,1);
	
	/*Enable the Error interrupt */
	hal_uart_configure_error_interrup(uart_handle->Instance, 1);
  
	val = uart_handle->Instance->DR;
	
	/*Enable RXNE interrupt */
	hal_uart_configure_rxne_interrup(uart_handle->Instance, 1);
	
}




/**
  * @brief This API handles the UART interrupt request
  * @param *huart: pointer to uart_handle_t structure that contains the configuration for the specified UART module.
  * @retval none
 */
void hal_uart_handle_interrupt(uart_handle_t *huart)
{
	uint32_t temp1 = 0, temp2 = 0;
	/* Parity Error Check */
	temp1 = (huart->Instance->SR & USART_REG_SR_PE_FLAG); // Check if Parity error occured
	temp2 = (huart->Instance->CR1 & USART_REG_CR1_PEIE_INT_ENABLE); // Check if Parity error interrupt is enabled.
	/* UART Parity error occured ------------------------------------------------------------------------------ */
	if(temp1 && temp2 )
	{
		/*Clear the erro Flag */
		hal_uart_clear_error_flag(huart);
		
		huart->ErrorCode = HAL_UART_ERROR_PE;
	}
	
	
	/*Frame Error Check */
	temp1 = (huart->Instance->SR & USART_REG_SR_FE_FLAG); // Check if frame error occured
	temp2 = (huart->Instance->CR3 & USART_REG_CR3_ERR_INT_ENABLE); // Check if error interrupt is enabled.
	/* UART Frame  error occured ------------------------------------------------------------------------------ */
	if(temp1 && temp2 )
	{
		/*Clear the error Flag */		
		hal_uart_clear_error_flag(huart);
		
		huart->ErrorCode = HAL_UART_ERROR_FE;
		
	}
	
	
	/* Overrun  error check */
	temp1 = (huart->Instance->SR & USART_REG_SR_ORE_FLAG); // Check if overun error occured
	temp2 = (huart->Instance->CR3 & USART_REG_CR3_ERR_INT_ENABLE); // Check if error interrupt is enabled.
	/* UART overrun  error occured ------------------------------------------------------------------------------ */
	if(temp1 && temp2 )
	{
		/*Clear the error Flag */		
		hal_uart_clear_error_flag(huart);
		
		huart->ErrorCode = HAL_UART_ERROR_ORE;
		
	}

  
  /* Noise Error Check */
  temp1 = (huart->Instance->SR & USART_REG_SR_NE_FLAG); // Check if noise error occured
	temp2 = (huart->Instance->CR3 & USART_REG_CR3_ERR_INT_ENABLE); // Check if error interrupt is enabled.
	/* UART noise  error occured ------------------------------------------------------------------------------ */
	if(temp1 && temp2 )
	{
		/*Clear the error Flag */		
		hal_uart_clear_error_flag(huart);
		
		huart->ErrorCode = HAL_UART_ERROR_NE;
		
	}	
	
	
	/* Check For RXNE Flag */
	 temp1 = (huart->Instance->SR & USART_REG_SR_RXNE_FLAG); // Check if RXNE Flag set
	temp2 = (huart->Instance->CR3 & USART_REG_CR1_RXNE_INT_ENABLE); // Check if RXNE interrupt is enabled.
	/* UART is in Receiver Mode ------------------------------------------------------------------------------ */
	if(temp1 && temp2 )
	{

		hal_uart_handle_RXNE_interrupt(huart);
		
	}	
	
	
	
	/* Check For TXE Flag */
	temp1 = (huart->Instance->SR & USART_REG_SR_TXE_FLAG); // Check if TXE Flag set
	temp2 = (huart->Instance->CR3 & USART_REG_CR1_TXE_INT_ENABLE); // Check if TXE interrupt is enabled.
	/* UART is in transmitter Mode ------------------------------------------------------------------------------ */
	if(temp1 && temp2 )
	{

		hal_uart_handle_TXE_interrupt(huart);
		
	}
	
	
	
	/* Check For TC Flag */
	temp1 = (huart->Instance->SR & USART_REG_SR_TC_FLAG); // Check if TC Flag set
	temp2 = (huart->Instance->CR3 & USART_REG_CR1_TCIE_INT_ENABLE); // Check if TCIE interrupt is enabled.
	/* UART Trasnmit Complete ------------------------------------------------------------------------------ */
	if(temp1 && temp2 )
	{

		hal_uart_handle_TC_interrupt(huart);
		
	}
	
	
	/* If there is a  Error */
	if(huart->ErrorCode != HAL_UART_ERROR_NONE)
	{
		/* Set the UART state ready*/
		huart->rx_state = HAL_UART_STATE_READY;
		huart->tx_state = HAL_UART_STATE_READY;
		
		/*Call the error handler */
		hal_uart_error_cb(huart);
	}

}

