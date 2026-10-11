#ifndef __ds1302_H__
#define __ds1302_H__

extern char DS1302_Time[];  //extern申明外部变量



void DS1302_Init(void);
void DS1302_WriteByte(unsigned char Command,Data);
unsigned char DS1302_ReadByte(unsigned char Command);
void DS1302_ReadTime(void);
void DS1302_SetTime(void);


#endif