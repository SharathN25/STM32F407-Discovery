#ifndef _HAL_GPIO_DRIVER_H
#define _HAL_GPIO_DRIVER_H

/*MCU specific header file for stm32f407vgt6 base discovery board */
#include "stm32f407xx.h"

/*************************************************************************************************************************/
/*                                                                                                                       */
/*                              1. Macros used for GPIO Initialization                                                   */
/*                                                                                                                       */
/*************************************************************************************************************************/
/* GPIO Mode Setting Values */
#define GPIO_PIN_INPUT_MODE                                          ( (uint32_t)0x00 )
#define GPIO_PIN_OUTPUT_MODE                                         ( (uint32_t)0x01 )
#define GPIO_PIN_ALT_FUN_MODE                                        ( (uint32_t)0x02 )
#define GPIO_PIN_ANALOG_MODE                                         ( (uint32_t)0x03 )


/* GPIO Output type selection value */
#define GPIO_PIN_OUTPUT_TYPE_PUSHPULL                                ( (uint32_t)0x00 )                            
#define GPIO_PIN_OUTPUT_TYPE_OPEN_DRAIN                              ( (uint32_t)0x01 )


/* GPIO Speed type selection value */
#define GPIO_PIN_SPEED_LOW                                           ( (uint32_t)0x00 )
#define GPIO_PIN_SPEED_MEDIUM                                        ( (uint32_t)0x01 )
#define GPIO_PIN_SPEED_HIGH                                          ( (uint32_t)0x02 )
#define GPIO_PIN_SPEED_VERY_HIGH                                     ( (uint32_t)0x03 )


/* GPIO  pull-up/pull-down values */
#define GPIO_PIN_NO_PUSH_PULL                                        ( (uint32_t)0x00 )
#define GPIO_PIN_PULL_UP                                             ( (uint32_t)0x01 )
#define GPIO_PIN_PULL_DOWN                                           ( (uint32_t)0x02 )


/*GPIO Port Addresss */
#define GPIO_PORT_A             GPIOA
#define GPIO_PORT_B             GPIOB   
#define GPIO_PORT_C             GPIOC   
#define GPIO_PORT_D             GPIOD  
#define GPIO_PORT_E             GPIOE  
#define GPIO_PORT_F             GPIOF
#define GPIO_PORT_G             GPIOG
#define GPIO_PORT_H             GPIOH  
#define GPIO_PORT_I             GPIOI


/* Macros for enabling clock for different GPIO ports in RCC Register*/   
#define _HAL_RCC_GPIOA_CLK_ENABLE()                                 (RCC->AHB1ENR |= (1 << 0))
#define _HAL_RCC_GPIOB_CLK_ENABLE()                                 (RCC->AHB1ENR |= (1 << 1))
#define _HAL_RCC_GPIOC_CLK_ENABLE()                                 (RCC->AHB1ENR |= (1 << 2))
#define _HAL_RCC_GPIOD_CLK_ENABLE()                                 (RCC->AHB1ENR |= (1 << 3))
#define _HAL_RCC_GPIOE_CLK_ENABLE()                                 (RCC->AHB1ENR |= (1 << 4))
#define _HAL_RCC_GPIOF_CLK_ENABLE()                                 (RCC->AHB1ENR |= (1 << 5))
#define _HAL_RCC_GPIOG_CLK_ENABLE()                                 (RCC->AHB1ENR |= (1 << 6))
#define _HAL_RCC_GPIOH_CLK_ENABLE()                                 (RCC->AHB1ENR |= (1 << 7))
#define _HAL_RCC_GPIOI_CLK_ENABLE()                                 (RCC->AHB1ENR |= (1 << 8))


/*************************************************************************************************************************/
/*                                                                                                                       */
/*                              2. Data structure for GPIO pin initialization                                            */
/*                                                                                                                       */
/*************************************************************************************************************************/

/**
* @brief GPIO Pin Configuration
*        This structure will be filled and passed to the driver  by application to initialize the GPIO pin.
**/

typedef struct
{
	
	uint32_t pin;               /* Specifies the GPIO pin to be configured */
	uint32_t mode;              /* Specifies the operatin mode for selected pin */
	uint32_t output_type;       /* Specifies the output type for selected pin*/
	uint32_t pull;              /* Specifies the pull-down / pull-up activation for selected pin */
	uint32_t speed;             /* Specifies the speed of the selected pin */
	uint32_t alternate;         /* Specifies alternate function value */

}gpio_pin_config_typedef;


/**
  *@brief Interrupt edge selection enum
*/

typedef enum{
	INT_RISING_EDGE,
	INT_FALLING_EDGE,
	INT_RISING_FALLING_EDGE
}int_edge_sel_t;


/*************************************************************************************************************************/
/*                                                                                                                       */
/*                                   3 . Driver Exposed API                                                              */
/*                                                                                                                       */
/*************************************************************************************************************************/

/**
  * @brief Initialize the GPIO pin 
  * @param GPIOx : GPIO port base address
  * @param gpio_pin_config : Pointer to pin config structure sent by application
  * @retval none
 */
void hal_gpio_init(GPIO_TypeDef *GPIOx, gpio_pin_config_typedef *gpio_pin_config);

/**
  * @brief Read from GPIO pin
  * @param GPIOx : GPIO port base address
  * @param pin_no: GPIO pin number 
  * @retval uint6_t : Value Read
 */
uint8_t hal_gpio_read_from_pin(GPIO_TypeDef *GPIOx, uint16_t pin_no);

/**
  * @brief Writeb to GPIO pin
  * @param GPIOx : GPIO port base address
  * @param pin_no: GPIO pin number
  * @param value : Value to be written	
  * @retval none
 */
void hal_gpio_write_to_pin(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint8_t value);

/**
  * @brief Set the alternate function for given gpio pin
  * @param GPIOx : GPIO port base address
  * @param pin_no: GPIO pin number 
  * @retval alt_fun_value : alternate function to be configured with
	* @retval none
 */
void hal_gpio_set_alt_function(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint16_t alt_fun_value);


/**
  * @brief Configure the interrupt for a given pin number
  * @param pin_no: GPIO pin number 
	* @param edge_sel: Triggering edge selection value of type "int_edge_sel_t".
	* @retval none
 */ 
void hal_gpio_configure_interrupt(uint16_t pin_no, int_edge_sel_t edge_sel);


/**
  * @brief Enable the interrupt for a given pin number and irq number
  * @param pin_no: GPIO pin number 
	* @param irq_no: Irq number to be enabled in NVIC
	* @retval none
 */ 
void hal_gpio_enable_interrupt(uint16_t pin_no, IRQn_Type irq_no);


/**
  * @brief Clear the sticky interrupt pending bit if set
  * @param pin_no: GPIO pin number 
	* @retval none
 */
 void hal_gpio_clear_interrupt(uint16_t pin);

#endif

