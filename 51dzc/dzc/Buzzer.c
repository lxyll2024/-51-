#include <REGX52.H>
#include "delay.h"
#include "intrins.h"
sbit Buzzer=P1^5;

void Buzzer_Delay500us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 227;
	while (--i);
}

void Buzzer_Time(unsigned int ms)
{
	unsigned int i;
				for(i=0;i<ms*2;i++)
			{
			Buzzer=!Buzzer; //1 和 0翻转,每一毫秒翻转一次高低电平都1毫秒，周期2毫秒频率500HZ,i<ms
				//Delay(1);  //就是500HZ频率响ms毫秒
				Buzzer_Delay500us(); //频率1000HZ
			}
}