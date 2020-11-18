#include "adc.h"
#include "delay.h"		
#include "usart.h"
#include "ad9959.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ADC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//��ʼ��ADC															   
void  Adc_DMA_Init(void)
{    
    GPIO_InitTypeDef  GPIO_InitStructure;
//	DMA_InitTypeDef  DMA_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
    
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��
	
    //�ȳ�ʼ��ADC1ͨ6 IO��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PA6 ͨ 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ�
	//��ʼ��ADC1ͨ��5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//��ʼ��DMA2

 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 

	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��

  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//ɨ��
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
  
  
	
  ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_3Cycles);
//  ADC_RegularChannelConfig(ADC1,ADC_Channel_6,2,ADC_SampleTime_3Cycles);
//  
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

  //ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  
	//ADC_DMACmd(ADC1,ENABLE);
  
	ADC_Cmd(ADC1, ENABLE);//����ADת����	
	ADC_SoftwareStartConv(ADC1);

}				  
////���ADCֵ
////ch: @ref ADC_channels 
////ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
////����ֵ:ת�����
u16 Get_Adc(u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_3Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}
////��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
////ch:ͨ�����
////times:��ȡ����
////����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
//u16 Get_Adc_Average(u8 ch,u8 times)
//{
//	u32 temp_val=0;
//	u8 t;
//	for(t=0;t<times;t++)
//	{
//		temp_val+=Get_Adc(ch);
//		delay_ms(5);
//	}
//	return temp_val/times;
//} 
//	 
//extern u16 x;
//#define shreshold_value 1.81
//void Judge_4351()
//{
//	u32 i=0,j=0;
//	u16 adc_dnum[1000];
//	float adc_anum[1000];
//	float average=0.0,sum=0.0,max=0;
//	for(i=0;i<1000;i++)
//		{
//			adc_dnum[i]=Get_Adc(5);
//			adc_anum[i]=((float)adc_dnum[i]*3.3)/4096;
//			//sum+=adc_anum[i];
//		}
//		//average=sum/1000;
//		//sum=0;
//		printf("%d \n",x);
//		max=adc_anum[0];
//		for(i=1;i<1000;i++)
//		{
//			if(adc_anum[i]<1.4&&adc_anum[i]>1&&adc_anum[i]>adc_anum[i-1])
//			{max=adc_anum[i];}
//		}
////	average=0;
////	average=sum/j;

//	if(max>shreshold_value)//����������ֵ��ѹ��������ѵ�̨
//	{	
//		x=1;
//		printf("%0.2f \n",max);
//		printf("%d \n",x);
//	}
//	else
//		x=0;
//}
//void adc_9959_afc(void) {
//	u16 i=0;
//	float adf4351_fre=0;
//	
//		for(i=0;i<150;i++)
//	{
//		if(x)
//			break;
//		adf4351_fre=40850000+1000*i;
//		
//		delay_ms(1000);
//		Write_frequence(2,adf4351_fre);
//		Judge_4351();
//				printf("%d \n",x);
//	}
//	for(i=0;i<300;i++)
//	{
//		if(x)
//			break;
//		adf4351_fre=41000000-1000*i;
//		delay_ms(1000);
//		Write_frequence(2,adf4351_fre);
//		Judge_4351();
//		printf("%d \n",x);
//	}
//	for(i=0;i<150;i++)
//	{
//		if(x)
//			
//			break;
//		adf4351_fre=40700000 + 1000*i;
//		delay_ms(1000);
//		Write_frequence(2,adf4351_fre);
//		Judge_4351();
//		printf("%d \n",x);
//	}
//	
//}







