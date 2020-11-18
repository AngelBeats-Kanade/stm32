#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

//直接操作库函数读取IO口状态

#define WK_UP GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//读取GPIOA_0


#define WK_UP_PRES 1


void KEY_Init(void);//IO口初始化
u8 KEY_Scan(u8);//键盘扫描
#endif