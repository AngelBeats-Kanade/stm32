#ifndef _ADF4351_H_
#define _ADF4351_H_
#include "sys.h"

#define ADF4351_CE PBout(10)
#define ADF4351_LE PBout(11)
#define ADF4351_OUTPUT_DATA PBout(12)
#define ADF4351_CLK		PBout(13)

//										无用的管脚
#define ADF4351_INPUT_DATA PCin(2)

#define ADF4351_RF      _OFF	((u32)0XEC801C)

void ADF4351Init(void);
//void ReadToADF4351(u8 count, u8 *buf);
void WriteToADF4351(u8 count, u8 *buf);
void WriteOneRegToADF4351(u32 Regster);
void ADF4351_Init_some(void);
void ADF4351WriteFreq(float Fre);		//	(xx.x) M Hz
void ReadToADF4351(u8 count, u8 *buf);
float Write_4351Frequency(float fre);
#endif

