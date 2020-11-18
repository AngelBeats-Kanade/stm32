#include "zlg7290.h"
#include "ad9959.h"
#include "delay.h"
#include "exti.h"
#include "myiic.h"
#include "sys.h"
#include "usart.h"

int a = 1, b = 1, c = 1, d = 1, e = 1, f = 1;
int i;
void ZLGKeyBoard_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  IIC_Start();
  // delay_us(30);
  IIC_Send_Byte(0x70);
  // delay_us(30);
  a = IIC_Wait_Ack();
  // delay_us(30);
  IIC_Send_Byte(0x00);
  b = IIC_Wait_Ack();
  IIC_Send_Byte(0xf0);
  c = IIC_Wait_Ack();
  IIC_Stop();

  IIC_Start();
  // delay_us(30);
  IIC_Send_Byte(0x70);
  // delay_us(30);
  d = IIC_Wait_Ack();
  // delay_us(30);
  IIC_Send_Byte(0x01);
  e = IIC_Wait_Ack();
  IIC_Send_Byte(0x00);
  f = IIC_Wait_Ack();
  IIC_Stop();

  delay_us(30);
}

u8 key_read(void)
{
  u8 num = 10;
  IIC_Start();
  // delay_us(30);
  IIC_Send_Byte(0x70);
  // delay_us(30);
  d = IIC_Wait_Ack();
  IIC_Send_Byte(0x01);
  e = IIC_Wait_Ack();
  IIC_Start();
  IIC_Send_Byte(0x71);
  f = IIC_Wait_Ack();

  // delay_us(30);
  num = IIC_Read_Byte(0);
  // delay_us(30);
  IIC_Stop();

  return num;
}
extern int down_flag;
extern u16 x;
u32 frequence = 0;
u32 key_point[5];
u32 times = 0, key_value;
u8 key_return()
{
  i = 0;
  if (down_flag == 1)
  {
    // LED0=!LED0;
    i = key_read();
    down_flag = 0;
    //	printf("%c\n",i);
    switch (i)
    {
    case 0x01:
      key_point[times] = 0;
      times++;
      break;
    case 0x02:
      key_point[times] = 1;
      times++;
      break;
    case 0x03:
      key_point[times] = 2;
      times++;
      break;
    case 0x04:
      key_point[times] = 3;
      times++;
      break;
    case 0x05:
      key_point[times] = 4;
      times++;
      break;

    case 0x09:
      key_point[times] = 5;
      times++;
      break;
    case 0x0a:
      key_point[times] = 6;
      times++;
      break;
    case 0x0b:
      key_point[times] = 7;
      times++;
      break;
    case 0x0c:
      key_point[times] = 8;
      times++;
      break;
    case 0x0d:
      key_point[times] = 9;
      times++;
      break;

    case 0x11:
      key_value = key_point[0] * 10000000 + key_point[1] * 1000000 + key_point[2] * 100000 + key_point[3] * 10000 +
                  key_point[4] * 1000;
      times = 0;

    case 0x12:
      return 'b';
    case 0x13:
      return 'c';
    case 0x14:
      return 'd';
    case 0x15:
      return 'e';

    case 0x19:
      return 'f';
    case 0x1a:
      return 'g';
    case 0x1b:
      return 'h';
    case 0x1c:
      return 'i';
    case 0x1d:
      return 'j';

    case 0x21:
      return 'k';
    case 0x22:
      return 'l';
    case 0x23:
      return 'm';
    case 0x24:
      return 'n';
    case 0x25:
      return 'o';
    default:
      return 0;
    }
    // delay_us(20);
  }
}
