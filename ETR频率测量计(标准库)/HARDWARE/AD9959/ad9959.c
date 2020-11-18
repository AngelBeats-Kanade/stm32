#include "ad9959.h"

u8 CSR_DATA0[1] = {0x10}; //  CH0
u8 CSR_DATA1[1] = {0x20}; //  CH1
u8 CSR_DATA2[1] = {0x40}; //  CH2
u8 CSR_DATA3[1] = {0x80}; //  CH3
u32 SinFre[5] = {50, 50, 50, 50};
u32 SinAmp[5] = {1023, 1023, 1023, 1023};
u32 SinPhr[5] = {0, 4095, 4095 * 3, 4095 * 2};

u8 FR2_DATA[2] = {0x00, 0x00};       // default Value = 0x0000
u8 CFR_DATA[3] = {0x00, 0x03, 0x02}; // default Value = 0x000302

u8 CPOW0_DATA[2] = {0x00, 0x00}; // default Value = 0x0000

u8 LSRR_DATA[2] = {0x00, 0x00};

u8 RDW_DATA[4] = {0x00, 0x00, 0x00, 0x00};

u8 FDW_DATA[4] = {0x00, 0x00, 0x00, 0x00};

void Init_AD9959(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  u8 FR1_DATA[3] = {0xD0, 0x00, 0x00};
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |
                                GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_13 |
                                GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10;
  //	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  //	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //	GPIO_Init(GPIOG, &GPIO_InitStructure);

  //	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  //	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  //	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //	GPIO_Init(GPIOG, &GPIO_InitStructure);

  Intserve();
  IntReset();

  WriteData_AD9959(FR1_ADD, 3, FR1_DATA, 1);
  //  WriteData_AD9959(FR2_ADD,2,FR2_DATA,0);
  //  WriteData_AD9959(CFR_ADD,3,CFR_DATA,1);
  //  WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
  //  WriteData_AD9959(ACR_ADD,3,ACR_DATA,0);
  //  WriteData_AD9959(LSRR_ADD,2,LSRR_DATA,0);
  //  WriteData_AD9959(RDW_ADD,2,RDW_DATA,0);
  //  WriteData_AD9959(FDW_ADD,4,FDW_DATA,1);

  Write_frequence(3, SinFre[3]);
  Write_frequence(0, SinFre[0]);
  Write_frequence(1, SinFre[1]);
  Write_frequence(2, SinFre[2]);

  ////	Write_frequence(3,50);
  ////	Write_frequence(0,50);
  ////	Write_frequence(1,50);
  ////	Write_frequence(2,50);

  Write_Phase(3, SinPhr[3]);
  Write_Phase(0, SinPhr[0]);
  Write_Phase(1, SinPhr[1]);
  Write_Phase(2, SinPhr[2]);

  Write_Amplitude(3, SinAmp[3]);
  Write_Amplitude(0, SinAmp[0]);
  Write_Amplitude(1, SinAmp[1]);
  Write_Amplitude(2, SinAmp[2]);
}

void delay1(u32 length)
{
  length = length * 12;
  while (length--)
    ;
}

void Intserve(void)
{
  AD9959_PWR = 0;
  CS = 1;
  SCLK = 0;
  UPDATE = 0;
  PS0 = 0;
  PS1 = 0;
  PS2 = 0;
  PS3 = 0;
  SDIO0 = 0;
  SDIO1 = 0;
  SDIO2 = 0;
  SDIO3 = 0;
}

void IntReset(void)
{
  Reset = 0;
  delay1(1);
  Reset = 1;
  delay1(30);
  Reset = 0;
}

void IO_Update(void)
{
  UPDATE = 0;
  delay1(2);
  UPDATE = 1;
  delay1(4);
  UPDATE = 0;
}

void WriteData_AD9959(u8 RegisterAddress, u8 NumberofRegisters, u8 *RegisterData, u8 temp)
{
  u8 ControlValue = 0;
  u8 ValueToWrite = 0;
  u8 RegisterIndex = 0;
  u8 i = 0;

  ControlValue = RegisterAddress;

  SCLK = 0;
  CS = 0;
  for (i = 0; i < 8; i++)
  {
    SCLK = 0;
    if (0x80 == (ControlValue & 0x80))
      SDIO0 = 1;
    else
      SDIO0 = 0;
    SCLK = 1;
    ControlValue <<= 1;
  }
  SCLK = 0;

  for (RegisterIndex = 0; RegisterIndex < NumberofRegisters; RegisterIndex++)
  {
    ValueToWrite = RegisterData[RegisterIndex];
    for (i = 0; i < 8; i++)
    {
      SCLK = 0;
      if (0x80 == (ValueToWrite & 0x80))
        SDIO0 = 1;
      else
        SDIO0 = 0;
      SCLK = 1;
      ValueToWrite <<= 1;
    }
    SCLK = 0;
  }
  if (temp == 1)
    IO_Update();
  CS = 1;
}

void Write_frequence(u8 Channel, u32 Freq)
{
  u8 CFTW0_DATA[4] = {0x00, 0x00, 0x00, 0x00};
  u32 Temp;
  Temp = (u32)Freq * 8.589934592;
  CFTW0_DATA[3] = (u8)Temp;
  CFTW0_DATA[2] = (u8)(Temp >> 8);
  CFTW0_DATA[1] = (u8)(Temp >> 16);
  CFTW0_DATA[0] = (u8)(Temp >> 24);
  if (Channel == 0)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 1);
    WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1);
  }
  else if (Channel == 1)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 1);
    WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1);
  }
  else if (Channel == 2)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 1);
    WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1);
  }
  else if (Channel == 3)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 1);
    WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 3);
  }
}

void Write_Amplitude(u8 Channel, u16 Ampli)
{
  u16 A_temp;
  u8 ACR_DATA[3] = {0x00, 0x00, 0x00};

  A_temp = Ampli | 0x1000;
  ACR_DATA[2] = (u8)A_temp;
  ACR_DATA[1] = (u8)(A_temp >> 8);
  if (Channel == 0)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 1);
    WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
  }
  else if (Channel == 1)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 1);
    WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
  }
  else if (Channel == 2)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 1);
    WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
  }
  else if (Channel == 3)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 1);
    WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
  }
}

void Write_Phase(u8 Channel, u16 Phase)
{
  u16 P_temp = 0;
  P_temp = (u16)Phase;
  CPOW0_DATA[1] = (u8)P_temp;
  CPOW0_DATA[0] = (u8)(P_temp >> 8);
  if (Channel == 0)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 0);
    WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 0);
  }
  else if (Channel == 1)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 0);
    WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 0);
  }
  else if (Channel == 2)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 0);
    WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 0);
  }
  else if (Channel == 3)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 0);
    WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 0);
  }
}
u32 fabsq(u32 a, u32 b)
{
  int x = 0;
  x = a - b;
  if (x >= 0)
    return x;
  if (x < 0)
    return -x;
}