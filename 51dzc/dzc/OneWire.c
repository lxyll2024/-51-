#include <REGX52.H>

sbit OneWire_DQ=P3^7;  //总线

unsigned char OneWire_Init(void) //时序初始化
{
	unsigned char i;
	unsigned char AckBit;  		//应答位
	
	OneWire_DQ = 1;	  				//总线先拉高在拉低让他进入时序
	OneWire_DQ = 0;
	i = 227;while (--i);  		//Delay 500us微秒
	OneWire_DQ = 1;   				//由图知之后就释放单总线
	i = 29;while (--i);				//Delay 70us微秒
	AckBit=OneWire_DQ;        //应答
	i = 227;while (--i);       //Delay 500us微秒
	return AckBit;
	
}

void OneWire_SendBit(unsigned char Bit)  //发送一位Bit
{
	unsigned char i;
	OneWire_DQ = 0;         //拉低来进入时序
	i = 3;while (--i);			//Delay 10us微秒
	OneWire_DQ = Bit;
	i = 22;	while (--i);    //Delay 50us微秒
	OneWire_DQ = 1;          //拉高结束
}

unsigned char OneWire_ReceveBit(void)         //获取Bit
{
	unsigned char i;
	unsigned char Bit;
		OneWire_DQ = 0;  
		i = 1;while (--i);  						//Delay 5us微秒
		OneWire_DQ = 1;   				//由图知之后就释放单总线
		i = 1;while (--i);  						//Delay 5us微秒
		Bit = OneWire_DQ;              //采样
	 	i = 22;	while (--i);    //Delay 50us微秒
			return Bit;
	
	return Bit;
}

void OneWire_SendByte(unsigned char Byte)        //发送字节
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		OneWire_SendBit(Byte & (0x01<<i));  //依次取出来得到完整字节发送
	}

}

unsigned char OneWire_ReceveByte(void)	//接收字节
{
	unsigned char i;
	unsigned char Byte=0x00;
		for(i=0;i<8;i++)
	{
		if(OneWire_ReceveBit()){Byte|=(0x01<<i);}  //如果函数有一则赋1到上面没有就不成立来获取字节
	}
	return Byte;
}