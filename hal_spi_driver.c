/***************************************************************************************************************************
  * @file    hal_spi_driver.c
  * @author  Sharath N
  * @brief   SPI HAL module driver,
             This file provides firmware functions to manage the functionality of SPI in STM32F407 Discovery Board.
***************************************************************************************************************************/

#include <stdint.h>
#include "hal_spi_driver.h"

/*************************************************************************************************************************/
/*                                                                                                                       */
/*                                         Helper functions                                                              */
/*                                                                                                                       */
/*************************************************************************************************************************/

/**
  * @brief Enable the SPI device
  * @param *SPIx : Based address of SPI
  * @retval none
 */
static void hal_spi_enable(SPI_TypeDef *SPIx)
{
	if(!(SPIx->CR1 &= SPI_REG_CR1_SPE ))
	{
		SPIx->CR1 |= SPI_REG_CR1_SPE;
	}

}


/**
  * @brief Disable the SPI device
  * @param *SPIx : Based address of SPI
  * @retval none
 */
static void hal_spi_disable(SPI_TypeDef *SPIx)
{
  SPIx->CR1 &= ~SPI_REG_CR1_SPE;
}


/**
  * @brief Configure SPI Clock phase and polarity
  * @param *SPIx : Based address of SPI
  * @param phase : configures phase
  * @param polarity : configures polarity
  * @retval none
 */
static void hal_spi_configure_phase_and_polarity(SPI_TypeDef *SPIx, uint32_t phase_value, uint32_t polarity)
{
	
	if(phase_value)
	{
		SPIx->CR1 |= SPI_REG_CR1_CPHA ;
	}
	else
	{
		SPIx->CR1 &= ~SPI_REG_CR1_CPHA;
	}
	
	if(polarity)
	{
		SPIx->CR1 |= SPI_REG_CR1_CPOL;
	}
	else
	{
		SPIx->CR1 &= ~SPI_REG_CR1_CPOL;
	}
	
}



/**
  * @brief Configures master or slave mode
  * @param *SPIx : Based address of SPI
  * @param master : if 1, then configure for master
  * @retval none
 */
static void hal_spi_configure_device_mode(SPI_TypeDef *SPIx, uint32_t master)
{
	
	if(master)
	{
		SPIx->CR1 |= SPI_REG_CR1_MSTR;
	}
	else
	{
	  SPIx->CR1 &= ~SPI_REG_CR1_MSTR;
	}
	
}



/**
  * @brief Configures SPI Data size and dirction 
  * @param *SPIx : Based address of SPI
  * @param datasize : data size to be configured
  * @param lsbmsbfirst: if 1, lsb will be sent first.
  * @retval none
 */
static void hal_spi_configure_datasize(SPI_TypeDef *SPIx, uint32_t datasize_16, uint32_t lasbfirst)
{
	
	if(datasize_16)
	{
		SPIx->CR1 |= SPI_REG_CR1_DFF;
	}
	else
	{
		SPIx->CR1 &= ~SPI_REG_CR1_DFF;
	}
	
	if(lasbfirst)
	{
		SPIx->CR1 |= SPI_REG_CR1_LSBFIRST;
	}
	else
	{
		SPIx->CR1 &= ~SPI_REG_CR1_LSBFIRST;
	}
	
}



/**
  * @brief Configures NSS pin for master
  * @param *SPIx : Based address of SPI
  * @retval none
 */
static void hal_spi_configure_nss_master(SPI_TypeDef *SPIx, uint32_t ssm_enable)
{
	
	if(ssm_enable)
	{
		SPIx->CR1 |= SPI_REG_CR1_SSM;
		SPIx->CR1 |= SPI_REG_CR1_SSI;
	}
	else
	{
		SPIx->CR1 &= ~SPI_REG_CR1_SSM;
	}
	
}


/**
  * @brief Configures NSS pin of slave 
  * @param *SPIx : Based address of SPI
  * @retval none
 */
static void hal_spi_configure_nss_slave(SPI_TypeDef *SPIx, uint32_t ssm_enable)
{
	
	if(ssm_enable)
	{
		SPIx->CR1 |= SPI_REG_CR1_SSM;
	}
	else
	{
		SPIx->CR1 &= ~SPI_REG_CR1_SSM;
	}
	
}


/**
  * @brief Configures The Buad Rate of SPI
  * @param *SPIx : Based address of SPI
  * @param baud_rate : buad rate pre scalar value.
  * @retval none
 */
static void hal_spi_configure_buadrate(SPI_TypeDef *SPIx, uint32_t baud_rate)
{
	SPIx->CR1 |= baud_rate;
}



/**
  * @brief Configures the direction of  SPI device
  * @param *SPIx : Based address of SPI
  * @param spi_direction :  specify spi device direction
  * @retval none
 */
static void hal_spi_configure_device_direction(SPI_TypeDef *SPIx, uint32_t spi_direction)
{
	if(spi_direction)
	{ 
		//1-line bidirectional data mode selected
		SPIx->CR1 |= SPI_REG_CR1_BIDIMODE;
	}
	else
	{
		//2-line unidirectional data mode selected
		SPIx->CR1 &= ~SPI_REG_CR1_BIDIMODE;
	}
}



/**
  * @brief Enable TX buffer emty interrupt 
  * @param SPIx:  SPI base address 
  * @retval none
 */
static void hal_spi_enable_txe_interrupt(SPI_TypeDef *SPIx)
{
		SPIx->CR2 |= SPI_REG_CR2_TXEIE_ENABLE;
}

/**
  * @brief Disable  TX buffer emty interrupt 
  * @param SPIx:  SPI base address 
  * @retval none
 */
static void hal_spi_disable_txe_interrupt(SPI_TypeDef *SPIx)
{
	SPIx->CR2 &= ~SPI_REG_CR2_TXEIE_ENABLE;
}


/**
  * @brief Enable RX buffer not empty interrupt
  * @param SPIx:  SPI base address 
  * @retval none
 */
static void hal_spi_enable_rxne_interrupt(SPI_TypeDef *SPIx)
{
		SPIx->CR2 |= SPI_REG_CR2_RXNEIE_ENABLE;
	
}

/**
  * @brief Disable RX buffer not empty interrupt
  * @param SPIx:  SPI base address 
  * @retval none
 */
static void hal_spi_disable_rxne_interrupt(SPI_TypeDef *SPIx)
{
		SPIx->CR2 &= ~SPI_REG_CR2_RXNEIE_ENABLE;
	
}



/**
* @brief	 Checks if SPI bus is busy or not.
* @param	 SPIx: SPI base address
* @retval	 Returns uint8_t value : If bus is Busy returns 1
                                   if bus is free returns 0
*/
static uint8_t hal_spi_is_bus_busy(SPI_TypeDef *SPIx)
{

	if(SPIx->SR & SPI_REG_SR_BSY_FLAG)
		return 1; // SPI bus is  busy 
	else
		return 0;// SPI bus is free
}
	

/**
  * @brief Close TX transfer 
  * @param SPIx:  SPI base address 
  * @retval none
 */
static void hal_spi_close_tx_interrupt(spi_handle_t *hspi)
{
	/*Disable TXE interrupt*/
  hal_spi_disable_txe_interrupt(hspi->Instance);
	
	/*Make state ready only when we are in master mode and driver in not in Receiving data*/
	if(hspi->Init.Mode && (hspi->state != HAL_SPI_STATE_BUSY_RX))
		hspi->state = HAL_SPI_STATE_READY;
}


/**
  * @brief Close RX transfer 
  * @param SPIx:  SPI base address 
  * @retval none
 */
static void hal_spi_close_rx_interrupt(spi_handle_t *hspi)
{
 while(hal_spi_is_bus_busy(hspi->Instance));
	
	/*Disable RXNE interrupt*/
	hal_spi_disable_rxne_interrupt(hspi->Instance);
	hspi->state = HAL_SPI_STATE_READY;
}
		
	
/*******************************************************************************************************************************************/
/*                                                                                                                                         */
/*                                               Driver Exposed APIs                                                                       */
/*                                                                                                                                         */
/*******************************************************************************************************************************************/


/**
  * @brief API given to initialize then given SPI device
  * @param *SPIx : Based address of SPI
  * @param *buffer  : Pointer to RX buffer 
  * @param len : lenght of RX data 
  * @retval none
 */
void hal_spi_init(spi_handle_t *spi_handle)
{
	/* Configure phase and polarity */
	hal_spi_configure_phase_and_polarity(spi_handle->Instance, spi_handle->Init.CLKPhase,spi_handle->Init.CLKPolarity);
	
	/*Configure the SPI device mode */
	hal_spi_configure_device_mode(spi_handle->Instance,spi_handle->Init.Mode);
	
	/*configure the spi data size */
	hal_spi_configure_datasize(spi_handle->Instance,spi_handle->Init.DataSize,spi_handle->Init.FirstBit);
	
	/*configure the slave select line*/
	hal_spi_configure_nss_master(spi_handle->Instance, spi_handle->Init.NSS);
	hal_spi_configure_nss_slave(spi_handle->Instance, spi_handle->Init.NSS);
	
	/*configure spi device speed*/
	hal_spi_configure_buadrate(spi_handle->Instance,spi_handle->Init.BaudRatePreScalar);
	
	/*Configure spi device direction */
	hal_spi_configure_device_direction(spi_handle->Instance, spi_handle->Init.Direction);
	
}

	

/**
  * @brief API used to do master data transmission 
  * @param *SPIx : Based address of SPI
  * @param *buffer  : Pointer to tx buffer 
  * @param len : lenght of tx data 
  * @retval none
 */
void hal_spi_master_tx(spi_handle_t *spi_handle, uint8_t *buffer , uint32_t len)
{
	spi_handle->pTxBuffPtr = buffer;
	spi_handle->TxXferCount = len;
	spi_handle->TxXferSize = len;
	
	spi_handle->state = HAL_SPI_STATE_BUSY_TX;
	
	hal_spi_enable(spi_handle->Instance);
	
	hal_spi_enable_txe_interrupt(spi_handle->Instance);
}


/**
  * @brief API used to do master data reception
  * @param *SPIx : Based address of SPI
  * @param *buffer  : Pointer to tx buffer 
  * @param len : lenght of tx data 
  * @retval none
 */
void hal_spi_master_rx(spi_handle_t *spi_handle, uint8_t *rx_buffer, uint32_t len)
{
	uint32_t val;
	
	/*this is dummy tx*/
	spi_handle->pTxBuffPtr = rx_buffer;
	spi_handle->TxXferCount = len;
	spi_handle->TxXferSize = len;
	
	/*data will received on RX buffer */
	spi_handle->pRxBuffPtr = rx_buffer;
	spi_handle->RxXferCount = len;
	spi_handle->RxXferSize = len;
	
	/* driver is busy in rx */
	spi_handle->state = HAL_SPI_STATE_BUSY_RX;
	
	hal_spi_enable(spi_handle->Instance);
	
	/* read the data register once before enabling the RXNE
	interrupt to make sure DR is emty */
	val = spi_handle->Instance->DR; 
	
	/* Now eanable both txe and rxe interrupt */
	hal_spi_enable_rxne_interrupt(spi_handle->Instance);
	hal_spi_enable_txe_interrupt(spi_handle->Instance);
	
}



/**
  * @brief API used to do slave data transmission
  * @param *SPIx : Based address of SPI
  * @param *buffer  : Pointer to tx buffer 
  * @param len : lenght of tx data 
  * @retval none
 */
void hal_spi_slave_tx(spi_handle_t *spi_handle, uint8_t *tx_buffer, uint32_t len)
{
	
	/*populate pointers and length information to Tx the data*/
	spi_handle->pTxBuffPtr = tx_buffer;
	spi_handle->TxXferCount = len;
	spi_handle->TxXferSize = len;
	
	/*pointers to handle dummy rx , you can reuse the same pointer */
	spi_handle->pRxBuffPtr = tx_buffer;
	spi_handle->RxXferCount = len;
	spi_handle->RxXferSize = len;
	
	/*driver is busy in doing TX*/
	spi_handle->state = HAL_SPI_STATE_BUSY_TX;
	
	hal_spi_enable(spi_handle->Instance);
	
	/*Now enable both rxne and txe */
	hal_spi_enable_rxne_interrupt(spi_handle->Instance);
	hal_spi_enable_txe_interrupt(spi_handle->Instance);
	
}

/**
  * @brief API used to do slave data reception
  * @param *SPIx : Based address of SPI
  * @param *buffer  : Pointer to RX buffer 
  * @param len : lenght of RX data 
  * @retval none
 */
void hal_spi_slave_rx(spi_handle_t *spi_handle, uint8_t *rcv_buffer , uint32_t len)
{
	/*populate the rcv_buffer and along with the size in handle */
	spi_handle->pRxBuffPtr = rcv_buffer;
	spi_handle->RxXferCount = len;
	spi_handle->RxXferSize = len;
	
	/*driver in busy in RX*/
	spi_handle->state = HAL_SPI_STATE_BUSY_RX;
	
	/*slave need to receive data so enable rxne interrupt*/
	/*byte reception will be taken care in  RXNE interrupt handling code*/
	
}

/**
  * @brief Handles TXE interrupts.
  * @param hspi : pointer to spi_handle_t structre that contains the configuration 
                   information for SPI Module.
  * @retval none
 */
void hal_spi_handle_tx_interrupt(spi_handle_t *hspi)
{
	/* Transmitt the data in 8 bit mode*/
  if(hspi->Init.DataSize == SPI_8BIT_DF_ENABLE)
	{
    hspi->Instance->DR = (*hspi->pTxBuffPtr++);
    hspi->TxXferCount--; // Sent 1 byte
  }
	
 /* Trasnmitt the data in 16 bit mode*/
 	else
	{
    hspi->Instance->DR = *((uint16_t *)hspi->pTxBuffPtr);
    hspi->pTxBuffPtr += 2;
		hspi->TxXferCount -=2; // Sent 2 bytes in one go
	}
	
	if(hspi->TxXferCount == 0)
	{
		/*We reached end of TX, close TX interrupt*/
		hal_spi_close_tx_interrupt(hspi);
	}
	
}


/**
  * @brief Handles RXNE interrupts.
  * @param hspi : pointer to spi_handle_t structre that contains the configuration 
                   information for SPI Module.
  * @retval none
 */
void hal_spi_handle_rx_interrupt(spi_handle_t *hspi)
{
	/* Receive Data in 8 bit mode*/
	if(hspi->Init.DataSize == SPI_8BIT_DF_ENABLE)
	{
		//NULL check
		if(hspi->pRxBuffPtr++)
			*(hspi->pRxBuffPtr) = hspi->Instance->DR ; /*a read from the data register will return the value held in the Rx buffer*/
		
		hspi->RxXferCount--;
	}
	else
	{
		*((uint16_t*)hspi->pRxBuffPtr) = hspi->Instance->DR;
		hspi->pRxBuffPtr += 2;
		hspi->RxXferCount -= 2;
	}
	
	if(hspi->RxXferCount == 0)
	{
		/* Receiving of data is completed, close the RXNE interrupt*/
		hal_spi_close_rx_interrupt(hspi);
	}
}
	
	

/**
  * @brief This function handles SPI Interrupt request
  * @param hspi : pointer to spi_handle_t structre that contains the configuration 
                   information for SPI Module.
  * @retval none
 */
void hal_spi_irq_handler(spi_handle_t *hspi)
{
	uint32_t temp1 =0, temp2 =0;
	
	/*Check if RXNE bit isn set in status regsiter*/
	temp1 = hspi->Instance->SR & SPI_REG_SR_RXNE_FLAG;
	
	/*check if RXNEIE bit is enables in status regsiter */
	temp2 = hspi->Instance->CR2 & SPI_REG_CR2_RXNEIE_ENABLE;
	
	if((temp1 != RESET) && (temp2 !=RESET))
	{
		/* RXNE flag is set , handle the RX of data bytes*/
		hal_spi_handle_rx_interrupt(hspi);
		
		return;
	}
	
	/*Check if TXE bit isn set in status regsiter*/
	temp1 = hspi->Instance->SR & SPI_REG_SR_TXE_FLAG;
	temp2 = hspi->Instance->CR2 & SPI_REG_CR2_TXEIE_ENABLE;
	
	if((temp1 != RESET) && (temp2 !=RESET))
	{
		/*TXE flag is set, handle thye TX of data byte*/
		hal_spi_handle_tx_interrupt(hspi);
	  
		return;
	}
	
}



