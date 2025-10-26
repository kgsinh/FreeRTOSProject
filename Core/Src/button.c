#include "button.h"

TaskHandle_t xButtonTaskHandle = NULL;

void button_gpio_init(void)
{
	// Enable GPIOC clock
	RCC->IOPENR |= RCC_IOPENR_GPIOCEN;

	// Configure PC13 as input
	GPIOC->MODER &= ~(3U << (2*13));  // Clear mode bits for PC13 (input mode)
	GPIOC->PUPDR &= ~(3U << (2*13)); // Clear pull-up/pull-down bits for PC13
	GPIOC->PUPDR |= (1U << (2*13));   // PC13 pull-up resistor enabled.

	//Interrupt configuration
	RCC->APBENR2 |= RCC_APBENR2_SYSCFGEN; // Enable SYSCFG clock

	// Connect EXTI13 line to PC13
	EXTI->EXTICR[3] &= ~(0x7U << 8);     // Clear EXTI13 bits
	EXTI->EXTICR[3] |= (2U << 8);   // Set EXTI13 to port C (0100)
	EXTI->IMR1 |= (1U << 13);       // Unmask EXTI13
	EXTI->FTSR1 |= (1U << 13);      // Trigger on falling edge
	EXTI->RTSR1 &= ~(1U << 13);     // Disable rising edge trigger

}

void button_enable_interrupt(void)
{
	// Clear any pending interrupt flag for EXTI13
	EXTI->FPR1 |= (1U << 13);       // Clear pending flag by writing 1
	NVIC_SetPriority(EXTI4_15_IRQn, 5); // Set priority (5 is suitable for FreeRTOS)
	NVIC_EnableIRQ(EXTI4_15_IRQn);      // Enable EXTI4_15 interrupt in NVIC
}


uint8_t is_button_pressed(void)
{
	// Return 1 if button is pressed (active low), otherwise return 0
	return (GPIOC->IDR & (1U << 13)) == 0;
}

void EXTI4_15_IRQHandler(void)
{
	if(EXTI->FPR1 & (1U << 13)) // Check if EXTI13 triggered the interrupt
	{
		EXTI->FPR1 |= (1U << 13); // Clear the pending interrupt flag by writing 1

		if(xButtonTaskHandle != NULL)
		{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(xButtonTaskHandle, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
		else
		{
			printf("Button ISR: xButtonTaskHandle is NULL!\n\r");
		}
	}
}
