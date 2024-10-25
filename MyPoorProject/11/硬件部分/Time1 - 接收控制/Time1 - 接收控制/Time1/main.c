#include <stm32f10x.h>
#include <stdio.h>
#include <stdlib.h>

#include "usart.h"
#include "DHT11.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"

#define year_days(a) (((a%100!=0)&&(a%4==0))||(a%400==0))   
#define days_in_year(i) (year_days(i) ? 366 : 365)  
 
unsigned long days_in_month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#define RxBufferSize 10
unsigned char RxBuffer[RxBufferSize];
unsigned char CmdBuffer[RxBufferSize];
extern unsigned char  Rx_Flag = 0;
unsigned char RxCounter=0;


void  GPIO_Configuration(void) ;
void 	RCC_Configuration(void);
void  ADC_Configuration(void) ;
void  RTC_Configuration(void);
void  RTCClock_Init(void);
void  NVIC_Configuration(void);


u16 Get_val(u8 ch);
int tim_bz=0;
u32 ADC_ConvertedValue;
static unsigned long ticks;


int main(void)
{
	u8 temperature;  	    
	u8 humidity; 
	

	u16 adcx;
	unsigned int n;
	int i;
	int key =0;
	
	u32 TimeData=0,hms=0;
	u32 year=0,month=0,days=0,hour=0,min=0,sec=0;
	
	RCC_Configuration();
	GPIO_Configuration();
	ADC_Configuration();
	NVIC_Configuration();
	RTCClock_Init();
	
	
	
	delay_init();
	uart_init(115200);

	USART_ClearFlag(USART1,USART_FLAG_TC);//清发送结束位
	
	 memset(CmdBuffer,10,0);
	
		printf("AT+CIPMODE=1\r\n");
	  delay_ms(500);
	  printf("AT+CIPSTART=\"TCP\",\"192.168.43.26\",8881\r\n"); 
	  delay_ms(500);
	  printf("AT+CIPSTATUS\r\n");
  	delay_ms(500);
	  printf("AT+CIPSEND\r\n");
	  delay_ms(500);
		
		
		
		POINT_COLOR=RED;
		LCD_Init();
		
		
		
		GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	  GPIO_SetBits(GPIOA,GPIO_Pin_8);
		
		
		while(1)
	{
	
		
		if(tim_bz==1){
			tim_bz=0;
			TimeData=RTC_GetCounter();
			days=TimeData/86400;
			hms=TimeData%86400;
			hour=hms/3600;
			min=(hms%3600)/60;
			
			sec=(hms%3600)%60;
			
			for(i = 2019; days > days_in_year(i); i++)    
		  days -= days_in_year(i);
			year = i;                        
	
			if(year_days(i))                      
				days_in_month[2] = 29;
			else
				days_in_month[2] = 28;
	
			for(i = 6; days > days_in_month[i]; i++)     
				days -= days_in_month[i];
				month = i; 
				
  

			printf("%0.4d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d\r\n",year,month,days,hour,min,sec);
			
		}   
		
		
		DHT11_Read_Data(&temperature,&humidity);	//获取温度和湿度
		
		printf("temperature=%d℃\n",temperature);
		printf("humidity=%d%%\r\n",humidity);
		
		LCD_ShowNum(10,60,year,4,24);
		LCD_ShowString(60,60,180,24,24,".");	
		LCD_ShowNum(65,60,month,2,24);
		LCD_ShowString(90,60,180,24,24,".");	
		LCD_ShowNum(100,60,days,2,24);
		LCD_ShowString(135,60,180,24,24,"  ");	
		LCD_ShowNum(140,60,hour,2,24);
		LCD_ShowString(165,60,180,24,24,":");	
		LCD_ShowNum(185,60,min,2,24);
		LCD_ShowString(210,60,180,24,24,":");	
		LCD_ShowNum(220,60,sec,2,24);
		
		LCD_ShowString(10,90,200,24,24,"temp:");	
		LCD_ShowNum(100,90,temperature,2,24);
		LCD_ShowString(160,90,200,24,24,"C");
		LCD_ShowString(10,120,200,16,24,"humi:");
		LCD_ShowNum(100,120,humidity,2,24);
		LCD_ShowString(160,120,200,16,24,"%");
		
		LCD_ShowString(10,150,200,16,24,CmdBuffer);
		
		delay_ms(9300);	
		delay_ms(9300);
		delay_ms(9300);
		
		LCD_Clear(WHITE);
		
		
		//电机驱动程序（温度达到30旋转）
		
		//
	 if (Rx_Flag==1)
{
   USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); //关闭接收中断
   Rx_Flag=0;
   RxCounter=0;
	 if (strstr(CmdBuffer,"1"))
		{
			key = 1;
		}
		else if(strstr(CmdBuffer,"2")){
			key = 0;
		}
		
		 USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启接收中断,接收寄存器不空（RXNE=1）时产生中断
	}
		
		if(temperature >= 32 || key==1){
			GPIO_SetBits(GPIOC,GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		}
		else{
			GPIO_ResetBits(GPIOC,GPIO_Pin_2);
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
		}
		
		
	}
}


void  GPIO_Configuration(void) {
	GPIO_InitTypeDef GPIO_InitStructure;//声明GPIO初始化结构变量。
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;   //配置管脚	PA10/USART1_RX 
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_IN_FLOATING;//开漏输入
	GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化PA10



	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9; //配置管脚PA9/USART1_TX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;  //IO口配置为复用输出口
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);  //初始化PA9
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0; //配置管脚DAC OUT1
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;  //IO口配置为复用输出口
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);  //初始化PC0
	
	//电机管脚PC2(高电平)  PC3（GND）
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //配置管脚2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //IO口配置为推挽输出口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //工作频率50MHz
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PC2口
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //配置管脚8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //IO口配置为推挽输出口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //工作频率50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA8口
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //配置管脚2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //IO口配置为推挽输出口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //工作频率50MHz
	GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化PD2口
	
	

}
void RCC_Configuration(void){
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//使能GPIOD的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1的时钟， USART1挂接到APB2上。其他USART2-5挂接到APB1上
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); 
	
}


void  ADC_Configuration(void) {
	ADC_InitTypeDef  ADC_InitStructure;
	
	ADC_TempSensorVrefintCmd(ENABLE);

	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode  = ADC_Mode_Independent;//选择ADC工作在独立模式或者双ADC模式
	ADC_InitStructure.ADC_ScanConvMode  = ENABLE;  //权责工作在扫描模式（多通道）还是单次（单通道）模式
	ADC_InitStructure.ADC_ContinuousConvMode  = ENABLE;//选择ADC工作在连续还是单次模式。

	ADC_InitStructure.ADC_ExternalTrigConv  = ADC_ExternalTrigConv_None;//定义使用外部触发来启动规则通道的ADC
	ADC_InitStructure.ADC_DataAlign  = ADC_DataAlign_Right;//设置数据左对齐还是右对齐
	ADC_InitStructure.ADC_NbrOfChannel  = 1;//设置进行规则转换的ADC通道数目
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_55Cycles5);
	
	ADC_Cmd(ADC1, ENABLE);//使能ADC1
	
	ADC_ResetCalibration(ADC1);  // 复位ADC1校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));  //等待校准复位结束

	ADC_StartCalibration(ADC1);//开始校准
	while(ADC_GetCalibrationStatus(ADC1)); //等待校准结束
	
	//ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}


void USART1_IRQHandler(void) //串口1 中断服务程序
 {
 unsigned int i=0;
 if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //判断接收寄存器是否非空
{
	//如果接收寄存器非空，说明检测到中断读入
		RxBuffer[RxCounter++]=USART_ReceiveData(USART1); // Read one byte from the receive data register 
		if (RxBuffer[RxCounter-1]==' ') //如果检测到换行，则设置接收标志为1. 
			// \n:换行，光标到下行行首； 
		// \r:回车，光标到本行行首
		{
     Rx_Flag=1;
     for (i=0;i<RxCounter;i++)//把接收到的数据缓存到CmdBuffer中
     {
        CmdBuffer[i]=RxBuffer[i];
      }
     CmdBuffer[RxCounter]=0; //发送缓冲区结束符
      RxCounter=0;
     }
if(RxCounter == RxBufferSize) //如果接收缓冲区满了。
    {
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); //关闭接收中断
    Rx_Flag=1;
    CmdBuffer[RxCounter]=0; //接收命令缓冲区结束符
    RxCounter=0;
   }
  }
}


void RTC_Configuration(void)

{
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
   //使能电源和后备域
    PWR_BackupAccessCmd(ENABLE);
    // 允许访问BKP区域
    BKP_DeInit();
    //BKP复位
    RCC_LSEConfig(RCC_LSE_ON);
    //启动外部低速晶振  32.768k
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    //等待外部低俗晶振是否已经稳定 
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    //选择RTC外部时钟为低俗晶振
    RCC_RTCCLKCmd(ENABLE);
    //使能RTC时钟
    RTC_WaitForSynchro();
    //等待RTC寄存器同步            
    RTC_WaitForLastTask();
    //等待写RTC寄存器完成
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    //使能RTC秒中断       
    RTC_WaitForLastTask();
		RTC_SetPrescaler(32767);
		//设置预分频
		RTC_WaitForLastTask();
}

void RTCClock_Init(void){
	if(BKP_ReadBackupRegister(BKP_DR1)!=0xA5A5){
		//第一次运行  初始化设置
		//RTC初始化
		RTC_Configuration();
		//等待写RTC寄存器完成
		RTC_WaitForLastTask();
    //设置时间初值
		RTC_SetCounter(0x001B82BC);
		RTC_WaitForLastTask();
		//写入配置标志
		BKP_WriteBackupRegister(BKP_DR1,0xA5A5);
	}
	else{
		RTC_WaitForSynchro();
    //等待RTC寄存器同步            
    RTC_WaitForLastTask();
    //等待写RTC寄存器完成
		 RTC_ITConfig(RTC_IT_SEC, ENABLE);
    //使能RTC秒中断   
		RTC_WaitForLastTask();
		
	}
	
	RCC_ClearFlag();    //清除复位标志
}
 

void RTC_IRQHandler(void) {
	if(RTC_GetITStatus(RTC_IT_SEC)!= RESET)//读取中断标志
	{
		RTC_ClearITPendingBit(RTC_IT_SEC);//清除中断标志
		tim_bz=1;//秒中断标志
	}
}

u16 Get_val(u8 ch)
{
    u16 DataValue; //又是英文注释、、啊哈    
		/* ADC1 regular channel14 configuration */ 
			ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);
    
    /* Start ADC1 Software Conversion */ 
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    /* Test if the ADC1 EOC flag is set or not */ 

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

		/*Returns the ADC1 Master data value of the last converted channel*/
		DataValue = ADC_GetConversionValue(ADC1); 
		return DataValue; 
} 





