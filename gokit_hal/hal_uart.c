/**
  ******************************************************************************
  * @file    Hal_OLED.c
  * @author  guo8113
	* @QQ      917581001 
	* @email 	 guo8113@126.com
  * @version V1.0
  * @date    15/06/2015
  * @brief   1，专业嵌入式智能设备方案定制
						 2，承接Freescale,NXP,ST Cortex M项目及TI DM816x 系列项目
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2014 jason</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/

#include "hal_uart.h"
#include "fsl_port_hal.h"
//for usart test
const uint8_t buffStart[]   = "\n\r++++++++++++++++ UART Send/Receive Blocking Example Start +++++++++++++++++\n\r";
const uint8_t bufferData1[] = "\n\rType characters from keyboard, the board will receive and then echo them to terminal screen\n\r";


static uart_state_t uartState;
void UARTx_Init(void)
{
		PORT_HAL_SetMuxMode(PORTC,16,kPortMuxAlt3);
	  PORT_HAL_SetMuxMode(PORTC,17,kPortMuxAlt3);

    uart_user_config_t uartConfig = {
        .bitCountPerChar = kUart8BitsPerChar,
        .parityMode      = kUartParityDisabled,
        .stopBitCount    = kUartOneStopBit,
        .baudRate        = GOKIT_UART_BAUD
    };
	  //Notice !!!! First init LPTMR used by UART
		OSA_Init();
		UART_DRV_Init(GOKIT_UART_INSTANCE, &uartState, &uartConfig);
		//UART_DRV_SendDataBlocking(UART_INSTANCE, buffStart, byteCountBuff, timeout);
}
void UARTx_test(void)
{
    uint8_t rxChar = 0;
    uint32_t byteCountBuff = 0;	
	// Inform to start blocking example
    byteCountBuff = sizeof(buffStart);
    UART_DRV_SendDataBlocking(GOKIT_UART_INSTANCE, buffStart, byteCountBuff, 1000u);

    // Inform user of what to do
    byteCountBuff = sizeof(bufferData1);
    UART_DRV_SendDataBlocking(GOKIT_UART_INSTANCE, bufferData1, byteCountBuff, 1000u);

    // Send/receive blocking function
    while(true)
    {
        // Wait to receive input data
        if (kStatus_UART_Success == UART_DRV_ReceiveDataBlocking(GOKIT_UART_INSTANCE, &rxChar, 1u, OSA_WAIT_FOREVER))
        {
            // Echo received character
            UART_DRV_SendDataBlocking(GOKIT_UART_INSTANCE, &rxChar, 1u, 1000u);
        }
    }
}