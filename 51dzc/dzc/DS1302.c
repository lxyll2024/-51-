#include <REGX52.H>
#include "lcd1602.h"

sbit DS1302_SCLK = P3^6;  //看原理图ds1302模块的3个代表定义方便查看,防止模块冲突
sbit DS1302_IO = P3^4;
sbit DS1302_CE = P3^5;  //单片机上电是默认1

#define DS1302_SECOND   0x80  //second 秒写入
#define DS1302_MINUTE   0x82  //minute 分写入
#define DS1302_HOUR   	0x84  //hour 	 时写入
#define DS1302_DATE   	0x86  //date 	 日写入
#define DS1302_MONTH  	0x88  //month  月写入
#define DS1302_DAY  	 	0x8A  //day 	 周写入
#define DS1302_YEAR  	  0x8C  //year 	 年写入
#define DS1302_WP   		0x8E  //wp 		 保护写入



char DS1302_Time[] = {24,1,15,16,35,55,1}; //一个数组给24年1月15号16点35分55秒，星期一

void DS1302_Init(void) //初始化打开时序准备相当于
{
	DS1302_CE = 0;
	DS1302_SCLK = 0;
}
void DS1302_WriteByte(unsigned char Command,Data)
{
//	DS1302_CE = 1;
//	DS1302_IO = Command&0x01; //清零前7位，取出第0位控制字，控制字是指令
//	DS1302_SCLK = 1;
//	DS1302_SCLK = 0;
//	
//	DS1302_IO = Command&0x02; //0000 0010，取出第1位控制字
//	DS1302_SCLK = 1;
//	DS1302_SCLK = 0;
	unsigned char i;
	DS1302_CE = 1;
	for(i = 0;i<8;i++)
	{
		DS1302_IO = Command&(0x01<<i); 
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
	}
		for(i = 0;i<8;i++)
	{
		DS1302_IO = Data&(0x01<<i); 	//这里是数据
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
	}
	DS1302_CE = 0; //写入完成关闭时序CE
}


unsigned char DS1302_ReadByte(unsigned char Command)
{
	unsigned char i,Data=0x00;
	DS1302_CE = 1;
	Command|=0x01;    //给第一位置1
		for(i = 0;i<8;i++)
	{
		DS1302_IO = Command&(0x01<<i); 
		DS1302_SCLK = 0;
		DS1302_SCLK = 1;
	}
	for(i = 0;i<8;i++)
	{
	DS1302_SCLK = 1;
	DS1302_SCLK = 0;
		if(DS1302_IO){Data|=(0x01<<i);} 
	}
		DS1302_IO=0;  //建议放这里
		DS1302_CE = 0; //读完完成关闭时序CE
	
		return Data;
		//	if(DS1302_IO){Data=Data|0x01;}  //如果io口读出为1，数据获得变为0000 0001
//	DS1302_SCLK = 0;
//	DS1302_SCLK = 1;
//	if(DS1302_IO){Data=Data|0x02;}  // 相当于把读取后com的量读给data 
//	DS1302_SCLK = 0;
//	DS1302_SCLK = 1;
}

void DS1302_SetTime(void)  //写入时间
{
	DS1302_WriteByte(DS1302_WP,0x00); //关闭写保护
	DS1302_WriteByte(DS1302_YEAR,DS1302_Time[0]/10*16+DS1302_Time[0]%10);
		DS1302_WriteByte(DS1302_MONTH,DS1302_Time[1]/10*16+DS1302_Time[1]%10);
		DS1302_WriteByte(DS1302_DATE,DS1302_Time[2]/10*16+DS1302_Time[2]%10);
	DS1302_WriteByte(DS1302_HOUR,DS1302_Time[3]/10*16+DS1302_Time[3]%10);
		DS1302_WriteByte(DS1302_MINUTE,DS1302_Time[4]/10*16+DS1302_Time[4]%10);
		DS1302_WriteByte(DS1302_SECOND,DS1302_Time[5]/10*16+DS1302_Time[5]%10);
	DS1302_WriteByte(DS1302_DAY,DS1302_Time[6]/10*16+DS1302_Time[6]%10);
	DS1302_WriteByte(DS1302_WP,0x80);
}

void DS1302_ReadTime(void)  //读取时间
{
	unsigned char Temp;
	Temp = DS1302_ReadByte(DS1302_YEAR);
	DS1302_Time[0] = Temp/16*10+Temp%16;
		Temp = DS1302_ReadByte(DS1302_MONTH);
	DS1302_Time[1] = Temp/16*10+Temp%16;
		Temp = DS1302_ReadByte(DS1302_DATE);
	DS1302_Time[2] = Temp/16*10+Temp%16;
		Temp = DS1302_ReadByte(DS1302_HOUR);
	DS1302_Time[3] = Temp/16*10+Temp%16;
		Temp = DS1302_ReadByte(DS1302_MINUTE);
	DS1302_Time[4] = Temp/16*10+Temp%16;
		Temp = DS1302_ReadByte(DS1302_SECOND);
	DS1302_Time[5] = Temp/16*10+Temp%16;
		Temp = DS1302_ReadByte(DS1302_DAY);
	DS1302_Time[6] = Temp/16*10+Temp%16;
	
}
