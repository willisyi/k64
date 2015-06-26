#ifndef _HAL_INFRARED_H
#define _HAL_INFRARED_H

#define Infrared_EXTI
#include <stdio.h>
#include "fsl_gpio_driver.h"
enum _infrared_pin{
	kInfPin = GPIO_MAKE_PIN(GPIOB_IDX, 9U)
};

void IR_Init(void);
void IR_Handle(void);
#endif /*_HAL_INFRARED_H*/


