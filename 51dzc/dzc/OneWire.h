#ifndef __onewire_H__
#define __onewire_H__

unsigned char OneWire_Init(void);           //时序初始化
void OneWire_SendBit(unsigned char Bit);   //发送一位Bit
unsigned char OneWire_ReceveBit(void);        //获取Bit
void OneWire_SendByte(unsigned char Byte);    //发送字节BYTE固定指令
unsigned char OneWire_ReceveByte(void);				//接收字节


#endif