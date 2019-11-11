/***************************************************************************************************************************
  * @file    hal_i2c_driver.c
  * @author  Sharath N
  * @brief   I2C HAL module driver,
             This file provides firmware functions to manage the functionality of I2C in STM32F407 Discovery Board.
***************************************************************************************************************************/

#include <stdint.h>
#include "hal_i2c_driver.h"
#include "led.h"

/***************************************************************************************************************************/
/*                                                                                                                         */
/*                                               Helper functions                                                          */
/*                                                                                                                         */
/***************************************************************************************************************************/

/**
  * @brief  Enable the given i2c module
  * @param  *i2cx : Base address of I2C peripheral
  * @retval  none
 */
static void hal_i2c_enable_peripheral(I2C_TypeDef *i2cx)
{
	i2cx->CR1 |= I2C_REG_CR1_ENABLE_I2C;
}



/**
  * @brief  Disable the given i2c module
  * @param  *i2cx : Base address of I2C peripheral
  * @retval  none
 */
static void hal_i2c_disable_peripheral(I2C_TypeDef *i2cx)
{
	i2cx->CR1 &= ~I2C_REG_CR1_ENABLE_I2C;
}



/**
  * @brief  Enable or disable clock stretching 
  * @param  *i2cx : Base address of I2C peripheral
  * @param  no_stretch: clock stretching enable/disable
  * @retval  none
 */
static void hal_i2c_manage_clock_stretch(I2C_TypeDef *i2cx, uint32_t no_stretch)
{
	if(no_stretch)
	{
		//Clock stretching disabled
		i2cx->CR1 |= I2C_REG_CR1_NOSTRETCH;
	}
	else
	{
		i2cx->CR1 &= ~I2C_REG_CR1_NOSTRETCH;
	}
}





/**
  * @brief  Configure the own I2C device Clock configuration register
  * @param  *i2cx : Base address of I2C peripheral
  * @param  pclk : I2C peripherl clock frequency
  * @retval  none
 */
static void hal_i2c_configure_ccr(I2C_TypeDef *i2cx, uint32_t pclk, uint32_t clkspeed,uint32_t duty_cycle)
{
	i2cx->CCR;
	
	
}


/**
  * @brief  Configure the own I2C device address
  * @param  *i2cx : Base address of I2C peripheral
  * @param  own_address : address of the I2C device to be configured
  * @retval  none
 */
static void hal_i2c_set_own_address1(I2C_TypeDef *i2cx, uint32_t own_address)
{
	i2cx->OAR1 &=  ~((0x7f) << 1);  //Bit 1 to 7 are cleared  
	i2cx->OAR1 |=  (own_address << 1); // Set the given 7-bit adrress from bit 1
}



/**
  * @brief  Configure the I2C addressing mode either 7bit or 10bit
  * @param  *i2cx : Base address of I2C peripheral
  * @param  adr_mode : addressing mode to be configured.
  * @retval  none
 */
static void hal_i2c_set_addressing_mode(I2C_TypeDef *i2cx, uint32_t adr_mode)
{
	if(adr_mode == I2C_ADDRMODE_10BIT)
	{
		i2cx->OAR1 |= I2C_REG_OAR1_ADDRMODE;
	}
	else
	{
		i2cx->OAR1 &= ~I2C_REG_OAR1_ADDRMODE;
	}
}



/**
  * @brief  Configure the I2C Clock duty cycle in Fast Mode
  * @param  *i2cx : Base address of I2C peripheral
  * @param  duty_cycle : duty cycle to be configured. It could be either I2C_FM_DUTY_16BY9 or I2C_FM_DUTY_2.
  * @retval  none
 */
static void hal_i2c_set_fm_mode_duty_cycle(I2C_TypeDef *i2cx, uint32_t duty_cycle)
{
	if(duty_cycle == I2C_FM_DUTY_16BY9)
	{
		i2cx->CCR |= I2C_REG_CCR_DUTY;
	}
	else
	{
		i2cx->CCR |= I2C_REG_CCR_DUTY;
	}
}



/**
  * @brief  Does I2C Clock realated initialization
  * @param  *i2cx : Base address of I2C peripheral
  * @param  clkspeed: I2C clock speed
  * @param  duty_cycle : I2C duty cycle
  * @retval  none
 */
static void hal_i2c_clk_init(I2C_TypeDef *i2cx, uint32_t clkspeed, uint32_t duty_cycle)
{
	uint32_t pclk = I2C_PERIPHERAL_CLK_FREQ_10MHZ;
	i2cx->CR2 &= ~(0x3F);
	i2cx->CR2 |= (pclk & 0x3F);
	hal_i2c_configure_ccr(i2cx,pclk,clkspeed,duty_cycle);
	//hal_i2c_rise_time_configuration(i2cx,pclk,clkspeed);
}



/**
  * @brief  Generate start condition. Used only by master.
  * @param  *i2cx : Base address of I2C peripheral
  * @retval  none
 */
static void hal_i2c_generate_start_condition(I2C_TypeDef *i2cx)
{
	i2cx->CR1 |= I2C_REG_CR1_START_GEN;
}



/**
  * @brief  Generate stop condition. Used only by master.
  * @param  *i2cx : Base address of I2C peripheral
  * @retval  none
 */
static void hal_i2c_generate_stop_condition(I2C_TypeDef *i2cx)
{
	i2cx->CR1 |= I2C_REG_CR1_STOP_GEN;
}



/**
  * @brief  Enables/Disables TXE and RXNE(buffer) interrupt
  * @param  *i2cx : Base address of I2C peripheral
  * @param  enable: if this is set to 0, tx and rx interrupt will be disabled.
  * @retval  none
 */
static void hal_i2c_configure_buffer_interrupt(I2C_TypeDef *i2cx, uint32_t enable)
{
	if(enable)
	{
		i2cx->CR2 |= I2C_REG_CR2_BUF_INT_ENABLE;
	}
	else
	{
		i2cx->CR2 &= ~I2C_REG_CR2_BUF_INT_ENABLE;
	}
}



/**
  * @brief  Enables/Disables Error interuupt
  * @param  *i2cx : Base address of I2C peripheral
  * @param  enable: if this is set to 0, error interrupt will be disabled.
  * @retval  none
 */
static void hal_i2c_configure_error_interrupt(I2C_TypeDef *i2cx, uint32_t enable)
{
	if(enable)
	{
		i2cx->CR2 |= I2C_REG_CR2_ERR_INT_ENABLE;
	}
	else
	{
		i2cx->CR2 &= ~I2C_REG_CR2_ERR_INT_ENABLE;
	}
}



/**
  * @brief  Enables/Disables Event interuupt
  * @param  *i2cx : Base address of I2C peripheral
  * @param  enable: if this is set to 0, event interrupt will be disabled.
  * @retval  none
 */
static void hal_i2c_configure_event_interrupt(I2C_TypeDef *i2cx, uint32_t enable)
{
	if(enable)
	{
		i2cx->CR2 |= I2C_REG_CR2_EVT_INT_ENABLE;
	}
	else
	{
		i2cx->CR2 &= ~I2C_REG_CR2_EVT_INT_ENABLE;
	}
}



/**
  * @brief  Check whether bus is busy or free
  * @param  *i2cx : Base address of I2C peripheral
  * @retval  returns 1 if bus is busy.
 */
static uint8_t hal_i2c_is_bus_busy(I2C_TypeDef *i2cx)
{
	if(i2cx->SR2 & I2C_REG_SR2_BUS_BUSY_FLAG)
	  return 1;
	else
		return 0;
}



/**
  * @brief  Call this function to wait until SB(start byte) flag is set.
  * @param  *i2cx : Base address of I2C peripheral
  * @retval  none
 */
static void hal_i2c_wait_until_sb_set(I2C_TypeDef *i2cx)
{
	/* Wait until SB flag is set */
	while( !(i2cx->SR1 & I2C_REG_SR1_SB_FLAG));
}



/**
  * @brief  Call this function to wait until ADDR flag is set.
  * @param  *i2cx : Base address of I2C peripheral
  * @retval  none
 */
static void hal_i2c_wait_until_addr_set(I2C_TypeDef *i2cx)
{
	/* Wait until SB flag is set */
	while( !(i2cx->SR1 & I2C_REG_SR1_ADDR_SENT_FLAG));
}


/**
  * @brief  Clear addr flag
  * @param  hi2c :  pointer to i2c_handle_t structure which contains I2C configuration information of I2C module
  * @retval  none
 */
static void hal_i2c_clear_addr_flag(i2c_handle_t *hi2c)
{
	uint32_t temp;
	/*Clear ADDR flag */
	temp = hi2c->Instance->SR1; //Read SR1
	temp = hi2c->Instance->SR2; //Read SR2
	
}



/**
  * @brief  Clear STOP flag
  * @param  hi2c :  pointer to i2c_handle_t structure which contains I2C configuration information of I2C module
  * @retval  none
 */
static void hal_i2c_clear_stop_flag(i2c_handle_t *hi2c)
{
	uint32_t temp;
	/*Clear ADDR flag */
  temp = hi2c->Instance->SR1; //Read SR1
	hi2c->Instance->CR1 |= I2C_REG_CR1_ENABLE_I2C; //Write CR1
	
}




/**
  * @brief   Handle STOPF flag for slave 
  * @param  hi2c :  pointer to i2c_handle_t structure which contains I2C configuration information of I2C module
  * @retval  none
 */
static void hal_i2c_slave_handle_stop_condition(i2c_handle_t *hi2c)
{
		/* Disable buffer, event and error interrupt */
		hi2c->Instance->CR2 &= ~I2C_REG_CR2_BUF_INT_ENABLE;
		hi2c->Instance->CR2 &= ~I2C_REG_CR2_EVT_INT_ENABLE;
		hi2c->Instance->CR2 &= ~I2C_REG_CR2_ERR_INT_ENABLE;
	
	  /*Clear STOP flag */
	  hal_i2c_clear_stop_flag(hi2c);
	  
	  /*Disable ACK*/
	  hi2c->Instance->CR1 &= ~I2C_REG_CR1_ACK;
	  
	  hi2c->State = HAL_I2C_STATE_READY;
}


/**
  * @brief  This function sends the slave address.
  * @param  *i2cx : Base address of I2C peripheral
  * @param  slave_addr : Slave addresss 
  * @retval  none
 */
static void hal_i2c_send_addr_first(I2C_TypeDef *i2cx, uint8_t slave_addr)
{
	i2cx->DR = slave_addr;
}



/**
  * @brief  Handle the TXE interrupt for the master
  * @param  hi2c :  pointer to i2c_handle_t structure which contains I2C configuration information of I2C module
  * @retval  none
 */
static void hal_i2c_master_handle_TXE_interrupt(i2c_handle_t *hi2c)
{
	/* Write data to data register */
	hi2c->Instance->DR = (*hi2c->pBuffPtr++);
	hi2c->XferCount--;
	
	if(hi2c->XferCount == 0)
	{
		/*Disabled the buffer interrupt */
		hi2c->Instance->CR2 &= ~I2C_REG_CR2_BUF_INT_ENABLE;
	}
}



/**
  * @brief  Handle BTF flag for master transmitter
  * @param  hi2c :  pointer to i2c_handle_t structure which contains I2C configuration information of I2C module
  * @retval  none
 */
static void hal_i2c_master_tx_handle_btf(i2c_handle_t *hi2c)
{
	if(hi2c->XferCount != 0)
	{
		/*Write data to DR*/
		hi2c->Instance->DR = (*hi2c->pBuffPtr++);
	  hi2c->XferCount--;
	}
	else
	{
		/* Disable buffer, event and error interrupt */
		hi2c->Instance->CR2 &= ~I2C_REG_CR2_BUF_INT_ENABLE;
		hi2c->Instance->CR2 &= ~I2C_REG_CR2_EVT_INT_ENABLE;
		hi2c->Instance->CR2 &= ~I2C_REG_CR2_ERR_INT_ENABLE;
		
		/*Generate stop condition */
		hi2c->Instance->CR1 |= I2C_REG_CR1_STOP_GEN;
		
		/*Since all bytes are sent make state as ready*/
		hi2c->State = HAL_I2C_STATE_READY; 
	}		

}




/**
  * @brief  Handle the TXE flag for the slave
  * @param  hi2c :  pointer to i2c_handle_t structure which contains I2C configuration information of I2C module
  * @retval  none
 */
static void hal_i2c_slave_handle_TXE_interrupt(i2c_handle_t *hi2c)
{
	if(hi2c->XferCount != 0)
	{
		/*Write data to DR*/
		hi2c->Instance->DR = (*hi2c->pBuffPtr++);
	  hi2c->XferCount--;
	}
}
	



/**
  * @brief  Handle the BTF flag for the slave transmitter
  * @param  hi2c :  pointer to i2c_handle_t structure which contains I2C configuration information of I2C module
  * @retval  none
 */
static void hal_i2c_slave_tx_handle_btf(i2c_handle_t *hi2c)
{
  if(hi2c->XferCount != 0)
	{
		/*Write data to DR*/
		hi2c->Instance->DR = (*hi2c->pBuffPtr++);
	  hi2c->XferCount--;
	}
}



/**
  * @brief  Handle the RXNE flag for the slave
  * @param  hi2c :  pointer to i2c_handle_t structure which contains I2C configuration information of I2C module
  * @retval  none
 */
static void hal_i2c_slave_handle_RXNE_interrupt(i2c_handle_t *hi2c)
{
	if(hi2c->XferCount != 0)
	{
		/*read from DR*/
		(*hi2c->pBuffPtr++) = hi2c->Instance->DR;
	  hi2c->XferCount--;
	}
}



/**
  * @brief  Handle the BTF flag for the slave receiver
  * @param  hi2c :  pointer to i2c_handle_t structure which contains I2C configuration information of I2C module
  * @retval  none
 */
static void hal_i2c_slave_rx_handle_btf(i2c_handle_t *hi2c)
{
		if(hi2c->XferCount != 0)
	{
		/*read from DR*/
		(*hi2c->pBuffPtr++) = hi2c->Instance->DR;
	  hi2c->XferCount--;
	}
}




/**
  * @brief  Handle AKC failure condition
  * @param  hi2c :  pointer to i2c_handle_t structure which contains I2C configuration information of I2C module
  * @retval  none
 */
static void hal_i2c_slave_handle_ack_failure(i2c_handle_t *hi2c)
{
	/* Disable buffer, event and error interrupt */
	hi2c->Instance->CR2 &= ~I2C_REG_CR2_BUF_INT_ENABLE;
	hi2c->Instance->CR2 &= ~I2C_REG_CR2_EVT_INT_ENABLE;
	hi2c->Instance->CR2 &= ~I2C_REG_CR2_ERR_INT_ENABLE;
	
	/*Clear AF flag */
	hi2c->Instance->SR1 &= ~I2C_REG_SR1_AF_FAILURE_FLAG;
	
	/*Disable ACK */
	hi2c->Instance->CR1 &= ~I2C_REG_CR1_ACK;
	
	hi2c->State = HAL_I2C_STATE_READY;
}



/**
  * @brief  I2C error callbacks
  * @param  I2Chandle : I2C handle
  * @retval  none
 */
void hal_i2c_error_cb(i2c_handle_t *I2Chandle)
{
	while(1)
	{
		led_toggle(GPIOD, LED_RED);
	}
}
















/***********************************************************************************************************************************/
/*                                                                                                                                 */
/*                                               Driver Exposed APIs                                                               */
/*                                                                                                                                 */
/***********************************************************************************************************************************/

/**
  * @brief  Initializes the Given I2C peripherl
  * @param  *handle : Handle to i2c peripheral, which the application wants to initialize.
  * @retval  none
 */
void hal_i2c_init(i2c_handle_t *handle)
{
	/* I2C Clock initializatio */
	
	/* Set I2C addressing mode */
	
	/* Enable ACking */
	
	/* Enable clock stretching */
	
	/* Configure the own address */
	
	/* Finally, enable the i2c peripheral */
	
}



/**
  * @brief API to do master data transmission
  * @param *handle: pointer to handle structure of I2C peripheral
  * @param slave_address: address to whuch we want to TX.
  * @param *buffer: hold the pointer to tx buffer 
	* @param len: length of the data to be transmitted
  * @retval none
 */
void hal_i2c_master_tx(i2c_handle_t *handle, uint8_t slave_address, uint8_t *buffer, uint32_t len)
{
	/*Populate the handle with tx buffer pointer and length information */
	handle->pBuffPtr = buffer;
	handle->XferCount = len;
	handle->XferSize = len;
	handle->State = HAL_I2C_STATE_BUSY_TX;
	
	/*Make sure the i2c is enabled */
	hal_i2c_enable_peripheral(handle->Instance);
	
	/*Generate the start condition */
	hal_i2c_generate_start_condition(handle->Instance);
	
	/*Wiat till SB is set */
	hal_i2c_wait_until_sb_set(handle->Instance);
	
	/*address phase : send 7 bit slave address with r/w bit */
	hal_i2c_send_addr_first(handle->Instance, slave_address);
	
	/*Wait untill addr is set */
  hal_i2c_wait_until_addr_set(handle->Instance);
	
	/*If you are here, then addr is set and clock is stretched and i2c is in wait state*/
	/*Clear addr flag and make i2c come out of wait state*/
	hal_i2c_clear_addr_flag(handle);
	
	/*Enable buffer ,event and error interrupt */
	hal_i2c_configure_buffer_interrupt(handle->Instance,1);
	hal_i2c_configure_event_interrupt(handle->Instance,1);
	hal_i2c_configure_error_interrupt(handle->Instance,1);

}





/**
  * @brief API to do master data rception
  * @param *handle: pointer to handle structure of I2C peripheral
  * @param slave_address: address to whuch we want to TX.
  * @param *buffer: hold the pointer to rx buffer 
	* @param len: length of the data to be transmitted
  * @retval none
 */
void hal_i2c_master_rx(i2c_handle_t *handle, uint8_t slave_address, uint8_t *buffer, uint32_t len)
{
	
	/*Populate the handle with tx buffer pointer and length information */
	handle->pBuffPtr = buffer;
	handle->XferCount = len;
	handle->XferSize = len;
	handle->State = HAL_I2C_STATE_BUSY_RX;
	
	/*Make sure the i2c is enabled */
	hal_i2c_enable_peripheral(handle->Instance);
	
	/*Make sure that POS bit is disabled*/
	handle->Instance->CR1 &= ~I2C_REG_CR1_POS;
	
	/*Make sure that ACKing is enabled */
	handle->Instance->CR1 |= I2C_REG_CR1_ACK;
	 
	/*Generate the start condition */
	hal_i2c_generate_start_condition(handle->Instance);
	
	/*Wiat till SB is set */
	hal_i2c_wait_until_sb_set(handle->Instance);
	
	/*address phase : send 7 bit slave address with r/w bit */
	hal_i2c_send_addr_first(handle->Instance, slave_address);
	
	/*Wait untill addr is set */
  hal_i2c_wait_until_addr_set(handle->Instance);
	
	/*If you are here, then addr is set and clock is stretched and i2c is in wait state*/
	/*Clear addr flag and make i2c come out of wait state*/
	hal_i2c_clear_addr_flag(handle);
	
	/*Enable buffer ,event and error interrupt */
	hal_i2c_configure_buffer_interrupt(handle->Instance,1);
	hal_i2c_configure_event_interrupt(handle->Instance,1);
	hal_i2c_configure_error_interrupt(handle->Instance,1);

}




/**
  * @brief API to do the slave data transmission
  * @param *handle: pointer to handle structure of I2C peripheral
  * @param *buffer: hold the pointer to tx buffer 
	* @param len: length of the data to be transmitted
  * @retval none
 */
void hal_i2c_slave_tx(i2c_handle_t *handle, uint8_t *buffer, uint32_t len)
{
	
	/*Populate the handle with tx buffer pointer and length information */
	handle->pBuffPtr = buffer;
	handle->XferCount = len;
	handle->XferSize = len;
	handle->State = HAL_I2C_STATE_BUSY_TX;
	
	/*Make sure the i2c is enabled */
	hal_i2c_enable_peripheral(handle->Instance);
	
	/*Make sure that POS bit is disabled*/
	handle->Instance->CR1 &= ~I2C_REG_CR1_POS;
	
	/*Enable Address Acknowledging*/
	handle->Instance->CR1 |= I2C_REG_CR1_ACK;
	
	/*Enable buffer ,event and error interrupt */
	hal_i2c_configure_buffer_interrupt(handle->Instance,1);
	hal_i2c_configure_event_interrupt(handle->Instance,1);
	hal_i2c_configure_error_interrupt(handle->Instance,1);

}




/**
  * @brief API to do the slave data reception
  * @param *handle: pointer to handle structure of I2C peripheral
  * @param *buffer: hold the pointer to rx buffer 
	* @param len: length of the data to be transmitted
  * @retval none
 */
void hal_i2c_slave_rx(i2c_handle_t *handle, uint8_t *buffer, uint32_t len)
{
	
	/*Populate the handle with tx buffer pointer and length information */
	handle->pBuffPtr = buffer;
	handle->XferCount = len;
	handle->XferSize = len;
	handle->State = HAL_I2C_STATE_BUSY_RX;
	
	/*Make sure the i2c is enabled */
	hal_i2c_enable_peripheral(handle->Instance);
	
	/*Make sure that POS bit is disabled*/
	handle->Instance->CR1 &= ~I2C_REG_CR1_POS;
	
	/*Enable ACKing */
	handle->Instance->CR1 |= I2C_REG_CR1_ACK;
	
	/*Enable buffer ,event and error interrupt */
	hal_i2c_configure_buffer_interrupt(handle->Instance,1);
	hal_i2c_configure_event_interrupt(handle->Instance,1);
	hal_i2c_configure_error_interrupt(handle->Instance,1);

}




/**
  * @brief This function handles I2C error interrupt request
  * @param  hi2c :  pointer to i2c_handle_t structure which contains I2C configuration information of I2C module
  * @retval none
 */
void hal_i2c_handle_error_interrup(i2c_handle_t *hi2c)
{
	uint32_t temp1 = 0, temp2 = 0, temp3 = 0;
	/*Bus error Checking */
	temp1 = (hi2c->Instance->SR1 & I2C_REG_SR1_BUSS_ERROR_FLAG); //Chech if bus error occured 
	temp2 = (hi2c->Instance->CR2 & I2C_REG_CR2_ERR_INT_ENABLE); // Check if error interrupt enabled
	/* Bus Error Occured ----------------------------------------------------------------------- */
	if( temp1 && temp2)
	{
		hi2c->ErrorCode |= HAL_I2C_ERROR_BERR;
	  /* Clear Bus Error Flag*/
		hi2c->Instance->SR1 &= ~I2C_REG_SR1_BUSS_ERROR_FLAG;
	}
	
	
	/*Arbitration Loss Error  Checking */
	temp1 = (hi2c->Instance->SR1 & I2C_REG_SR1_ARLO_FLAG); //Chech if Arbitration Loss error occured 
	temp2 = (hi2c->Instance->CR2 & I2C_REG_CR2_ERR_INT_ENABLE); // Check if error interrupt enabled
	/* Arbitration Loss Error Occured ----------------------------------------------------------- */
	if( temp1 && temp2)
	{
		hi2c->ErrorCode |= HAL_I2C_ERROR_ARLO;
	  /* Clear ARLO Flag*/
		hi2c->Instance->SR1 &= ~I2C_REG_SR1_ARLO_FLAG;
	}
	
	
	/*ACK failure Error  Checking */
	temp1 = (hi2c->Instance->SR1 & I2C_REG_SR1_AF_FAILURE_FLAG); //Chech if ACK failure Error occured 
	temp2 = (hi2c->Instance->CR2 & I2C_REG_CR2_ERR_INT_ENABLE); // Check if error interrupt enabled
	/* ACK failre Error Occured ----------------------------------------------------------- */
	if( temp1 && temp2)
	{
		temp1 = (hi2c->Instance->SR2 & I2C_REG_SR2_MSL_FLAG);//Master mode check
		temp2 = hi2c->XferCount;// Transfer count check
		temp3 = hi2c->State; //I2C state
		
		if((!temp1 ) && (temp2) && (temp3 == HAL_I2C_STATE_BUSY_TX))
		{
			/* if ACK failure happens for slave, then slave should assume that master no longer needs any
			 data so slave should stop sending data*/
			hal_i2c_slave_handle_ack_failure(hi2c);
		}
		else
		{
			/* If ACK failure happens for master then its an error*/
			hi2c->State = HAL_I2C_ERROR_AF;
			/*Clear AF Flag*/
			hi2c->Instance->SR1 &= ~I2C_REG_SR1_AF_FAILURE_FLAG;
		}
			
	}
	

	/*Overun/Underun Error  Checking */
	temp1 = (hi2c->Instance->SR1 & I2C_REG_SR1_OVR_FLAG); //Chech if Overrun/Underrun error occured 
	temp2 = (hi2c->Instance->CR2 & I2C_REG_CR2_ERR_INT_ENABLE); // Check if error interrupt enabled
	/* Overun/Underun Error Occured ----------------------------------------------------------- */
	if( temp1 && temp2)
	{
		hi2c->ErrorCode |= HAL_I2C_ERROR_OVR;
	  /* Clear OVR Flag*/
		hi2c->Instance->SR1 &= ~I2C_REG_SR1_OVR_FLAG;
	}	
	
	if(hi2c->ErrorCode != HAL_I2C_ERROR_NONE)
	{
		hi2c->State = HAL_I2C_STATE_READY;
		
		/* Disable pos bit in I2C cr1 when error occured in master/mem Receive IT Process*/
		hi2c->Instance->CR1 &= ~I2C_REG_CR1_POS;
		hal_i2c_error_cb(hi2c);
	}
}
		
		
	
	