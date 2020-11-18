#include "adf4351.h"
#include "delay.h"
#include "usart.h"


//#define 
#define ADF4351_R0			((u32)0X2C8018)
#define ADF4351_R1			((u32)0X8029)
#define ADF4351_R2			((u32)0X10E42)
#define ADF4351_R3			((u32)0X4B3)
#define ADF4351_R4			((u32)0XEC803C)
#define ADF4351_R5			((u32)0X580005)

#define ADF4351_R1_Base	((u32)0X00008001)
#define ADF4351_R4_Base	((u32)0X8C803C)
#define ADF4351_R4_ON	  ((u32)0X8C803C)
#define ADF4351_R4_OFF	((u32)0X8C883C)

#define ADF4351_RF_OFF	((u32)0XEC801C)

#define ADF4351_PD_ON		((u32)0X10E42)
#define ADF4351_PD_OFF	((u32)0X10E02)

 
void ADF_Output_GPIOInit(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
void ADF_Input_GPIOInit(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
void WriteToADF4351(u8 count, u8 *buf) {
	
	u8 ValueToWrite = 0;
	u8 i = 0;
	u8 j = 0;
	
//	ADF_Output_GPIOInit();
	
	ADF4351_CE = 1;
	delay_us(1);
	ADF4351_CLK = 0;
	ADF4351_LE = 0;
	delay_us(1);
	
	for(i = count; i>0; i--)
	{
		ValueToWrite = *(buf+i-1);
		for(j=0; j<8; j++)
		{
			if(0x80 == (ValueToWrite & 0x80))
			{
				ADF4351_OUTPUT_DATA = 1;
			}
			else
			{
				ADF4351_OUTPUT_DATA = 0;
			}
			delay_us(1);
			ADF4351_CLK = 1;
			delay_us(1);
			ValueToWrite <<= 1;
			ADF4351_CLK = 0;	
		}
	}
	ADF4351_OUTPUT_DATA = 0;
	delay_us(1);
	ADF4351_LE = 1;
	delay_us(1);
	ADF4351_LE = 0;

}
void ReadToADF4351(u8 count, u8 *buf)
{
	u8 i = 0;
	u8 j = 0;
	u8 iTemp = 0;
	u8 RotateData = 0;
	
	ADF_Input_GPIOInit();
	ADF4351_CE = 1;
	delay_us(1);
	ADF4351_CLK = 0;
	ADF4351_LE = 0;
	delay_us(1);
	
	for(i = count; i>0; i--)
	{
		for(j = 0; j<8; j++)
		{
			RotateData <<=1;
			delay_us(1);
			iTemp = ADF4351_INPUT_DATA;
			ADF4351_CLK = 1;
			if(0x01 == (iTemp&0x01))
			{
				RotateData |= 1;
			}
			delay_us(1);
			ADF4351_CLK = 0;
		}
		*(buf+i-1) = RotateData;
	}
	delay_us(1);
	ADF4351_LE = 1;
	delay_us(1);
	ADF4351_LE = 0;
}
void ADF4351Init(void) {
	
	u8 buf[4]={0,0,0,0};
	
	ADF_Output_GPIOInit();
	
	buf[3] = 0x00;
	buf[2] = 0x58;
	buf[1] = 0x00;				//write communication register 0x00580005 to control the progress 
 	buf[0] = 0x05;				//to write Register 5 to set digital lock detector
	WriteToADF4351(4,buf);		

	buf[3] = 0x00;
	buf[2] = 0xEc;				//(DB23=1)The signal is taken from the VCO directly;(DB22-20:4H)the RF divider is 16;(DB19-12:50H)R is 80
	buf[1] = 0x80;				//(DB11=0)VCO powerd up;
 	buf[0] = 0x3C;				//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	WriteToADF4351(4,buf);		

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x04;				//(DB14-3:96H)clock divider value is 150.
 	buf[0] = 0xB3;
	WriteToADF4351(4,buf);	

	buf[3] = 0x00;
	buf[2] = 0x01;				//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
	buf[1] = 0x0E;				//(DB8=0)enable fractional-N digital lock detect;
 	buf[0] = 0x42;				//(DB12-9:7H)set Icp 2.50 mA;
	WriteToADF4351(4,buf);		//(DB23-14:1H)R counter is 1

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0xE1;			   //(DB14-3:6H)MOD counter is 6;
 	buf[0] = 0xA9;			   //(DB26-15:6H)PHASE word is 1,neither the phase resync 
	WriteToADF4351(4,buf);	   //nor the spurious optimization functions are being used
							   //(DB27=1)prescaler value is 8/9

	buf[3] = 0x00;
	buf[2] = 0x33;
	buf[1] = 0x42;
 	buf[0] = 0xc8;				//(DB14-3:0H)FRAC value is 0;
	WriteToADF4351(4,buf);		//(DB30-15:140H)INT value is 320;
}

void WriteOneRegToADF4351(u32 Regster)
{
	u8 buf[4] = {0,0,0,0};
	buf[3] = (u8)((Regster>>24)&(0X000000FF));
	buf[2] = (u8)((Regster>>16)&(0X000000FF));
	buf[1] = (u8)((Regster>>8) &(0X000000FF));
	buf[0] = (u8)((Regster)&(0X000000FF));
	WriteToADF4351(4,buf);	
}
void ADF4351_Init_some(void)
{
	WriteOneRegToADF4351(ADF4351_R2);
	WriteOneRegToADF4351(ADF4351_R3);
	WriteOneRegToADF4351(ADF4351_R5);
}

void ADF4351WriteFreq(float Fre)		//	fre单位MHz -> (xx.x) M Hz
{
	u16 Fre_temp, N_Mul = 1, Mul_Core = 0;
	u16 INT_Fre, Frac_temp, Mod_temp, i;
	u32 W_ADF4351_R0 = 0, W_ADF4351_R1 = 0, W_ADF4351_R4 = 0;
	float multiple;
	
	if(Fre < 35.0)
		Fre = 35.0;
	if(Fre > 4400.0)
		Fre = 4400.0;
	Mod_temp = 1000;
	Fre = ((float)((u32)(Fre*100)))/100;
	
	Fre_temp = Fre;
	for(i = 0; i < 10; i++)
	{
		if(((Fre_temp*N_Mul) >= 2199.9) && ((Fre_temp*N_Mul) <= 4400.1))
			break;
		Mul_Core++;
		N_Mul = N_Mul*2;
	}
	
	multiple = (Fre*N_Mul)/25;		//25：鉴相频率，板载100M参考，经寄存器4分频得25M鉴相。若用户更改为80M参考输入，需将25改为20；10M参考输入，需将25改为2.5，以此类推。。。
	INT_Fre = (u16)multiple;
	Frac_temp = ((u32)(multiple*1000))%1000;
	while(((Frac_temp%5) == 0) && ((Mod_temp%5) == 0))
	{
		Frac_temp = Frac_temp/5;
		Mod_temp = Mod_temp/5;
	}
	while(((Frac_temp%2) == 0)&&((Mod_temp%2) == 0))
	{
		Frac_temp = Frac_temp/2;
		Mod_temp = Mod_temp/2;
	}
	
	Mul_Core %= 7;
	W_ADF4351_R0 = (INT_Fre<<15)+(Frac_temp<<3);
	W_ADF4351_R1 = ADF4351_R1_Base + (Mod_temp<<3);
	W_ADF4351_R4 = ADF4351_R4_ON + (Mul_Core<<20);
	
	
//	WriteOneRegToADF4351(ADF4351_PD_OFF); //ADF4351_RF_OFF
//	WriteOneRegToADF4351((u32)(ADF4351_R4_OFF + (6<<20)));
	WriteOneRegToADF4351(ADF4351_RF_OFF);
	WriteOneRegToADF4351(W_ADF4351_R0);
	WriteOneRegToADF4351(W_ADF4351_R1);
	WriteOneRegToADF4351(W_ADF4351_R4);
//	WriteOneRegToADF4351(ADF4351_PD_ON);
}
extern u32 value_to_reg0;
extern	u32 value_to_reg1;
extern	u32 value_to_reg2;
extern	u32 value_to_reg3;
extern	u32 value_to_reg4;
extern	u32	value_to_reg5;

float Write_4351Frequency(float fre) {
	u16 div=64,f=1;//RFout分频系数和通道分辨率
	u16 INT,FRAC,MOD;	
	float N,dema;//N分频系数和N的小数部分

	
//	N=INT+(float)FRAC/MOD;
	N=(float)fre*2.56;//25为100M4分频
	INT=(int)N;
	dema=N-INT;
	if(dema!=0)
	{
//		if(dema*5==(int)(dema*5))
//		{
//			MOD=5;
//			FRAC=dema*5;
//		}
//		else if(dema*25==(int)(dema*25))
//		{
//			MOD=25;
//			FRAC=dema*25;
//		}
//		else if(dema*125==(int)(dema*125))
//		{
//			MOD=125;
//			FRAC=dema*125;
//		}
//		else if(dema*625==(int)(dema*625))
//		{
//			MOD=625;
//			FRAC=dema*625;
//		}
//	}
		MOD=3125;
		FRAC=(int)3125*dema;
	}
	else if(dema==0)
	{
		MOD=2;
		FRAC=0;
	}
	value_to_reg0 &= ~(0xFFFF<<15);//清除 INT
	value_to_reg0 &= ~(0xFF3 <<3);//清除	FRAC
	value_to_reg1 &= ~(0xFFF <<3);//清除	MOD
	value_to_reg4 &= ~(0x7 <<20);//清除	DIV
	
	value_to_reg0 |= (INT<<15);// INT
	value_to_reg0 |= (FRAC <<3);//	FRAC
	value_to_reg1 |= (MOD <<3);//	MOD
		switch(div)
	{
		case 1:value_to_reg4  = value_to_reg4 |(0x00<<20);break;	//div = 1
		case 2:value_to_reg4   |= (0x01<<20);break;	//div = 2
		case 4:value_to_reg4   |= (0x02<<20);break;	//div = 4
		case 8:value_to_reg4   |= (0x03<<20);break;	//div = 8
		case 16:value_to_reg4  |= (0x04<<20);break;	//div = 16
		case 32:value_to_reg4  |= (0x05<<20);break;	//div = 32
		case 64:value_to_reg4  |= (0x06<<20);break;	//div = 64
	}
	WriteOneRegToADF4351(ADF4351_RF_OFF);
	WriteOneRegToADF4351(value_to_reg0);
	WriteOneRegToADF4351(value_to_reg1);
	WriteOneRegToADF4351(value_to_reg4);
}