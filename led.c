#include "led.h"

/** 
  * @brief  Initialize LEDs
	* @param  none
	* @retval none
**/
void led_init(void)
{
	
	/* Enable clock for  GPIOD port */
	_HAL_RCC_GPIOD_CLK_ENABLE();
	
	gpio_pin_config_typedef led_pin_config;
	
	led_pin_config.pin = LED_ORANGE;
	led_pin_config.mode = GPIO_PIN_OUTPUT_MODE;
	led_pin_config.output_type = GPIO_PIN_OUTPUT_TYPE_PUSHPULL;
	led_pin_config.speed = GPIO_PIN_SPEED_LOW;
	led_pin_config.pull = GPIO_PIN_NO_PUSH_PULL;
	
	hal_gpio_init(GPIO_PORT_D, &led_pin_config);
	
	led_pin_config.pin = LED_GREEN;
	hal_gpio_init(GPIO_PORT_D, &led_pin_config);
	
	led_pin_config.pin = LED_RED;
	hal_gpio_init(GPIO_PORT_D, &led_pin_config);
	
	led_pin_config.pin = LED_BLUE;
	hal_gpio_init(GPIO_PORT_D, &led_pin_config);
	
}


/** 
  * @brief  Turns on the LED which is connected on the given pin
	* @param  GPIOx : Base address of GPIO port
  * @param  pin : pin number of LED
	* @retval none
**/
void led_turn_on(GPIO_TypeDef *GPIOx, uint16_t pin)
{
	
	hal_gpio_write_to_pin(GPIOx, pin, 1);
	
}



/** 
  * @brief  Turns off the LED which is connected on the given pin
	* @param  GPIOx : Base address of GPIO port
  * @param  pin : pin number of LED
	* @retval none
**/
void led_turn_off(GPIO_TypeDef *GPIOx, uint16_t pin)
{
	
	hal_gpio_write_to_pin(GPIOx, pin, 0);
	
}


/** 
  * @brief  Toggle the LED which is connected on the given pin
	* @param  GPIOx : Base address of GPIO port
  * @param  pin : pin number of LED
	* @retval none
**/
void led_toggle(GPIO_TypeDef *GPIOx, uint16_t pin)
{
	
	if( hal_gpio_read_from_pin(GPIOx, pin))
	{
		hal_gpio_write_to_pin(GPIOx,pin,0);
	}
	else
	{
		hal_gpio_write_to_pin(GPIOx,pin,1);
	}
	
	/* Another Way or Logic --->
	
	hal_gpio_write_to_pin(GPIOx, pin, ~(hal_gpio_read_from_pin(GPIOx,pin));
	
	*/
	
}







	
	
