//PC11 - BLUE LED as PWM output

#include "pwm.h"

#define RCC_CFGR_PPRE1_DIV1 (1U<<12) // APB1 prescaler = 1

void pwm_init(void)
{
	// Enable GPIOC clock
	RCC->IOPENR |= RCC_IOPENR_GPIOCEN;

	// Configure PC11 as alternate function mode
	GPIOC->MODER &= ~(3U << (2*11));  // Clear mode bits for PC11
	GPIOC->MODER |= (2U << (2*11));   // PC11 alternate function mode
	GPIOC->OSPEEDR |= (3U << (2*11)); // PC11 high speed
	GPIOC->AFR[1] &= ~(0xF << (4*(11-8))); // Clear AF bits for PC11
	GPIOC->AFR[1] |= (2U << (4*(11-8)));    // Set AF2 for TIM1_CH4

	// Enable TIM1 clock
	RCC->APBENR2 |= RCC_APBENR2_TIM1EN;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV1; // APB1 prescaler = 1

	// Configure TIM1 for PWM
	TIM1->PSC = 79; // Prescaler value (16 MHz clock, 16 MHz / (79 + 1) = 200 kHz timer clock)
	TIM1->ARR = 999; // Auto-reload value for 1 kHz PWM frequency
	TIM1->CCR4 = 0; // Initial duty cycle 0%

	// Configure PWM mode for channel 4
	TIM1->CCMR2 &= ~TIM_CCMR2_OC4M; // Clear output compare mode bits for channel 4
	TIM1->CCMR2 |= (6U << TIM_CCMR2_OC4M_Pos); // PWM mode 1
	TIM1->CCMR2 |= TIM_CCMR2_OC4PE; // Enable preload for CCR4

	// Enable main output for TIM1
	TIM1->BDTR |= TIM_BDTR_MOE; // Main output enable

	TIM1->CCER |= TIM_CCER_CC4E; // Enable output for channel 4

	TIM1->CR1 |= TIM_CR1_ARPE; // Enable auto-reload preload
	TIM1->EGR |= TIM_EGR_UG; // Generate an update event to load the prescaler value
	TIM1->CR1 |= TIM_CR1_CEN; // Start the timer
}

void set_pwm_duty_cycle(uint8_t duty_percent)
{
	if(duty_percent > 100)
	{
		duty_percent = 100; // Cap duty cycle at 100%
	}

	TIM1->CCR4 = (TIM1->ARR + 1) * duty_percent / 100; // Update CCR4 for desired duty cycle
}

void set_pwm_brightness(uint16_t brightness)
{
	if(brightness > 999)
	{
		brightness = 999; // Cap brightness at 100%
	}

	TIM1->CCR4 = brightness; // Update CCR4 for desired brightness
}

void pwm_fade(void)
{
	static uint16_t brightness = 0;
	    static uint8_t direction = 1; // 1 = up, 0 = down

	    if(direction)
	    {
	        brightness += 10;
	        if(brightness >= 999)
	        {
	        	direction = 0;
	        }
	    }
	    else
	    {
	        brightness -= 10;
	        if(brightness <= 0)
	        {
	        	direction = 1;
	        }
	    }

	    TIM1->CCR4 = brightness;
}









