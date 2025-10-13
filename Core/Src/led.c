#include "led.h"

void led_gpio_init(void)
{
	// Enable GPIOC clock
	RCC->IOPENR |= RCC_IOPENR_GPIOCEN;

	// Configure PC10, PC11, PC12 as output
	GPIOC->MODER &= ~(3U << (2*10));  // Clear mode bits for PC10
	GPIOC->MODER |= (1U << (2*10));   // PC10 output mode
	GPIOC->MODER &= ~(3U << (2*11));  // Clear mode bits for PC11
	GPIOC->MODER |= (1U << (2*11));   // PC11 output mode
	GPIOC->MODER &= ~(3U << (2*12));  // Clear mode bits for PC12
	GPIOC->MODER |= (1U << (2*12));   // PC12 output mode
	GPIOC->OSPEEDR |= (3U << (2*10)); // PC10 high speed
	GPIOC->OSPEEDR |= (3U << (2*11)); // PC11 high speed
	GPIOC->OSPEEDR |= (3U << (2*12)); // PC12 high speed
}

void led_on(uint16_t led_pin)
{
	GPIOC->ODR |= (1U << led_pin); // Set the pin high to turn on the LED
}

void led_off(uint16_t led_pin)
{
	GPIOC->ODR &= ~(1U << led_pin); // Set the pin low to turn off the LED
}



