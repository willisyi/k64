#ifndef __DELAY_H
#define __DELAY_H 	
#include <stdint.h>
#include "fsl_port_hal.h"



void Delay_Init(uint8_t SYSCLK);
void Delay_ms(uint16_t nms);
void Delay_us(uint32_t nus);


#endif
