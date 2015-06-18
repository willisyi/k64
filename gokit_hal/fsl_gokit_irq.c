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
#include "fsl_uart_hal.h"
#include "protocol.h"
/******************************************************************************
 * Code
 *****************************************************************************/
/******************************************************************************/
/*             KEY1 :长按  RESET WIFI, KEY2 :短按  配置WiFi联                 */
/*             定时器3中断服务程序                                            */
/******************************************************************************/
//extern Pro_Wait_AckTypeDef           	  Wait_AckStruct;
//extern uint32_t                         SystemTimeCount ;
extern uint8_t 													KeyCountTime;
//extern uint32_t ReportTimeCount;
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
            KeyCountTime++;
        }
    }
}
#else

#if (FSL_FEATURE_PIT_TIMER_COUNT > 0U)
void PIT0_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_HAL_ClearIntFlag(g_pitBase[0], 0U);
    KeyCountTime++;
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


extern void * g_uartStatePtr[UART_INSTANCE_COUNT];
//#define GOKIT
#ifdef GOKIT
extern UART_HandleTypeDef  							UART_HandleStruct;
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
#endif
/*private function used by UART3 ISR copyed from fsl_uart_driver.c */
static void UART_DRV_CompleteSendData(uint32_t instance)
{
    assert(instance < UART_INSTANCE_COUNT);

    UART_Type * base = g_uartBase[instance];
    uart_state_t * uartState = (uart_state_t *)g_uartStatePtr[instance];

    /* Disable the transmitter data register empty interrupt */
    UART_BWR_C2_TIE(base, 0U);

    /* Signal the synchronous completion object. */
    if (uartState->isTxBlocking)
    {
        OSA_SemaPost(&uartState->txIrqSync);
    }

    /* Update the information of the module driver state */
    uartState->isTxBusy = false;
}
/*private function used by UART3 ISR copyed from fsl_uart_driver.c */
static void UART_DRV_CompleteReceiveData(uint32_t instance)
{
    assert(instance < UART_INSTANCE_COUNT);

    uart_state_t * uartState = (uart_state_t *)g_uartStatePtr[instance];
    UART_Type * base = g_uartBase[instance];

    /* Disable receive data full and rx overrun interrupt */
    UART_BWR_C2_RIE(base, 0U);
    UART_HAL_SetIntMode(base, kUartIntRxOverrun, false);

    /* Signal the synchronous completion object. */
    if (uartState->isRxBlocking)
    {
        OSA_SemaPost(&uartState->rxIrqSync);
    }

    /* Update the information of the module driver state */
    uartState->isRxBusy = false;
}
/*customed ISR to deal with package generate */
void UART3_RX_TX_IRQHandler(void)
{
		uint32_t instance = 3;
    //UART_DRV_IRQHandler(3);
	  uart_state_t * uartState = (uart_state_t *)g_uartStatePtr[instance];
    UART_Type * base = g_uartBase[instance];

    /* Exit the ISR if no transfer is happening for this instance. */
    if ((!uartState->isTxBusy) && (!uartState->isRxBusy))
    {
        return;
    }

    /* Handle receive data register full interrupt, if rx data register full
     * interrupt is enabled AND there is data available. */
    if((UART_BRD_C2_RIE(base)) && (UART_BRD_S1_RDRF(base)))
    {
#if FSL_FEATURE_UART_HAS_FIFO
        /* Read out all data from RX FIFO */
        while(UART_HAL_GetRxDatawordCountInFifo(base))
        {
#endif
            /* Get data and put into receive buffer */
            UART_HAL_Getchar(base, uartState->rxBuff);
#ifdef GOKIT
					GOKIT_UART_PackData(*uartState->rxBuff);
#endif
            /* Invoke callback if there is one */
            if (uartState->rxCallback != NULL)
            {
                uartState->rxCallback(instance, uartState);
            }
            else
            {
                ++uartState->rxBuff;
                --uartState->rxSize;

                /* Check and see if this was the last byte */
                if (uartState->rxSize == 0U)
                {
                    UART_DRV_CompleteReceiveData(instance);
                    #if FSL_FEATURE_UART_HAS_FIFO
                    break;
                    #endif
                }
            }
#if FSL_FEATURE_UART_HAS_FIFO
        }
#endif
    }

    /* Handle transmit data register empty interrupt, if tx data register empty
     * interrupt is enabled AND tx data register is currently empty. */
    if((UART_BRD_C2_TIE(base)) && (UART_BRD_S1_TDRE(base)))
    {
        /* Check to see if there are any more bytes to send */
        if (uartState->txSize)
        {
            uint8_t emptyEntryCountInFifo;
#if FSL_FEATURE_UART_HAS_FIFO
            emptyEntryCountInFifo = uartState->txFifoEntryCount -
                                    UART_HAL_GetTxDatawordCountInFifo(base);
#else
            emptyEntryCountInFifo = uartState->txFifoEntryCount;
#endif
            while(emptyEntryCountInFifo--)
            {
                /* Transmit data and update tx size/buff */
                UART_HAL_Putchar(base, *(uartState->txBuff));

                /* Invoke callback if there is one */
                if (uartState->txCallback != NULL)
                {
                   /* The callback MUST set the txSize to 0 if the
                    * transmit is ended.*/
                   uartState->txCallback(instance, uartState);
                }
                else
                {
                    ++uartState->txBuff;
                    --uartState->txSize;
                }

                /* Check and see if this was the last byte */
                if (uartState->txSize == 0U)
                {
                    UART_DRV_CompleteSendData(instance);
                    break;
                }
            }
        }
    }

    /* Handle receive overrun interrupt */
    if (UART_HAL_GetStatusFlag(base, kUartRxOverrun))
    {
        /* Clear the flag, OR the rxDataRegFull will not be set any more */
        UART_HAL_ClearStatusFlag(base, kUartRxOverrun);
    }
} 


/*! @} */

/*******************************************************************************
 * EOF
 ******************************************************************************/

