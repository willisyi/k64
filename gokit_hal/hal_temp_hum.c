/**
  ******************************************************************************
  * @file    Hal_hemp_hum.c
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
#include "hal_temp_hum.h"
#include "delay.h"

																		
#define	DHT11_DQ_OUT(n)  GPIO_DRV_WritePinOutput(kGokitDHT,n)
#define	DHT11_DQ_IN   	 GPIO_DRV_ReadPinInput(kGokitDHT)

//Set GPIO Direction
static void DHT11_IO_OUT()
{
		gpio_output_pin_user_config_t dthoutPin={   
													.pinName = kGokitDHT,											
													.config.outputLogic = 0,									
													.config.slewRate = kPortSlowSlewRate,			
													.config.isOpenDrainEnabled = false,				
													.config.driveStrength = kPortLowDriveStrength 
											}; 																						
	GPIO_DRV_OutputPinInit(&dthoutPin);	
}
static void DHT11_IO_IN()
{
			gpio_input_pin_user_config_t dthinPin={   
						.pinName = kGokitDHT,							     
						.config.isPullEnable = true,				    
						.config.pullSelect = kPortPullUp,		    	
						.config.isPassiveFilterEnabled = false, 
					  .config.interrupt = kPortIntDisabled    
			}; 																					
		GPIO_DRV_InputPinInit(&dthinPin);					
}
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 											//SET OUTPUT
  DHT11_DQ_OUT(0); 											//GPIOA.0=0
  Delay_ms(20);    											//Pull down Least 18ms
  DHT11_DQ_OUT(1); 											//GPIOA.0=1 
	Delay_us(30);     										//Pull up 20~40us
}

uint8_t DHT11_Check(void) 	   
{   
	uint8_t retry=0;
	DHT11_IO_IN();												//SET INPUT	 
  while (DHT11_DQ_IN&&retry<100)				//DHT11 Pull down 40~80us
	{
		retry++;
		Delay_us(1);
	}	 
	
	if(retry>=100)
		return 1;
	else 
		retry=0;
	
  while (!DHT11_DQ_IN&&retry<100)				//DHT11 Pull up 40~80us
	{
		retry++;
		Delay_us(1);
	}
	
	if(retry>=100)
		return 1;														//chack error	    
	
	return 0;
}

uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(DHT11_DQ_IN&&retry<100)					//wait become Low level
	{
		retry++;
		Delay_us(1);
	}
	
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)				//wait become High level
	{
		retry++;
		Delay_us(1);
	}
	
	Delay_us(40);//wait 40us
	
	if(DHT11_DQ_IN)
		return 1;
	else 
		return 0;		   
}

uint8_t DHT11_Read_Byte(void)    
{        
  uint8_t i,dat;
  dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
  }						    
  
	return dat;
}

uint8_t DHT11_Read_Data(uint8_t *temperature,uint8_t *humidity)    
{        
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humidity=buf[0];
			*temperature=buf[2];
		}
	}
	else 
		return 1;
	
	return 0;	    
}
	 
uint8_t DHT11_Init(void)
{	 			 
 	DHT11_IO_OUT();
	DHT11_DQ_OUT(1);		//must be 1,then change to 0 to RST.
			    
	DHT11_Rst();  
	return DHT11_Check();
} 
/*EOF*/