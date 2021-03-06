#include "adc.h"
#include "delay.h"		
#include "usart.h"
#include "ad9959.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//ADC 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//初始化ADC															   
void  Adc_DMA_Init(void)
{    
    GPIO_InitTypeDef  GPIO_InitStructure;
//	DMA_InitTypeDef  DMA_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 
    
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
	
    //先初始化ADC1通6 IO口
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PA6 通 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始�
	//初始化ADC1通道5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//初始化DMA2

 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 

	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化

  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//扫描
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
  
  
	
  ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_3Cycles);
//  ADC_RegularChannelConfig(ADC1,ADC_Channel_6,2,ADC_SampleTime_3Cycles);
//  
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

  //ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  
	//ADC_DMACmd(ADC1,ENABLE);
  
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	
	ADC_SoftwareStartConv(ADC1);

}				  
////获得ADC值
////ch: @ref ADC_channels 
////通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
////返回值:转换结果
u16 Get_Adc(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_3Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}
////获取通道ch的转换值，取times次,然后平均 
////ch:通道编号
////times:获取次数
////返回值:通道ch的times次转换结果平均值
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

//	if(max>shreshold_value)//若大于门限值电压，则可以搜到台
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







