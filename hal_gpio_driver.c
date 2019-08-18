#include <stdint.h>
#include "hal_gpio_driver.h"

/*************************************************************************************************************************/
/*                                                                                                                       */
/*                                 Static helper function                                                                */
/*                                                                                                                       */
/*************************************************************************************************************************/

/**
  * @breief Configure the mode of the pin : input, output, analog
  * @param  GPIOx : GPIO port base address
	* @param  pin_no : GPIO pin number
	* @param  mode : mode to be cinfigured
	* @retval none
**/	
static void hal_gpio_configure_pin_mode(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t mode)
{
	GPIOx->MODER |= (mode << (2 * pin_no)); 
}


/**
  * @breief Configure the output type of the pin
  * @param  GPIOx : GPIO port base address
	* @param  pin_no : GPIO pin number
	* @param  output_type: output type to be configured
	* @retval none
**/
static void hal_gpio_configure_pin_output_type(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t output_type)
{
	GPIOx->OTYPER |= ( output_type << pin_no);

}
/**
  * @breief Configure the Speed of the pin
  * @param  GPIOx : GPIO port base address
	* @param  pin_no : GPIO pin number
	* @param  speed : value of the spedd
	* @retval none
**/
static void hal_gpio_configure_pin_speed(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t speed)
{
	GPIOx->OSPEEDR |= (speed << (2 * pin_no));
}



/**
  * @breief Activate the internal pull-up/pull-down resistor
  * @param  GPIOx : GPIO port base address
	* @param  pin_no : GPIO pin number
	* @param  pupd : specifies which resistor to activate
	* @retval none
**/
static void hal_gpio_configure_pin_pupd(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t pupd)
{
	GPIOx->PUPDR |= (pupd << (2 * pin_no));	
}


/************************************************************************************************************************/



/**
  * @brief Initialize the GPIO pin 
  * @param GPIOx : GPIO port base address
  * @param gpio_pin_config : Pointer to pin config structure sent by application
  * @retval none
**/
void hal_gpio_init(GPIO_TypeDef *GPIOx, gpio_pin_config_typedef *gpio_pin_config)
{
	//Configure Pin MOde
	hal_gpio_configure_pin_mode(GPIOx, gpio_pin_config->pin,gpio_pin_config->mode);
	
	//Configure the output type of the pin
	hal_gpio_configure_pin_output_type(GPIOx, gpio_pin_config->pin, gpio_pin_config->output_type);
	
	//Configure the Speed of the pin
	hal_gpio_configure_pin_speed(GPIOx, gpio_pin_config->pin, gpio_pin_config->speed);
	
	//Activate the internal pull-up/pull-down resistor
	hal_gpio_configure_pin_pupd(GPIOx, gpio_pin_config->pin, gpio_pin_config->pull);
	
	
}

/**
  * @brief Read from GPIO pin
  * @param GPIOx : GPIO port base address
  * @param pin_no: GPIO pin number 
  * @retval uint6_t : Value Read
**/
uint8_t hal_gpio_read_from_pin(GPIO_TypeDef *GPIOx, uint16_t pin_no)
{
	
	uint8_t value;
	/* Read from the input data register (IDR), then right shift by the value of pin number to get
	   input status value of the pin to LSB. */
	value =  (((GPIOx->IDR) >> pin_no) & 0x00000001);
	
	return value;
	
}


/**
  * @brief Writeb to GPIO pin
  * @param GPIOx : GPIO port base address
  * @param pin_no: GPIO pin number
  * @param value : Value to be written	
  * @retval none
**/
void hal_gpio_write_to_pin(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint8_t value)
{
	if(value)
		GPIOx->ODR |= (1 << pin_no);
	else
		GPIOx->ODR &= ~(1 << pin_no); 
	
	/* if(value)
		GPIOx->BSRR = (1 << pin_no); 
	else
		GPIOx->BSRR = (1 << (2 * pin_no));*/
	
	
	
}

/**
  * @brief Set the alternate function for given gpio pin
  * @param GPIOx : GPIO port base address
  * @param pin_no: GPIO pin number 
  * @retval alt_fun_value : alternate function to be configured with
	* @retval none
**/
void hal_gpio_set_alt_function(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint16_t alt_fun_value)
{ 
	
	// AFR[0] - Alternate function Low register
	// AFR[1] - Alternate function High register
	if(pin_no <= 7)
	{
		GPIOx->AFR[0] |= (alt_fun_value << ( 4 * pin_no ));
	}
	else
	{
		GPIOx->AFR[1] |= (alt_fun_value << ( 4 * (pin_no % 8)));
	}
	
}



/**
  * @brief Configure the interrupt for a given pin number
  * @param pin_no: GPIO pin number 
	* @param edge_sel: Triggering edge selection value of type "int_edge_sel_t".
	* @retval none
 */ 
void hal_gpio_configure_interrupt(uint16_t pin_no, int_edge_sel_t edge_sel)
{
	if(edge_sel == INT_RISING_EDGE)
	{
		EXTI->RTSR |= (1 << pin_no);
	}
	else if(edge_sel == INT_FALLING_EDGE)
	{
		EXTI->FTSR |= (1 << pin_no);
	}
	else if(edge_sel == INT_RISING_FALLING_EDGE)
	{
		EXTI->RTSR |= (1 << pin_no);
		EXTI->FTSR |= (1 << pin_no);

	}
	else
	{
		//To do
	}
}


/**
  * @brief Enable the interrupt for a given pin number and irq number
  * @param pin_no: GPIO pin number 
	* @param irq_no: Irq number to be enabled in NVIC
	* @retval none
 */ 
void hal_gpio_enable_interrupt(uint16_t pin_no, IRQn_Type irq_no)
{
	EXTI->IMR |= (1 << pin_no);
	NVIC_EnableIRQ(irq_no);
}


/**
  * @brief Clear the sticky interrupt pending bit if set
  * @param pin_no: GPIO pin number 
	* @retval none
 */
void hal_gpio_clear_interrupt(uint16_t pin)
{
	if(EXTI->PR & (1 << pin ))
	{
		EXTI->PR |= ( 1 << pin);
	}
}





