/**
  ******************************************************************************
  * @file    Hal_infrared.c
  * @author  jason
	* @Tel     18938045680
	* @QQ      570526723 
  * @version V2.0
  * @date    12/05/2014
  * @brief   1，专业嵌入式智能设备方案定制
						 2，专业开发工具供应商：http://wenjieteam.taobao.com/
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, jason SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2014 jason</center></h2>
  */ 
	
#include "hal_infrared.h"
#include "protocol.h"
#include "fsl_debug_console.h"

extern Device_ReadTypeDef        Device_ReadStruct;
bool preStatus;
void IR_Init(void)
{	
  gpio_input_pin_user_config_t infPin=
	{
		.pinName = kInfPin,
		.config.isPullEnable = true,
    .config.pullSelect = kPortPullUp,				//ipu
    .config.isPassiveFilterEnabled = false,
    //.config.interrupt = kPortIntFallingEdge
		.config.interrupt = kPortIntDisabled
	};
	GPIO_DRV_InputPinInit(&infPin);
		
}


void IR_Handle(void)
{
	if(GPIO_DRV_ReadPinInput(kInfPin))
	{
		Device_ReadStruct.Infrared = false;
	}
	else
	{		
		Device_ReadStruct.Infrared = true;
	}
	//如果状态改变，立即发送状态。
	if(Device_ReadStruct.Infrared != preStatus)
	{
		preStatus = Device_ReadStruct.Infrared;
		PRINTF("Infrared_EXTI...\r\n");
		Pro_D2W_ReportDevStatusHandle();
	}
}




