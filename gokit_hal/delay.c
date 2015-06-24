#include "delay.h"
#include "fsl_debug_console.h"

static uint8_t  fac_us=0;																		//us延时倍乘数
static uint32_t fac_ms=0;																		//ms延时倍乘数
extern void SysTick_Handler(void);


void SysTick_Handler(void)
{	
	//PRINTF("SysTick_Handler \r\n");
	//TimingDelay_Decrement(); 
	SysTick->VAL =0U; 
}
void Delay_Init(uint8_t SYSCLK)
{
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	SysTick->CTRL = ((SysTick->CTRL & ~SysTick_CTRL_CLKSOURCE_Msk)
                                     | 1<< SysTick_CTRL_CLKSOURCE_Pos);
	fac_us=SYSCLK; //硬件1分频,fac_us得出的值是要给下面的时钟函数用的  
	fac_ms =fac_us*1000; 	
}	

		    								   
void Delay_us(uint32_t nus)
{		
	Delay_Init(120);
		uint32_t temp;  
		SysTick->LOAD = nus*fac_us;  //延时10us的话就是  10*9=90,装到load寄存器中  
		SysTick->VAL=0U;//计数器清0,因为currrent字段被手动清零时,load将自动重装到VAL中  
		//SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk |SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
		do  
		{  
			 temp = SysTick->CTRL;  //时间到了之后,该位将被硬件置1,但被查询后自动清0  
		}  
		while(temp & 0x01 && !(temp &(1<<16))); //查询  	
		SysTick->CTRL = 0U;  //关闭计数器  
		SysTick->VAL = 0U;   //清空val      																//清空计数器	 
}


void Delay_ms(uint16_t nms)
{	 		  	  
	Delay_Init(120);	
	uint32_t temp;  
		SysTick->LOAD = nms*fac_ms;  //延时10us的话就是  10*9=90,装到load寄存器中  
		SysTick->VAL=0U;//计数器清0,因为currrent字段被手动清零时,load将自动重装到VAL中  
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
		do  
		{  
			 temp = SysTick->CTRL;  //时间到了之后,该位将被硬件置1,但被查询后自动清0  
		}  
		while(temp & 0x01 && !(temp &(1<<16))); //查询  
	
		SysTick->CTRL =0U;  //关闭计数器  
		SysTick->VAL = 0U;   //清空val   	  	    
} 
