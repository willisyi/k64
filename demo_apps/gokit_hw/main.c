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
}
int main (void)
{
    // RX buffers
    //! @param receiveBuff Buffer used to hold received data
    // Initialize standard SDK demo application pins
    hardware_init();
		LED1_EN;
		
		gokit_hal_init();
    // Initialize LED1
		
    // Print the initial banner
	PRINTF("\r\nHello World!clock:%d\n\n\r",SystemCoreClock);

	int i=0;
		//UARTx_test();
	int j=0;
    while(1)
    {
			
			// Main routine that simply echoes received characters forever
				uint8_t temp,hum=0;
			DHT11_Read_Data(&temp,&hum);
		PRINTF("Temp and hum %d %d\r\n",temp,hum);
			for(j=0;j<5;j++)
			Delay_ms(200);	
//			for(j=0;j<10;j++)
//			Delay_us(50000);
			PRINTF("\r\n1000ms....%d\r\n",++i);
	//		PRINTF("\r\n500ms....\r\n");
        // First, get character
       // receiveBuff = GETCHAR();
//			KEY_Handle();					
        // Now echo the received character
       // PUTCHAR(receiveBuff);
    }
}

