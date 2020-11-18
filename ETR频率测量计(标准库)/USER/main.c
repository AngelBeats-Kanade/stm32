#include "24cxx.h"
#include "ad9959.h"
#include "adf4351.h"
#include "delay.h"
#include "exti.h"
#include "key.h"
#include "myiic.h"
#include "stm32f4xx.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"
#include "zlg7290.h"

u16 x = 0;
u32 CNT = 0;
float frequency = 40.700;
float n = 0;
int main(void)
{
  u16 t;
  u8 y = 0;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  uart_init(115200);
  delay_init(168);
  //	IIC_Init();
  //	EXTIX_Init();
  //	ZLGKeyBoard_Init();
  KEY_Init();
  TIM3_Int_Init(1000 - 1, 8400 - 1); // 500ms
  TIM2_Int_Init();
  TIM2_Mode_Sec();
  while (1)
  {
    ;
  }
}