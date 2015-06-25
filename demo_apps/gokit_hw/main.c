/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////
// SDK Included Files
#include "board.h"
#include "fsl_pit_driver.h"
#include "fsl_uart_driver.h"
#include "fsl_debug_console.h"
#include "fsl_port_hal.h"
#include "gokit_hal/hal_key.h"
#include "gokit_hal/hal_rgb_led.h"
#include "gokit_hal/hal_motor.h"
#include "gokit_hal/hal_uart.h"
#include "gokit_hal/protocol.h"
#include "gokit_hal/delay.h"
#include "gokit_hal/hal_temp_hum.h"
#include "gokit_hal/hal_infrared.h"
////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define PIT_PERIOD    1000U	//PIT frequence time in us
#define PIT_INSTANCE   0U
////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
//#define GOKIT_UART_BAUD  9600U
//#define GOKIT_UART_INSTANCE 3U

UART_HandleTypeDef         				UART_HandleStruct;
Pro_Wait_AckTypeDef           	  Wait_AckStruct;
Device_WirteTypeDef   					  Device_WirteStruct;    		
Device_ReadTypeDef                Device_ReadStruct; 

Pro_M2W_ReturnInfoTypeDef  				Pro_M2W_ReturnInfoStruct;
Pro_CommonCmdTypeDef      			 	Pro_CommonCmdStruct;
Pro_W2D_WifiStatusTypeDef     	 	Pro_W2D_WifiStatusStruct;     
Pro_CommonCmdTypeDef     	 				Pro_M2WResetCmdStruct;    		
Pro_D2W_ConfigWifiTypeDef       	Pro_D2WConfigCmdStruct;   		
Pro_D2W_ReportDevStatusTypeDef   	Pro_D2W_ReportStatusStruct;
KEY_StatusTypeDef 								KEY_Status;
uint8_t									 					SN;    

uint32_t SystemTimeCount = 0;
uint32_t ReportTimeCount = 0;

uint8_t Set_LedStatus = 0;

void PIT0_CH0_Init()
{
	    // LPTMR configurations
    pit_user_config_t chn0Confg = {
        .isInterruptEnabled = true,
        .periodUs = PIT_PERIOD
    };
  // Init pit module and enable run in debug
    PIT_DRV_Init(0, false);

    // Initialize PIT timer instance for channel 0 and 1
    PIT_DRV_InitChannel(PIT_INSTANCE, 0, &chn0Confg);
    PIT_DRV_StartTimer(PIT_INSTANCE, 0);

}
void gokit_hal_init()
{
	PIT0_CH0_Init();	
	KEY_GPIO_Init();
	RGB_LED_Init();
	Motor_Init();
	UARTx_Init();
	Delay_Init(120);	//120MHZ
	DHT11_Init();
	IR_Init();
}
void McuStatusInit(void)
{
	PRINTF("UART_HandleStruct Init...\r\n");
	memset(&UART_HandleStruct, 0, sizeof(UART_HandleStruct));
	memset(&Pro_CommonCmdStruct, 0, sizeof(Pro_CommonCmdStruct));
	Pro_CommonCmdStruct.Pro_HeadPart.Head[0] = 0XFF;
	Pro_CommonCmdStruct.Pro_HeadPart.Head[1] = 0XFF;	
	
	PRINTF("Pro_CommonCmdStruct Init...\r\n");
	memset(&Pro_CommonCmdStruct, 0, sizeof(Pro_CommonCmdStruct));
	Pro_CommonCmdStruct.Pro_HeadPart.Head[0] = 0XFF;
	Pro_CommonCmdStruct.Pro_HeadPart.Head[1] = 0XFF;	
	
	
	memset(&Pro_M2W_ReturnInfoStruct, 0, sizeof(Pro_M2W_ReturnInfoStruct));
	Pro_M2W_ReturnInfoStruct.Pro_HeadPart.Head[0] = 0XFF; 
	Pro_M2W_ReturnInfoStruct.Pro_HeadPart.Head[1] = 0XFF;
	Pro_M2W_ReturnInfoStruct.Pro_HeadPart.Len = exchangeBytes(sizeof(Pro_M2W_ReturnInfoStruct) - 4);
	Pro_M2W_ReturnInfoStruct.Pro_HeadPart.Cmd = Pro_D2W__GetDeviceInfo_Ack_Cmd;
	memcpy(Pro_M2W_ReturnInfoStruct.Pro_ver, PRO_VER, strlen(PRO_VER));
	memcpy(Pro_M2W_ReturnInfoStruct.P0_ver, P0_VER, strlen(P0_VER));
	memcpy(Pro_M2W_ReturnInfoStruct.Hard_ver, HARD_VER, strlen(HARD_VER));
	memcpy(Pro_M2W_ReturnInfoStruct.Soft_ver, SOFT_VER, strlen(SOFT_VER));
	memcpy(Pro_M2W_ReturnInfoStruct.Product_Key, PRODUCT_KEY, strlen(PRODUCT_KEY));
	Pro_M2W_ReturnInfoStruct.Binable_Time = exchangeBytes(0);;		
	
	PRINTF("Pro_D2W_ReportStatusStruct Init...\r\n");
	memset(&Pro_D2W_ReportStatusStruct, 0, sizeof(Pro_D2W_ReportStatusStruct));
	Pro_D2W_ReportStatusStruct.Pro_HeadPartP0Cmd.Pro_HeadPart.Head[0] = 0XFF;
	Pro_D2W_ReportStatusStruct.Pro_HeadPartP0Cmd.Pro_HeadPart.Head[1] = 0XFF;            
	Pro_D2W_ReportStatusStruct.Pro_HeadPartP0Cmd.Pro_HeadPart.Len = exchangeBytes(sizeof(Pro_D2W_ReportStatusStruct) - 4);
	Pro_D2W_ReportStatusStruct.Pro_HeadPartP0Cmd.Pro_HeadPart.Cmd = Pro_D2W_P0_Cmd;
	
	PRINTF("Pro_M2WResetCmdStruct Init...\r\n");
	memset(&Pro_M2WResetCmdStruct, 0, sizeof(Pro_M2WResetCmdStruct));
	Pro_M2WResetCmdStruct.Pro_HeadPart.Head[0] = 0XFF;
	Pro_M2WResetCmdStruct.Pro_HeadPart.Head[1] = 0XFF;
	Pro_M2WResetCmdStruct.Pro_HeadPart.Len = exchangeBytes(sizeof(Pro_M2WResetCmdStruct) - 4);
	Pro_M2WResetCmdStruct.Pro_HeadPart.Cmd = Pro_D2W_ResetWifi_Cmd;
	
	PRINTF("Pro_D2WConfigCmdStruct Init...\r\n");
	memset(&Pro_D2WConfigCmdStruct, 0, sizeof(Pro_D2WConfigCmdStruct));
	Pro_D2WConfigCmdStruct.Pro_HeadPart.Head[0] = 0XFF;
	Pro_D2WConfigCmdStruct.Pro_HeadPart.Head[1] = 0XFF;
	Pro_D2WConfigCmdStruct.Pro_HeadPart.Len = exchangeBytes(sizeof(Pro_D2WConfigCmdStruct) - 4);
	Pro_D2WConfigCmdStruct.Pro_HeadPart.Cmd = Pro_D2W_ControlWifi_Config_Cmd;	

	PRINTF("Device_WirteStruct Init...\r\n");
	memset(&Device_WirteStruct, 0, sizeof(Device_WirteStruct));
	Device_WirteStruct.Motor = exchangeBytes(0x05);   //¹Ø±Õµç»ú
	
	PRINTF("Device_ReadStruct Init...\r\n");
	memset(&Device_ReadStruct, 0, sizeof(Device_ReadStruct));
	
	PRINTF("KEY_Status Init...\r\n");
	memset(&KEY_Status, 0, sizeof(KEY_Status));
	
}
void PRINTF_DevStatus(void)
{
	//PRINTF_TimeDisplay();
	
	PRINTF("RGB LED R=%d,G=%d,B=%d; Motor=%d; Humidity=%d,Temperature=%d; Infrared=%d\r\n",
	Pro_D2W_ReportStatusStruct.Device_All.Device_Wirte.LED_R,Pro_D2W_ReportStatusStruct.Device_All.Device_Wirte.LED_G,Pro_D2W_ReportStatusStruct.Device_All.Device_Wirte.LED_B,
	exchangeBytes(Pro_D2W_ReportStatusStruct.Device_All.Device_Wirte.Motor),
	Pro_D2W_ReportStatusStruct.Device_All.Device_Read.Humidity,
	Pro_D2W_ReportStatusStruct.Device_All.Device_Read.Temperature,
	Pro_D2W_ReportStatusStruct.Device_All.Device_Read.Infrared);
	PRINTF("------------------------------ReportDevStatus-------------------------------------\r\n");
}
void ReportDevStatusHandle(void)
{
	uint8_t Device_Status = 0;
	if(ReportTimeCount >= 3000)
	{
		ReportTimeCount = 0;
		Device_Status = memcmp(&Device_ReadStruct,&Pro_D2W_ReportStatusStruct.Device_All.Device_Read, sizeof(Device_ReadStruct));
		if(Device_Status)
		{
			Pro_D2W_ReportDevStatusHandle();
		}	
	}
}


int main (void)
{
    hardware_init();
		//LED1_EN;		
		gokit_hal_init();
		McuStatusInit();
    // Initialize LED1
		
	//	UARTx_test();//while(1) ,test uart3

    while(1)
    {
		MessageHandle();
		KEY_Handle();		
		IR_Handle();
		DHT11_Read_Data(&Device_ReadStruct.Temperature, &Device_ReadStruct.Humidity);
		ReportDevStatusHandle();
    }
}

