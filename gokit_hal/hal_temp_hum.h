#ifndef _HAL_HEMP_HUM_H
#define _HAL_HEMP_HUM_H

#include <stdio.h>
#include "fsl_gpio_driver.h"

enum dhtpin{
	kGokitDHT = GPIO_MAKE_PIN(GPIOA_IDX, 1U)
};
uint8_t DHT11_Init(void); //Init DHT11
uint8_t DHT11_Read_Data(uint8_t *temperature,uint8_t *humidity); //Read DHT11 Value
uint8_t DHT11_Read_Byte(void);//Read One Byte
uint8_t DHT11_Read_Bit(void);//Read One Bit
uint8_t DHT11_Check(void);//Chack DHT11
void DHT11_Rst(void);//Reset DHT11    



#endif /*_HAL_HEMP_HUM_H*/

