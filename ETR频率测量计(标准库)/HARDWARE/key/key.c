#include "key.h"
#include "delay.h"

//������ʼ������
void KEY_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);//ʹ��ʱ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;//GPIOC_0
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//ģ������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//����

	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	
}
	//mode:0������������1��������
	//0 û�а�������
	//1 WK UP���£�2 key0����
	u8 KEY_Scan(u8 mode){ 
		static u8 key_up=1;//�����ɿ���־
		if(mode)key_up=1;//֧��������
		if(key_up&&(WK_UP==0)) {
			delay_ms(10);//ȥ����
			key_up=0;
			if(WK_UP==0)return 1;
		}
		else if(WK_UP==1)key_up=1;
		return 0;//�ް�������
		
	}

