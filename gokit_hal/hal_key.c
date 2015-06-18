/**
  ******************************************************************************
  * @file    Hal_key.c
  * @author  Chay
	* @QQ      917581001 
	* @email 	 guo8113@126.com
  * @version V1.0
  * @date    15/06/2015
  * @brief   1，专业嵌入式智能设备方案定制
						 2，承接Freescale,NXP,ST Cortex M项目，及TI DM816x 系列项目
  ******************************************************************************
  */ 
#include "hal_key.h"
#include "hal_rgb_led.h"
#include "hal_motor.h"
#include "fsl_debug_console.h"	//for debug

//extern Pro_CommonCmdTypeDef         	 Pro_CommonCmdStruct;
//extern Pro_CommonCmdTypeDef     		   Pro_M2WResetCmdStruct;    //MCU控制WIFI重置
//extern Pro_D2W_ConfigWifiTypeDef       Pro_D2WConfigCmdStruct;   //MCU配置WiFi模块
//extern Device_ReadTypeDef               Device_ReadStruct;
//extern Device_WirteTypeDef   					  Device_WirteStruct;

extern uint8_t									     	 SN; 
uint8_t KeyCountTime;


/*******************************************************************************
* Function Name  : KEY_GPIO_Init
* Description    : Configure GPIO Pin
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void KEY_GPIO_Init(void)
{
	gpio_input_pin_user_config_t keyPin=
	{
		.pinName = kGokitKey1,
		.config.isPullEnable = true,
    .config.pullSelect = kPortPullUp,				//ipu
    .config.isPassiveFilterEnabled = false,
    .config.interrupt = kPortIntDisabled
	};
	GPIO_DRV_InputPinInit(&keyPin);
	keyPin.pinName = kGokitKey2;
	GPIO_DRV_InputPinInit(&keyPin);
	keyPin.pinName = kGokitKey3;
	GPIO_DRV_InputPinInit(&keyPin);
}

/*******************************************************************************
* Function Name  : Get_Key
* Description    : Read the KEY state
* Input          : None
* Output         : None
* Return         : uint8_t KEY state
* Attention		 : 2015/6/12
*******************************************************************************/

uint8_t Get_Key(void) 
{
	uint8_t ReadKey;
	
	if(!GPIO_DRV_ReadPinInput(kGokitKey1))
	{
			ReadKey |= PRESS_KEY1;
	}	
	if(!GPIO_DRV_ReadPinInput(kGokitKey2))
	{
			ReadKey |= PRESS_KEY2;
	}
	if(!GPIO_DRV_ReadPinInput(kGokitKey3))
	{
			ReadKey |= PRESS_KEY3;
	}
  
		return ReadKey;
}


/*******************************************************************************
* Function Name  : ReadKeyValue
* Description    : Read the KEY value
* Input          : None
* Output         : None
* Return         : uint8_t KEY value
* Attention		 : None
*******************************************************************************/
uint8_t ReadKeyValue(void)
{
	static uint8_t Key_Check;
	static uint8_t Key_State;
	static uint16_t Key_LongCheck;
	static uint8_t Key_Prev    = 0;        							//上一次按键   
	
	uint8_t Key_press;
	uint8_t Key_return = 0;
	
	if(KeyCountTime >= 10)  //KeyCountTime 1MS+1  按键消抖10MS
	{
		KeyCountTime = 0;
		Key_Check = 1;
	}
	if(Key_Check == 1)
	{	
		Key_Check = 0;
		Key_press = Get_Key();
		switch (Key_State)
		{
			case 0:
				if(Key_press != 0)
				{
					Key_Prev = Key_press;
					Key_State = 1;
				}
					
				break;
			case 1:
				if(Key_press == Key_Prev) 
				{
					Key_State = 2;
					Key_return= Key_Prev | KEY_DOWN;
				}
			  else 																					//按键抬起,是抖动,不响应按键 
				{
					Key_State = 0;
				}
				break;
			case 2:

				if(Key_press != Key_Prev)
				{					
					Key_State = 0;
					Key_LongCheck = 0;
					Key_return = Key_Prev | KEY_UP;
					return Key_return;				
				}

				if(Key_press == Key_Prev) 
				{
					Key_LongCheck++;
					if(Key_LongCheck >= 100)    //长按2S
					{			
						Key_LongCheck = 0;
						Key_State = 3;
						Key_return= Key_press |  KEY_LONG;
						return Key_return;	
					}				
				}
				break;
				
				case 3:
				if(Key_press != Key_Prev)
				{
					Key_State = 0;
				}
				break;	
		}
	}
	
	return  NO_KEY;
}
/*******************************************************************************
* Function Name  : KEY_Handle
* Description    : Key processing function
* Input          : None
* Output         : None
* Return         : None
* Attention		 	 : None
*******************************************************************************/
void KEY_Handle(void)
{
	uint8_t Key_return =0;
	
	Key_return = ReadKeyValue();
	
	if(Key_return & KEY_UP)
	{
		if(Key_return & PRESS_KEY1)
		{
			PRINTF("KEY1 PRESS\r\n");
//			LED_RGB_Control(0,0,10);
			Motor_status(0);
			
		}
		if(Key_return & PRESS_KEY2)
		{
			PRINTF("KEY2 PRESS\r\n");
//			LED_RGB_Control(0,10,0);
			Motor_status(1);
		}
		if(Key_return & PRESS_KEY3)
		{
			PRINTF("KEY3 PRESS\r\n");
//			LED_RGB_Control(10,0,00);
//			DHT11_Read_Data(&Device_ReadStruct.Temperature, &Device_ReadStruct.Humidity);
//			printf("Temperature =%d;Humidity = %d " ,Device_ReadStruct.Temperature,Device_ReadStruct.Humidity);
			LED_RGB_Control(0,0,0);
  		Motor_status(6);
			
		}					
	}

	if(Key_return & KEY_LONG)
	{
		if(Key_return & PRESS_KEY1)
		{
			KEY_LongHandle(PRESS_KEY1);
			PRINTF("KEY1 Long PRESS\r\n");
			PRINTF("M2W RESET WIFI\r\n");
		}
		if(Key_return & PRESS_KEY2)
		{
			KEY_LongHandle(PRESS_KEY2);
			PRINTF("KEY2 Long PRESS\r\n");
			PRINTF("M2W  Config WIFI\r\n");
		}
		if(Key_return & PRESS_KEY3)
		{
			PRINTF("KEY3 Long PRESS\r\n");
		}					
	}
}		
/*******************************************************************************
* Function Name  : KEY_LongHandle
* Description    : Key Long Press processing function
                   
* Input          : KeyAction :
														  RESS_KEY1 ->MCU To WIFI Send Reset command
                              PRESS_KEY2 ->MCU To WIFI Send Config command
* Output         : None
* Return         : None
* Attention		 	 : None
*******************************************************************************/	
void KEY_LongHandle(uint8_t KeyAction)
{
	if(KeyAction  == KEY1_Long_Action)
	{
//		Device_WirteStruct.LED_R = 50;
//		Device_WirteStruct.LED_G = 0;
//		Device_WirteStruct.LED_B = 0;
//		LED_RGB_Control(Device_WirteStruct.LED_R,Device_WirteStruct.LED_G,Device_WirteStruct.LED_B);
//		
//		Pro_M2WResetCmdStruct.Pro_HeadPart.SN = SN++;
//		Pro_M2WResetCmdStruct.Sum = CheckSum((uint8_t *)&Pro_M2WResetCmdStruct, sizeof(Pro_M2WResetCmdStruct));
//		Pro_UART_SendBuf((uint8_t *)&Pro_M2WResetCmdStruct,sizeof(Pro_M2WResetCmdStruct), 0);	

			LED_RGB_Control(100,10,20);
	}

	if(KeyAction  == KEY2_Long_Action)
	{
//		Device_WirteStruct.LED_R = 0;
//		Device_WirteStruct.LED_G = 0;
//		Device_WirteStruct.LED_B = 50;
//		LED_RGB_Control(Device_WirteStruct.LED_R,Device_WirteStruct.LED_G,Device_WirteStruct.LED_B);		
//		
//		Pro_D2WConfigCmdStruct.Pro_HeadPart.SN = SN++;
//		Pro_D2WConfigCmdStruct.Config_Method = 0x02;
//		Pro_D2WConfigCmdStruct.Sum = CheckSum((uint8_t *)&Pro_D2WConfigCmdStruct, sizeof(Pro_D2WConfigCmdStruct));
//		Pro_UART_SendBuf((uint8_t *)&Pro_D2WConfigCmdStruct,sizeof(Pro_D2WConfigCmdStruct), 0);	
					LED_RGB_Control(0,0,100);

	}
	
}	





