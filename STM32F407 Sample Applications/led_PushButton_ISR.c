/**************************************************************************************************************************
 * @file     led_PushButton_ISR.c
 * @author   Sharath N 
 * @brief    This is a sample application to demonstrate ISR, this program bliks the the blue LED of stm32f407 discovery 
             board, when user push button is pressed an Interrupt is generated which calls the ISR program. ISR turns ON
						 GREEN, RED and ORANGE LED one by one then turns of GEEN ,RED and ORANGE LED simultaneously. Once ISR is comple
						 -ted . Program execute main function i.e, it start blinking blue LED again.
 **************************************************************************************************************************/


#include "led.h"


void msDelay(uint32_t msTime)
{
	for(uint32_t i=0;i<msTime*4000;i++);
}

int main(void)
{
	
/* Initiale LEDs */
	led_init();
	
/* Push Button is connected to PA0, So enable portA clocl*/
	_HAL_RCC_GPIOA_CLK_ENABLE();

/* Configure button interrupt as falling edge */
	hal_gpio_configure_interrupt(GPIO_BUTTON_PIN,INT_FALLING_EDGE);
	
/* enable interrupt on EXTI0 line */
	hal_gpio_enable_interrupt(GPIO_BUTTON_PIN, EXTI0_IRQn);
	
    while(1)
    {
		
		led_turn_on(GPIOD, LED_BLUE);
		//led_turn_on(GPIOD, LED_BLUE);
		msDelay(1000);
		
		led_turn_off(GPIOD, LED_BLUE);
	  //led_turn_off(GPIOD, LED_BLUE);
		msDelay(1000);

    }

}


/**
  *@brief Interrupt Service routine(ISR) for the configured EXTIO interrupt
  *@retval none
*/
void EXTI0_IRQHandler(void)
{
	/* Clear the bit in pending request register */
	hal_gpio_clear_interrupt(GPIO_BUTTON_PIN);
	
	/* Do your tasks */
	led_turn_on(GPIOD, LED_GREEN);
	msDelay(1000);
	led_turn_on(GPIOD, LED_RED);
	msDelay(1000);
	led_turn_on(GPIOD, LED_ORANGE);
	msDelay(1000);
	led_turn_off(GPIOD, LED_GREEN);
	led_turn_off(GPIOD, LED_RED);
	led_turn_off(GPIOD, LED_ORANGE);
}







	
