#ifndef __HX711_H__
#define __HX711_H__


#include <REGX52.H>
#include <intrins.h>

//IO设置
sbit HX711_DOUT=P2^1; 
sbit HX711_SCK=P2^0;
//函数或者变量声明

extern void Delay__hx711_us(void);  //711延时
extern unsigned long HX711_Read(void);		//711读取重量

#endif