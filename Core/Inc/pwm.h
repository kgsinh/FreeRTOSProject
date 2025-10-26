#ifndef INC_PWM_H_
#define INC_PWM_H_

#include "main.h"
#include "cmsis_os.h"
#include "stm32g071xx.h"

void pwm_init(void);
void set_pwm_duty_cycle(uint8_t duty_percent);
void set_pwm_brightness(uint16_t brightness);
void pwm_fade(void);


#endif /* INC_PWM_H_ */
