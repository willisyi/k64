#include "delay.h"
#include "fsl_debug_console.h"

static uint8_t  fac_us=0;																		//us延时倍乘数
static uint32_t fac_ms=0;																		//ms延时倍乘数

void Delay_Init(uint8_t SYSCLK)
{
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	SysTick->CTRL = ((SysTick->CTRL & ~SysTick_CTRL_CLKSOURCE_Msk)
                                     | 1<< SysTick_CTRL_CLKSOURCE_Pos);
	fac_us=SYSCLK; //硬件1分频,fac_us得出的值是要给下面的时钟函数用的  
	fac_ms =fac_us*1000; 	
}	

		    								   
int Delay_us(uint32_t nus)
{		
		uint32_t temp;  
		temp = nus*fac_us-1;  //延时10us的话就是  10*120=120,装到load寄存器中 
		if(temp > 0xFFFFFF)	//SysTick是24位的计数器，检查是否越界。max us 139810us
			return -1;
		SysTick->LOAD = temp;
		SysTick->VAL=0U;//计数器清0,因为currrent字段被手动清零时,load将自动重装到VAL中  
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk |SysTick_CTRL_CLKSOURCE_Msk;
		//SysTick->CTRL = 0x5U;
 
		while(!(SysTick->CTRL &(1<<16))); //查询  	
		SysTick->CTRL = 0x4U;  //关闭计数器  
		SysTick->VAL = 0U;   //清空val      																//清空计数器	 
		return 0;
}



void Delay_ms(uint16_t nms)		//max delay ms = 139
{	 		  	  
		SysTick->LOAD =nms*fac_ms-1;
		SysTick->VAL=0U;//计数器清0,因为currrent字段被手动清零时,load将自动重装到VAL中  
		//SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk |SysTick_CTRL_CLKSOURCE_Msk;
		SysTick->CTRL = 0x5U;
		while(!(SysTick->CTRL &(1<<16))); //查询  	
		SysTick->CTRL = 0x4U;  //关闭计数器  
		SysTick->VAL = 0U;   //清空val      																//清空计数器	 
} 
