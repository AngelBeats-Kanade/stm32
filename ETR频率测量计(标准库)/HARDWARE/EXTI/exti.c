//#include "led.h"
//#include "key.h"
//#include "delay.h"
//#include "exti.h"
////�ⲿ�ж�0�������
//void EXTI0_IRQHandler(void) {
//	delay_ms(10);
//	if(WK_UP==1)LED0=!LED0;
//	EXTI_ClearITPendingBit(EXTI_Line0);//���Line0�ϵ��жϱ�־λ
//}
//void EXTI4_IRQHandler(void) {
//	delay_ms(10);
//	if(KEY0==0)LED1=!LED1;
//	EXTI_ClearITPendingBit(EXTI_Line4);//���Line4�ϵ��жϱ�־λ
//}
////��ʼ��PE4��PA0Ϊ�ж�����
//void EXTIX_Init(void) {
//	NVIC_InitTypeDef NVIC_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;
//	KEY_Init();
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);//ʹ���ⲿ�ж�ʱ��
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);//PA0������0
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);//PE4������4
//	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
//	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//�ж��¼�
//	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//�½��ش���
//	EXTI_InitStructure.EXTI_LineCmd=ENABLE;//�ж���ʹ��
//	EXTI_Init(&EXTI_InitStructure);//����
//	//����Line0
//	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
//	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//�ж��¼�
//	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;//�½��ش���
//	EXTI_InitStructure.EXTI_LineCmd=ENABLE;//�ж���ʹ��
//	EXTI_Init(&EXTI_InitStructure);//����
//	//���ȼ�����
//	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;//�ⲿ�ж�0
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;//��ռ���ȼ�0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;//����Ӧ���ȼ�2
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//ʹ���ⲿͨ���ж�
//	NVIC_Init(&NVIC_InitStructure);//����NVIC
//	
//	NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn;//�ⲿ�ж�0
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;//��ռ���ȼ�0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;//����Ӧ���ȼ�2
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//ʹ���ⲿͨ���ж�
//	NVIC_Init(&NVIC_InitStructure);//����NVIC

//	
//}

//#include "sys.h"
//#include "usart.h"	
//////////////////////////////////////////////////////////////////////////////////// 	 
////���ʹ��ucos,����������ͷ�ļ�����.
//#if SYSTEM_SUPPORT_OS
//#include "includes.h"					//ucos ʹ��	  
//#endif
////////////////////////////////////////////////////////////////////////////////////	 
////������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
////ALIENTEK STM32F4̽���߿�����
////����1��ʼ��		   
////����ԭ��@ALIENTEK
////������̳:www.openedv.com
////�޸�����:2014/6/10
////�汾��V1.5
////��Ȩ���У�����ؾ���
////Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
////All rights reserved
////********************************************************************************
////V1.3�޸�˵�� 
////֧����Ӧ��ͬƵ���µĴ��ڲ���������.
////�����˶�printf��֧��
////�����˴��ڽ��������.
////������printf��һ���ַ���ʧ��bug
////V1.4�޸�˵��
////1,�޸Ĵ��ڳ�ʼ��IO��bug
////2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
////3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
////4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
////V1.5�޸�˵��
////1,�����˶�UCOSII��֧��
//////////////////////////////////////////////////////////////////////////////////// 	  
// 

////////////////////////////////////////////////////////////////////
////�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#if 1
//#pragma import(__use_no_semihosting)             
////��׼����Ҫ��֧�ֺ���                 
//struct __FILE 
//{ 
//	int handle; 
//}; 

//FILE __stdout;       
////����_sys_exit()�Ա���ʹ�ð�����ģʽ    
//_sys_exit(int x) 
//{ 
//	x = x; 
//} 
////�ض���fputc���� 
//int fputc(int ch, FILE *f)
//{ 	
//	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
//	USART1->DR = (u8) ch;      
//	return ch;
//}
//#endif
// 
//#if EN_USART1_RX   //���ʹ���˽���
////����1�жϷ������
////ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
//u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
////����״̬
////bit15��	������ɱ�־
////bit14��	���յ�0x0d
////bit13~0��	���յ�����Ч�ֽ���Ŀ
//u16 USART_RX_STA=0;       //����״̬���	

////��ʼ��IO ����1 
////bound:������
//void uart_init(u32 bound){
//   //GPIO�˿�����
//  GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
// 
//	//����1��Ӧ���Ÿ���ӳ��
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
//	
//	//USART1�˿�����
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
//	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

//   //USART1 ��ʼ������
//	USART_InitStructure.USART_BaudRate = bound;//����������
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
//  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
//	
//  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
//	
//	//USART_ClearFlag(USART1, USART_FLAG_TC);
//	
//#if EN_USART1_RX	
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

//	//Usart1 NVIC ����
//  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

//#endif
//	
//}


//void USART1_IRQHandler(void)                	//����1�жϷ������
//{
//	u8 Res;
//#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntEnter();    
//#endif
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//	{
//		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
//		
//		if((USART_RX_STA&0x8000)==0)//����δ���
//		{
//			if(USART_RX_STA&0x4000)//���յ���0x0d
//			{
//				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
//				else USART_RX_STA|=0x8000;	//��������� 
//			}
//			else //��û�յ�0X0D
//			{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//				}		 
//			}
//		}   		 
//  } 
//#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntExit();  											 
//#endif
//} 
//#endif	

// 



#include "exti.h"
#include "delay.h" 
#include "sys.h"
#include "stm32f4xx_exti.h"
#include "myiic.h"

int down_flag = 0;
//��PB0��Ϊ�ж�����
void EXTI0_IRQHandler(void)
{
	 down_flag=1;
	 EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־
}	

void EXTIX_Init(void)
{
	
  NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);//PB0 ���ӵ��ж���0
  /* ����EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
  EXTI_Init(&EXTI_InitStructure);//����

	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
}


