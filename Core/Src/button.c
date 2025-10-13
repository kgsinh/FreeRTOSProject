#include "button.h"

void button_gpio_init(void)
{
	// Enable GPIOC clock
	RCC->IOPENR |= RCC_IOPENR_GPIOCEN;

	// Configure PC13 as input
	GPIOC->MODER &= ~(3U << (2*13));  // Clear mode bits for PC13 (input mode)
	GPIOC->PUPDR |= (1U << (2*13));   // PC13 pull-up resistor enabled
}

uint8_t is_button_pressed(void)
{
	// Return 1 if button is pressed (active low), otherwise return 0
	return (GPIOC->IDR & (1U << 13)) == 0;
}
