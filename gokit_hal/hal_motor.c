/**
  ******************************************************************************
  * @file    Hal_motor.c
  * @author  guo8113
	* @QQ      917581001 
	* @email 	 guo8113@126.com
  * @version V1.0
  * @date    15/06/2015
  * @brief   1，专业嵌入式智能设备方案定制
						 2，承接Freescale,NXP,ST Cortex M项目
  ******************************************************************************
  * @copy
  *
  */ 
#include "hal_motor.h"

//FTM 8KHZ
ftm_pwm_param_t ftmParam = {
        .mode                   = kFtmEdgeAlignedPWM,
        .edgeMode               = kFtmLowTrue,
        .uFrequencyHZ           = 8000u,
        .uDutyCyclePercent      = 0,
        .uFirstEdgeDelayPercent = 0,
    };
ftm_user_config_t ftmInfo;
void FTM_PWM_Set_Start(uint32_t dutyCyclePercent)
{
	ftmParam.uDutyCyclePercent = dutyCyclePercent;
		// Start PWM mode with updated duty cycle.
  FTM_DRV_PwmStart(GOKIT_FTM_INSTANCE, &ftmParam, GOKIT_FTM_CHANNEL);
 // Software trigger to update registers.
  FTM_HAL_SetSoftwareTriggerCmd(g_ftmBase[GOKIT_FTM_INSTANCE], true);
}
void Motor_Init(void)
{
		gpio_output_pin_user_config_t outPin={
		.pinName    = kMot1,
    .config.outputLogic = 1,	//default output logic.
    .config.slewRate = kPortSlowSlewRate,
    .config.isOpenDrainEnabled = false,
    .config.driveStrength = kPortLowDriveStrength
	};
	GPIO_DRV_OutputPinInit(&outPin);
	
	//set as FTM0 CH7 output
	PORT_HAL_SetMuxMode(PORTA,2,kPortMuxAlt3);  	
	
	//Use FTM PWM
	memset(&ftmInfo, 0, sizeof(ftmInfo));
  ftmInfo.syncMethod = kFtmUseSoftwareTrig;
  FTM_DRV_Init(GOKIT_FTM_INSTANCE, &ftmInfo);
  FTM_DRV_SetClock(GOKIT_FTM_INSTANCE, kClock_source_FTM_SystemClk, kFtmDividedBy1);
	FTM_PWM_Set_Start(0);

}
void Motor_status(uint16_t status)
{		
	if(status == 5)
	{
		GPIO_DRV_ClearPinOutput(kMot1);
		//GPIO_DRV_ClearPinOutput(kMot2);
		FTM_PWM_Set_Start(0);
	}
	else if (status > 5)
	{
		GPIO_DRV_ClearPinOutput(kMot1);
		FTM_PWM_Set_Start( (10-status)*10 );	//only 40% strength used.
	}
	else if (status < 5)
	{
		GPIO_DRV_SetPinOutput(kMot1);
		FTM_PWM_Set_Start( status*10 );
	}

}
