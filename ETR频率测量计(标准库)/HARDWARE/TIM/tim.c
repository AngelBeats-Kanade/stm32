#include "ad9959.h"
#include "adf4351.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"

void TIM3_Int_Init(u16 arr, u16 psc)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  TIM_TimeBaseInitStructure.TIM_Period = arr;
  TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  TIM_Cmd(TIM3, ENABLE);
}

void TIM2_Int_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
}

void TIM2_Mode_Sec()
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

  TIM_DeInit(TIM2);

  TIM_TimeBaseInitStructure.TIM_Period = 0xffffffff;
  TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
  TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x00);

  TIM_SetCounter(TIM2, 0);
  TIM_Cmd(TIM2, ENABLE);
}
extern u32 CNT;
extern u16 x;
extern float frequency;
extern float n;
void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {

    TIM_Cmd(TIM2, DISABLE);
    TIM_Cmd(TIM3, DISABLE);
    CNT = TIM_GetCounter(TIM2);
    CNT = 10 * CNT;
    printf("\r\n%d", CNT);
    TIM_SetCounter(TIM2, 0);
    TIM_SetCounter(TIM3, 0);
    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
  }
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}
