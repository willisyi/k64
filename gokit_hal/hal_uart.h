#ifndef _HAL_UART_H
#define _HAL_UART_H

#include <stdio.h>
#include "fsl_uart_driver.h"

#define Open_UART3

#define GOKIT_UART_BAUD  9600U
#define GOKIT_UART_INSTANCE 3U

	
void UARTx_Init(void);
void UARTx_test(void);
void UART2_Send_DATA(uint8_t data);//UART3 on K64
void UART1_Send_DATA(uint8_t data);
#endif /*_HAL_UART_H*/


