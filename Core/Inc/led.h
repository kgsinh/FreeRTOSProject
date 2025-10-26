/*
 * led.h
 *
 *  Created on: Oct 12, 2025
 *      Author: ksrgo
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "cmsis_os.h"
#include "stm32g071xx.h"

void led_gpio_init(void);
void led_on(uint16_t led_pin);
void led_off(uint16_t led_pin);





#endif /* INC_LED_H_ */
