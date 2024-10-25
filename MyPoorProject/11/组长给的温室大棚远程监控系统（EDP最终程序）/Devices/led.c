//#include "main.h"
//===========
#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "usart1.h"
#include "usart2.h"
#include "edpkit.h"
#include "led.h"
#include "esp8266.h"
//===========



volatile unsigned char  red_value=0;
volatile unsigned char  yellow_value=0;
volatile unsigned char  blue_value=0;
volatile unsigned char  green_value=0;

// void mDelay(uint32_t i)
// {
// 		uint32_t j=0;
// 		for(;i>0;i--)
// 		{
// 				for(j=0;j<12000;j++);
// 		}
// }

/**
  * @brief  LED指示灯初始化函数
**/
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //Pb6--对应开发板led2指示灯;Pb7--对应开发板led3指示灯;Pb8--对应开发板led4指示灯;Pb9--对应开发板led5指示灯;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	  LED_1_OFF;LED_2_OFF;LED_3_OFF;
}


/**
  * @brief  分析平台下发的LED控制命令
**/
void LED_CmdCtl(void)
{
		if((NULL != strstr((const char *)usart2_cmd_buf, "LED11")))  //约定平台控制命令"LED11"为打开红色LED灯
		{
				LED_1_ON;	
				red_value=1;
		}
		if((NULL != strstr((const char *)usart2_cmd_buf, "LED21")))  //约定平台控制命令"LED21"为打开绿色LED灯
		{
				LED_2_ON;	
				green_value=1;
		}
		if((NULL != strstr((const char *)usart2_cmd_buf, "LED31")))  //约定平台控制命令"LED31"为打开黄色LED灯
		{
				LED_3_ON;
				yellow_value=1;			
		}

		
		if((NULL != strstr((const char *)usart2_cmd_buf, "LED10")))  //约定平台控制命令"LED10"为关闭红色LED灯
		{
				LED_1_OFF;	
				red_value=0;
		}
		if((NULL != strstr((const char *)usart2_cmd_buf, "LED20")))  //约定平台控制命令"LED20"为关闭绿色LED灯
		{
				LED_2_OFF;
				green_value=0;	
		}
		if((NULL != strstr((const char *)usart2_cmd_buf, "LED30")))  //约定平台控制命令"LED30"为关闭黄色LED灯
		{
				LED_3_OFF;	
				yellow_value=0;
		}

}

/**
  * @brief  LED指示灯操作函数，点亮和熄灭指示灯
**/
void LED_Switch(const uint8_t statu,const uint8_t mode)
{
		if(statu)
		{
				LED_RED_OFF;
				LED_GREEN_OFF;
				LED_YELLOW_OFF;
				LED_BLUE_OFF;
			
				if(mode&0x01)
				{
						LED_RED_ON;
				}
				if(mode&0x02)
				{
						LED_GREEN_ON;
				}
				if(mode&0x04)
				{
						LED_YELLOW_ON;
				}
				if(mode&0x08)
				{
						LED_BLUE_ON;
				}
		}
		else
		{
				if(mode&0x01)
				{
						LED_RED_OFF;
				}
				if(mode&0x02)
				{
						LED_GREEN_OFF;
				}
				if(mode&0x04)
				{
						LED_YELLOW_OFF;
				}
				if(mode&0x08)
				{
						LED_BLUE_OFF;
				}
		}
}


/**
  * @brief  获取LED的当前状态值
**/
void LED_GetValue(void)
{
		if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_6))
		{
				red_value=0;
		}
		else
		{
				red_value=1;
		}
		
		if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_7))
		{
				green_value=0;
		}
		else
		{
				green_value=1;
		}
		
		if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8))								
		{
				yellow_value=0;
		}
		else
		{
				yellow_value=1;
		}
		
		if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_9))
		{
				blue_value=0;
		}
		else
		{
				blue_value=1;
		}
		
}







