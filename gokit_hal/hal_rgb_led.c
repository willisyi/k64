/**
  ******************************************************************************
  * @file    Hal_OLED.c
  * @author  guo8113
	* @QQ      917581001 
	* @email 	 guo8113@126.com
  * @version V1.0
  * @date    15/06/2015
  * @brief   1，专业嵌入式智能设备方案定制
						 2，承接Freescale,NXP,ST Cortex M项目
  ******************************************************************************
  */ 
#include "hal_rgb_led.h"

void LED_delay(unsigned int  ms)
{
	volatile unsigned  int i=0;
     for(i=0;i<ms;i++);
  
}


/************ generation clock *********************/
void ClkProduce(void)
{  
  SCL_LOW;    // SCL=0
	LED_delay(40);
  SCL_HIGH;     // SCL=1
  LED_delay(40);  
}
 

/**********  send 32 zero ********************/
void Send32Zero(void)
{
  unsigned char i; 
	SDA_LOW;   // SDA=0
	for (i=0; i<32; i++)
		ClkProduce();
}


 /********* invert the grey value of the first two bits ***************/
uint8_t TakeAntiCode(uint8_t dat)
{
    uint8_t tmp = 0;

	tmp=((~dat) & 0xC0)>>6;		 
	return tmp;
}
 

/****** send gray data *********/
void DatSend(uint32_t dx)
{
    uint8_t i;
 
	for (i=0; i<32; i++)
	{
	    if ((dx & 0x80000000) != 0)
		{
	        
	        SDA_HIGH;     //  SDA=1; 
		}
		else
		{
             SDA_LOW;    //  SDA=0; 
		}
 
		dx <<= 1;
    ClkProduce();
	}
}
 
/******* data processing  ********************/
void DataDealWithAndSend(uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t dx = 0;
 
    dx |= (uint32_t)0x03 << 30;             // The front of the two bits 1 is flag bits
    dx |= (uint32_t)TakeAntiCode(g) << 28;
    dx |= (uint32_t)TakeAntiCode(b) << 26;	
    dx |= (uint32_t)TakeAntiCode(r) << 24;
 
    dx |= (uint32_t)g << 16;
    dx |= (uint32_t)b << 8;
    dx |= r;
 
    DatSend(dx);
}

void RGB_LED_Init(void)
{
	gpio_output_pin_user_config_t outPin={
		.pinName    = kSCL,
    .config.outputLogic = 1,	//default output logic.
    .config.slewRate = kPortSlowSlewRate,
    .config.isOpenDrainEnabled = false,
    .config.driveStrength = kPortLowDriveStrength
	};
	GPIO_DRV_OutputPinInit(&outPin);
	outPin.pinName = kSDA;
	GPIO_DRV_OutputPinInit(&outPin);
	
 	Send32Zero();	
	DataDealWithAndSend(0,0,0);	  // display red
  DataDealWithAndSend(0,0,0);	

}

void LED_RGB_Control(uint8_t R, uint8_t G, uint8_t B)
{
	Send32Zero();
	DataDealWithAndSend(R, G, B);	  // display red
  DataDealWithAndSend(R, G, B);	  // display red
//	printf("LED_R = %d LED_G = %d LED_B = %d",R,G,B);
}



