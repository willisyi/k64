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
/**
  ******************************************************************************
  * @file    fsl_gokit_irq.c
  * @author  guo8113
	* @QQ      917581001 
	* @email 	 guo8113@126.com
  * @version V1.0
  * @date    15/06/2015
  * @brief   1，专业嵌入式智能设备方案定制
						 2，承接Freescale,NXP,ST Cortex M项目,TI DM816x项目
  ******************************************************************************
  * @copy
  *
  */ 
#include <stdlib.h>
#include <assert.h>
#include "fsl_pit_driver.h"
#include "fsl_uart_driver.h"
#include "fsl_uart_driver.h"
#include "fsl_gpio_driver.h"
#include "protocol.h"
#include "hal_infrared.h"
#include "fsl_debug_console.h"
/******************************************************************************
 * Code
 *****************************************************************************/

extern UART_HandleTypeDef  		          UART_HandleStruct;
extern Device_ReadTypeDef               Device_ReadStruct;
extern uint32_t 		                    ReportTimeCount;
extern Pro_Wait_AckTypeDef              Wait_AckStruct;
extern uint32_t                         SystemTimeCount ;
extern uint8_t 													KeyCountTime;

extern void UART_DRV_IRQHandler(uint32_t instance);
/*!
 * @brief System default IRQ handler defined in startup code.
 */
#if FSL_FEATURE_PIT_HAS_SHARED_IRQ_HANDLER
void PIT_IRQHandler(void)
{
    uint32_t i;
    for(i=0; i < 2; i++)  // We only support two channels for pit example
    {
        if (PIT_HAL_IsIntPending(g_pitBase[0], i))
        {
            /* Clear interrupt flag.*/
            PIT_HAL_ClearIntFlag(g_pitBase[0], i);
            SystemTimeCount++;
						Wait_AckStruct.SendTime ++;
						KeyCountTime++;
						ReportTimeCount++;
        }
    }
}
#else

#if (FSL_FEATURE_PIT_TIMER_COUNT > 0U)
void PIT0_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_HAL_ClearIntFlag(g_pitBase[0], 0U);
    SystemTimeCount++;
		Wait_AckStruct.SendTime ++;
		KeyCountTime++;
		ReportTimeCount++;
}
#endif

#if (FSL_FEATURE_PIT_TIMER_COUNT > 1U)
void PIT1_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_HAL_ClearIntFlag(g_pitBase[0], 1U);
   //TMOD
}
#endif

#endif /* FSL_FEATURE_PIT_HAS_SHARED_IRQ_HANDLER */


/*customed ISR to deal with package generate */
void UART3_RX_TX_IRQHandler(void)
{
		//uint32_t instance = 3;
    UART_DRV_IRQHandler(3);
}

/*//当使用中断方式时，会导致Uart_send 特别慢。
void PORTB_IRQHandler()
{
	GPIO_DRV_ClearPinIntFlag(kInfPin);
	PRINTF("Infrared_EXTI...\r\n");
	
	  if(GPIO_DRV_ReadPinInput(kInfPin))
		{
			Device_ReadStruct.Infrared = false;
		}
		else
		{		
			Device_ReadStruct.Infrared = true;
		}	
		Pro_D2W_ReportDevStatusHandle();
}
*/


/*******************************************************************************
 * EOF
 ******************************************************************************/

