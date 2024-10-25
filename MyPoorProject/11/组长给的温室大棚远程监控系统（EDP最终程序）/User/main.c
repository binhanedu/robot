/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
//USE_STDPERIPH_DRIVER, STM32F10X_HD, USE_STM3210B_EVAL

//==============EDP mini 温湿度 光强 上传ok
//==============增加灯的控制OK
#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "usart1.h"
#include "usart2.h"
#include "EdpKit.h"
#include "utils.h"
#include "esp8266.h"
#include "bh1750fvi.h"
#include "EdpDemo.h"
#include "led.h"
int main(void)
{
	

	  LED_Init();    //LED指示灯初始化函数
	  LED_Switch(LED_ON,LED_R|LED_G|LED_Y|LED_B);	   //点亮开发板四个指示灯，表示程序进入主流程
    /*初始化串口1 用于调试信息输出*/
    USART1_Init();
    /*初始化串口2 用于MT6331通信*/
    USART2_Init();
    mDelay(1000);
    /*I2C初始化，使用I2C2*/
    Hal_I2C_Init();
	  ESP8266_Init(EDP_CIPSTART,CWJAP);
	  LED_Switch(LED_OFF,LED_R|LED_G|LED_Y|LED_B); //熄灭开发板四个指示灯，表示程序完成设备连接
	
    EDP_Loop();
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
