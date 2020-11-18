#include "key.h"
#include "delay.h"

//按键初始化函数
void KEY_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);//使能时钟
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;//GPIOC_0
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//模拟输入模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//上拉

	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	
}
	//mode:0不可连续按；1可连续按
	//0 没有按键按下
	//1 WK UP按下；2 key0按下
	u8 KEY_Scan(u8 mode){ 
		static u8 key_up=1;//按键松开标志
		if(mode)key_up=1;//支持连续按
		if(key_up&&(WK_UP==0)) {
			delay_ms(10);//去抖动
			key_up=0;
			if(WK_UP==0)return 1;
		}
		else if(WK_UP==1)key_up=1;
		return 0;//无按键按下
		
	}

