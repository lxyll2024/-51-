#include <REGX52.H>
#include "onewire.h"

#define DS18B20_SKIP_ROM  0xcc   		//转化温度暂存区指令
#define DS18B20_CONVERT_T  0x44
#define DS18B20_READ_SCRATCHPAD  0xBE


void DS18B20_ConvertT(void)  //转换温度
{
	OneWire_Init(); 
	OneWire_SendByte(DS18B20_SKIP_ROM);
	OneWire_SendByte(DS18B20_CONVERT_T);
}

float DS18B20_ReadT(void)  //读取温度
{
	unsigned char TLSB,TMSB;
	int Temp;
	float T;
		OneWire_Init(); 
		OneWire_SendByte(DS18B20_SKIP_ROM);
		OneWire_SendByte(DS18B20_READ_SCRATCHPAD);  //读取温度
		
		TLSB=OneWire_ReceveByte();    //接收该温度 低位
		TMSB=OneWire_ReceveByte();    //拿出温度字节的数据 高位
		Temp=(TMSB<<8)|TLSB;    //完整的温度值
		T = Temp/16.0;   //实际温度,具体看图,防止损失精度
		return T;
	
}

