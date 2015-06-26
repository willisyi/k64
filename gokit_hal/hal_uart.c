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
#include <stdio.h>
#include <string.h>
#include "hal_uart.h"
#include "fsl_port_hal.h"
#include "protocol.h"
#include "fsl_debug_console.h"

//for usart test
const uint8_t buffStart[]   = "\n\r++++++++++++++++ UART Send/Receive Blocking Example Start +++++++++++++++++\n\r";
const uint8_t bufferData1[] = "\n\rType characters from keyboard, the board will receive and then echo them to terminal screen\n\r";
extern UART_HandleTypeDef  		UART_HandleStruct;

static uart_state_t uartState;
int cnt=0;
uint8_t Buff[256];
void UART2_Send_DATA(uint8_t data);

/* Pointer to uart runtime state structure */
static void GOKIT_UART_PackData(const uint8_t vlue)
{
	if(UART_HandleStruct.Package_Flag ==0)
		{
			
			if(UART_HandleStruct.UART_Flag1 ==0)
			{
				if(vlue == 0xff)
				{   
						UART_HandleStruct.UART_Count = 0;						
						UART_HandleStruct.UART_Buf[UART_HandleStruct.UART_Count]=vlue;
						UART_HandleStruct.UART_Count++;	
						UART_HandleStruct.UART_Flag1 = 1;
				}			
				return ;
			}
			else if(UART_HandleStruct.UART_Flag2 ==0)
			{
					UART_HandleStruct.UART_Buf[UART_HandleStruct.UART_Count]=vlue;
					UART_HandleStruct.UART_Count++;
					if(UART_HandleStruct.UART_Buf[1] == 0xff)
					{
						UART_HandleStruct.UART_Flag2 = 1;	
						
					}					
					else
					{
						UART_HandleStruct.UART_Flag1 = 0;
					}
					return ;
			}
			else
			{
				UART_HandleStruct.UART_Buf[UART_HandleStruct.UART_Count] = vlue;
				if(UART_HandleStruct.UART_Count >=4 && UART_HandleStruct.UART_Buf[UART_HandleStruct.UART_Count] == 0x55 && UART_HandleStruct.UART_Buf[UART_HandleStruct.UART_Count - 1] == 0xFF)
				{}
				else 
				UART_HandleStruct.UART_Count++;
				if(UART_HandleStruct.UART_Count == 0x04)
				{
					UART_HandleStruct.UART_Cmd_len = UART_HandleStruct.UART_Buf[2]*256+  UART_HandleStruct.UART_Buf[3]; 	

				}
				if(UART_HandleStruct.UART_Count ==  (UART_HandleStruct.UART_Cmd_len + 4))
				{
					UART_HandleStruct.Package_Flag = 1;
					UART_HandleStruct.UART_Flag1 = 0;
					UART_HandleStruct.UART_Flag2 = 0;
				}	
			}
		}/*if(UART_HandleStruct.UART_Flag1 ==0)*/
}/*function end*/

void uart_rx_callback(uint32_t instance, void * uartState)
{
	uart_state_t *state = (uart_state_t*)uartState;
	//call datapack.
	//Buff[cnt] = *(state->rxBuff);
	GOKIT_UART_PackData(*(state->rxBuff));
}
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
		/*Then you can register callback func.*/
		UART_DRV_InstallRxCallback(GOKIT_UART_INSTANCE,
                                              uart_rx_callback,
                                              Buff,
                                              &uartState,
                                              true);

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
			
		UART_DRV_InstallRxCallback(GOKIT_UART_INSTANCE,
                                              uart_rx_callback,
                                              &rxChar,
                                              &uartState,
                                              true);

    while(true)
    {
			
			if(UART_HandleStruct.Package_Flag ==1)
        //UART_DRV_SendDataBlocking(GOKIT_UART_INSTANCE, Buff, 10, 1000u); 
			{
				UART_HandleStruct.Package_Flag = 0;
					PRINTF("%d \r\n",UART_HandleStruct.Message_Len);
				PRINTF("%d \r\n",UART_HandleStruct.UART_Cmd_len);
			}
			
    }
}
void UART2_Send_DATA(uint8_t data)
{
	UART_DRV_SendDataBlocking(GOKIT_UART_INSTANCE, &data, 1, 1000u);
}
void UART1_Send_DATA(uint8_t data)
{
	PRINTF("%d",data);
}
/*EOF*/
