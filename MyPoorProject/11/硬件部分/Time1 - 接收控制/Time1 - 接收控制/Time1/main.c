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

	USART_ClearFlag(USART1,USART_FLAG_TC);//�巢�ͽ���λ
	
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
		
		
		DHT11_Read_Data(&temperature,&humidity);	//��ȡ�¶Ⱥ�ʪ��
		
		printf("temperature=%d��\n",temperature);
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
		
		
		//������������¶ȴﵽ30��ת��
		
		//
	 if (Rx_Flag==1)
{
   USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); //�رս����ж�
   Rx_Flag=0;
   RxCounter=0;
	 if (strstr(CmdBuffer,"1"))
		{
			key = 1;
		}
		else if(strstr(CmdBuffer,"2")){
			key = 0;
		}
		
		 USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//���������ж�,���ռĴ������գ�RXNE=1��ʱ�����ж�
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
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ������
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;   //���ùܽ�	PA10/USART1_RX 
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_IN_FLOATING;//��©����
	GPIO_Init(GPIOA,&GPIO_InitStructure);//��ʼ��PA10



	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9; //���ùܽ�PA9/USART1_TX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;  //IO������Ϊ���������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);  //��ʼ��PA9
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0; //���ùܽ�DAC OUT1
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;  //IO������Ϊ���������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);  //��ʼ��PC0
	
	//����ܽ�PC2(�ߵ�ƽ)  PC3��GND��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //���ùܽ�2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //IO������Ϊ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����Ƶ��50MHz
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PC2��
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //���ùܽ�8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //IO������Ϊ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����Ƶ��50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA8��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //���ùܽ�2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //IO������Ϊ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����Ƶ��50MHz
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PD2��
	
	

}
void RCC_Configuration(void){
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��GPIOA��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//ʹ��GPIOD��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1��ʱ�ӣ� USART1�ҽӵ�APB2�ϡ�����USART2-5�ҽӵ�APB1��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); 
	
}


void  ADC_Configuration(void) {
	ADC_InitTypeDef  ADC_InitStructure;
	
	ADC_TempSensorVrefintCmd(ENABLE);

	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode  = ADC_Mode_Independent;//ѡ��ADC�����ڶ���ģʽ����˫ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode  = ENABLE;  //Ȩ������ɨ��ģʽ����ͨ�������ǵ��Σ���ͨ����ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode  = ENABLE;//ѡ��ADC�������������ǵ���ģʽ��

	ADC_InitStructure.ADC_ExternalTrigConv  = ADC_ExternalTrigConv_None;//����ʹ���ⲿ��������������ͨ����ADC
	ADC_InitStructure.ADC_DataAlign  = ADC_DataAlign_Right;//������������뻹���Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel  = 1;//���ý��й���ת����ADCͨ����Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_55Cycles5);
	
	ADC_Cmd(ADC1, ENABLE);//ʹ��ADC1
	
	ADC_ResetCalibration(ADC1);  // ��λADC1У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));  //�ȴ�У׼��λ����

	ADC_StartCalibration(ADC1);//��ʼУ׼
	while(ADC_GetCalibrationStatus(ADC1)); //�ȴ�У׼����
	
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


void USART1_IRQHandler(void) //����1 �жϷ������
 {
 unsigned int i=0;
 if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�жϽ��ռĴ����Ƿ�ǿ�
{
	//������ռĴ����ǿգ�˵����⵽�ж϶���
		RxBuffer[RxCounter++]=USART_ReceiveData(USART1); // Read one byte from the receive data register 
		if (RxBuffer[RxCounter-1]==' ') //�����⵽���У������ý��ձ�־Ϊ1. 
			// \n:���У���굽�������ף� 
		// \r:�س�����굽��������
		{
     Rx_Flag=1;
     for (i=0;i<RxCounter;i++)//�ѽ��յ������ݻ��浽CmdBuffer��
     {
        CmdBuffer[i]=RxBuffer[i];
      }
     CmdBuffer[RxCounter]=0; //���ͻ�����������
      RxCounter=0;
     }
if(RxCounter == RxBufferSize) //������ջ��������ˡ�
    {
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); //�رս����ж�
    Rx_Flag=1;
    CmdBuffer[RxCounter]=0; //�����������������
    RxCounter=0;
   }
  }
}


void RTC_Configuration(void)

{
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
   //ʹ�ܵ�Դ�ͺ���
    PWR_BackupAccessCmd(ENABLE);
    // �������BKP����
    BKP_DeInit();
    //BKP��λ
    RCC_LSEConfig(RCC_LSE_ON);
    //�����ⲿ���پ���  32.768k
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    //�ȴ��ⲿ���׾����Ƿ��Ѿ��ȶ� 
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    //ѡ��RTC�ⲿʱ��Ϊ���׾���
    RCC_RTCCLKCmd(ENABLE);
    //ʹ��RTCʱ��
    RTC_WaitForSynchro();
    //�ȴ�RTC�Ĵ���ͬ��            
    RTC_WaitForLastTask();
    //�ȴ�дRTC�Ĵ������
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    //ʹ��RTC���ж�       
    RTC_WaitForLastTask();
		RTC_SetPrescaler(32767);
		//����Ԥ��Ƶ
		RTC_WaitForLastTask();
}

void RTCClock_Init(void){
	if(BKP_ReadBackupRegister(BKP_DR1)!=0xA5A5){
		//��һ������  ��ʼ������
		//RTC��ʼ��
		RTC_Configuration();
		//�ȴ�дRTC�Ĵ������
		RTC_WaitForLastTask();
    //����ʱ���ֵ
		RTC_SetCounter(0x001B82BC);
		RTC_WaitForLastTask();
		//д�����ñ�־
		BKP_WriteBackupRegister(BKP_DR1,0xA5A5);
	}
	else{
		RTC_WaitForSynchro();
    //�ȴ�RTC�Ĵ���ͬ��            
    RTC_WaitForLastTask();
    //�ȴ�дRTC�Ĵ������
		 RTC_ITConfig(RTC_IT_SEC, ENABLE);
    //ʹ��RTC���ж�   
		RTC_WaitForLastTask();
		
	}
	
	RCC_ClearFlag();    //�����λ��־
}
 

void RTC_IRQHandler(void) {
	if(RTC_GetITStatus(RTC_IT_SEC)!= RESET)//��ȡ�жϱ�־
	{
		RTC_ClearITPendingBit(RTC_IT_SEC);//����жϱ�־
		tim_bz=1;//���жϱ�־
	}
}

u16 Get_val(u8 ch)
{
    u16 DataValue; //����Ӣ��ע�͡�������    
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





