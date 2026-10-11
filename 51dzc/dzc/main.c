#include <REGX52.H>
#include "stdio.h"
#include "ds1302.h"
#include "lcd1602.h"
#include "key.h"
#include "at24c02.h"
#include "uart.h"
#include "delay.h"
#include "timer0.h"
#include "weight.h"
#include "buzzer.h"
#include "ds18b20.h"
sbit LED=P2^4;
//int Sum_num[10]={0};
void main()
{
    LCD_Init();
    DS1302_Init();
		DS1302_SetTime();
    Timer0Init();
		DS18B20_ConvertT();//转换温度,为了更新温度读取第一次的值准确一点
				Delay(1000);
    
    Display_M1();
		
    URAT_Init(9600);
    m3_Flag=AT24C02_Readbyte(9);
    while(1)                       //BCD码
    {
       DS18B20_ConvertT();	//转换温度
			T=DS18B20_ReadT();	 //读取温度
//         Get_Weight();		//获取重量
        Keynum = Key();
				Key_deal();
    }
}

void Timer0Routine() interrupt 1//1是T0计时器的中断
{
static unsigned int T0Count1,T0Count2;
TL0 = 0x66;		//设置定时初值
TH0 = 0xFC;		//设置定 时初值
T0Count1++;
if(T0Count1>=500)
{
T0Count1=0;
TimeSetFlashFlag = !TimeSetFlashFlag;  //时间设置闪烁标志位，！逻辑取反~按位取反，即这是1变0变1的取反

}
T0Count2++;
if(T0Count2>=1000)
{
Weight_Flag=1;

T0Count2=0;
//Get_Weight();  //时间设置闪烁标志位，！逻辑取反~按位取反，即这是1变0变1的取反

}
}
void URAT() interrupt 4
{

if(RI)
{
temp=SBUF;
SBUF=temp;
RI = 0;
}
}