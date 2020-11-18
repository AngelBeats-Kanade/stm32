#include "adc.h"
#include "delay.h"		
#include "usart.h"
#include "ad9959.h"
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK STM32F407¿ª·¢°å
//ADC Çı¶¯´úÂë	   
//ÕıµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//´´½¨ÈÕÆÚ:2014/5/6
//°æ±¾£ºV1.0
//°æÈ¨ËùÓĞ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖİÊĞĞÇÒíµç×Ó¿Æ¼¼ÓĞÏŞ¹«Ë¾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//³õÊ¼»¯ADC															   
void  Adc_DMA_Init(void)
{    
    GPIO_InitTypeDef  GPIO_InitStructure;
//	DMA_InitTypeDef  DMA_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2Ê±ÖÓÊ¹ÄÜ 
    
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Ê¹ÄÜGPIOAÊ±ÖÓ
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//Ê¹ÄÜGPIOBÊ±ÖÓ
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //Ê¹ÄÜADC1Ê±ÖÓ
	
    //ÏÈ³õÊ¼»¯ADC1Í¨6 IO¿Ú
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PA6 Í¨ 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//Ä£ÄâÊäÈë
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//²»´øÉÏÏÂÀ­
    GPIO_Init(GPIOA, &GPIO_InitStructure);//³õÊ¼»
	//³õÊ¼»¯ADC1Í¨µÀ5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//³õÊ¼»¯DMA2

 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1¸´Î»
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//¸´Î»½áÊø	 

	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//¶ÀÁ¢Ä£Ê½
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;//Á½¸ö²ÉÑù½×¶ÎÖ®¼äµÄÑÓ³Ù5¸öÊ±ÖÓ
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAÊ§ÄÜ
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ô¤·ÖÆµ4·ÖÆµ¡£ADCCLK=PCLK2/4=84/4=21Mhz,ADCÊ±ÖÓ×îºÃ²»Òª³¬¹ı36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//³õÊ¼»¯

  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12Î»Ä£Ê½
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//É¨Ãè
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//Á¬Ğø×ª»»
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//½ûÖ¹´¥·¢¼ì²â£¬Ê¹ÓÃÈí¼ş´¥·¢
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ÓÒ¶ÔÆë	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1¸ö×ª»»ÔÚ¹æÔòĞòÁĞÖĞ Ò²¾ÍÊÇÖ»×ª»»¹æÔòĞòÁĞ1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC³õÊ¼»¯
  
  
	
  ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_3Cycles);
//  ADC_RegularChannelConfig(ADC1,ADC_Channel_6,2,ADC_SampleTime_3Cycles);
//  
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

  //ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  
	//ADC_DMACmd(ADC1,ENABLE);
  
	ADC_Cmd(ADC1, ENABLE);//¿ªÆôAD×ª»»Æ÷	
	ADC_SoftwareStartConv(ADC1);

}				  
////»ñµÃADCÖµ
////ch: @ref ADC_channels 
////Í¨µÀÖµ 0~16È¡Öµ·¶Î§Îª£ºADC_Channel_0~ADC_Channel_16
////·µ»ØÖµ:×ª»»½á¹û
u16 Get_Adc(u8 ch)   
{
	  	//ÉèÖÃÖ¸¶¨ADCµÄ¹æÔò×éÍ¨µÀ£¬Ò»¸öĞòÁĞ£¬²ÉÑùÊ±¼ä
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_3Cycles );	//ADC1,ADCÍ¨µÀ,480¸öÖÜÆÚ,Ìá¸ß²ÉÑùÊ±¼ä¿ÉÒÔÌá¸ß¾«È·¶È			    
  
	ADC_SoftwareStartConv(ADC1);		//Ê¹ÄÜÖ¸¶¨µÄADC1µÄÈí¼ş×ª»»Æô¶¯¹¦ÄÜ	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//µÈ´ı×ª»»½áÊø
	return ADC_GetConversionValue(ADC1);	//·µ»Ø×î½üÒ»´ÎADC1¹æÔò×éµÄ×ª»»½á¹û
}
////»ñÈ¡Í¨µÀchµÄ×ª»»Öµ£¬È¡times´Î,È»ºóÆ½¾ù 
////ch:Í¨µÀ±àºÅ
////times:»ñÈ¡´ÎÊı
////·µ»ØÖµ:Í¨µÀchµÄtimes´Î×ª»»½á¹ûÆ½¾ùÖµ
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

//	if(max>shreshold_value)//Èô´óÓÚÃÅÏŞÖµµçÑ¹£¬Ôò¿ÉÒÔËÑµ½Ì¨
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







