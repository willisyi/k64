#ifndef _HAL_RGB_LED_H
#define _HAL_RGB_LED_H

#include "fsl_gpio_driver.h"

#define  R_MAX  255
#define  G_MAX  255
#define  B_MAX  255

enum _virtual_i2c{
	kSDA = GPIO_MAKE_PIN(GPIOE_IDX, 25U),//K64 PTE24 is i2c0's SCL
	kSCL = GPIO_MAKE_PIN(GPIOE_IDX, 24U),//K64 PTE25 is i2c0's SDA
};

#define SCL_LOW 	GPIO_DRV_ClearPinOutput(kSCL)
#define SCL_HIGH 	GPIO_DRV_SetPinOutput(kSCL)

#define SDA_LOW		GPIO_DRV_ClearPinOutput(kSDA)
#define SDA_HIGH	GPIO_DRV_SetPinOutput(kSDA)

void RGB_LED_Init(void);
void LED_R_Control(uint8_t R);
void LED_G_Control(uint8_t G);
void LED_B_Control(uint8_t B);
void LED_RGB_Control(uint8_t R,uint8_t B,uint8_t G);

#endif /*_HAL_RGB_LED_H*/

