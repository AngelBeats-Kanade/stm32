#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

//ֱ�Ӳ����⺯����ȡIO��״̬

#define WK_UP GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//��ȡGPIOA_0


#define WK_UP_PRES 1


void KEY_Init(void);//IO�ڳ�ʼ��
u8 KEY_Scan(u8);//����ɨ��
#endif