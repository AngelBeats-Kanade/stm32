//#ifndef __EXTI_H
//#define __EXTI_H
//#include "sys.h"
//void EXTIX_Init(void);//外部中断初始化
//#endif
#ifndef __EXTI_H
#define __EXTI_H	 
#include "sys.h"  	
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究
//All rights reserved			  
//////////////////////////////////////////////////////////////////////////////////
extern int down_flag;

void EXTIX_Init(void);//外部中断初始化

#endif
