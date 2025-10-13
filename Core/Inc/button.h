/*
 * button.c
 *
 *  Created on: Oct 12, 2025
 *      Author: ksrgo
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "cmsis_os.h"
#include "stm32g0xx_hal.h"
#include "stm32g071xx.h"

void button_gpio_init(void);
uint8_t is_button_pressed(void);


#endif /* INC_BUTTON_C_ */
