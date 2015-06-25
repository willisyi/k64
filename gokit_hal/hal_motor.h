#ifndef _HAL_MOTOR_H
#define _HAL_MOTOR_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "fsl_gpio_driver.h"
#include "fsl_port_hal.h"
#include "fsl_ftm_driver.h"

#define Motor_stop        0
#define Motor_Forward     1
#define Motor_Reverse     2


#define MOT1 PBout(8)  //PTB23
#define MOT2 PBout(9)	 //PTA2-FTM0 ch7

#define GOKIT_FTM_INSTANCE    0
#define GOKIT_FTM_CHANNEL    7

enum _motor_pin{
	kMot1 = GPIO_MAKE_PIN(GPIOB_IDX, 23U),
	kMot2 = GPIO_MAKE_PIN(GPIOA_IDX, 2U)			// support FTM in k64f
};

#define MOTOR_ARR 899 //8kHZ
#define MOTOR_MAX 100
#define MOTOR_MAX1 -100
#define MOTOR_MIN 0

		
void Motor_Init(void);
void Motor_status(uint16_t status);

#endif /*_HAL_MOTOR_H*/


