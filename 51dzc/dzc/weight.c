#include <REGX52.H>
#include "uart.h"
#include "ds1302.h"
#include "hx711.h"
#include "buzzer.h"
#include "lcd1602.h"
#include "at24c02.h"
#include "delay.h"

sbit LED=P2^4;
unsigned int Weight_Maopi = 0,Range=1000,qupi,num;
long Weight_Shiwu = 0;
unsigned char num_Store,Weight_Store_Num=0,mode,m4;
unsigned char ERROR1,Bad_Num1,Keynum,TimeSetSelect=7,TimeSetFlashFlag;
unsigned char Weight_Shiwu_Flag1,num_Flag,Weight_Shiwu_Flag2;
unsigned int Weight_Standard1,FanWei_Max,FanWei_Min;
int Weight_Standard=200;
char temp,m3,ERROR,Bad_Num,sum,Month,Day,Hour,Minute,m3_Flag;
//unsigned int Weight_Store[5]={0},Bad[5]={0};
unsigned char i,Weight_Flag,KeynumFlag1,k8,m3_qingling;
float T;
//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数
#define GapValue 426.04

/*---------------------------------------
//毛皮重量
-----------------------------------------*/
void Get_Maopi() 
{
	//Weight_Maopi = HX711_Read();
  qupi = Weight_Shiwu;	
}
/*---------------------------------------
串口
-----------------------------------------*/
void UART_Send_num()
{
                    Send_Word("last time:\r\n");
                    UART_SendByte(Month/10+0x30);
                    UART_SendByte(Month%10+0x30);
                    Send_Word("-");
                    UART_SendByte(Day/10+0x30);
                    UART_SendByte(Day%10+0x30);
                    Send_Word(" ");
                    UART_SendByte(Hour/10+0x30);
                    UART_SendByte(Hour%10+0x30);
                    Send_Word(":");
                    UART_SendByte(Minute/10+0x30);
                    UART_SendByte(Minute%10+0x30);
                    Send_Word("\r\n");
                    Send_Word("last standard:\r\n");
                    Send_Word("error: ");
                    UART_SendByte(ERROR1/10+0x30);
                    UART_SendByte(ERROR1%10+0x30);
                    Send_Word(" weight: ");
                    UART_SendByte(Weight_Standard1/1000+0x30);  //千位
                    UART_SendByte(Weight_Standard1%1000/100+0x30);
                    UART_SendByte(Weight_Standard1%100/10+0x30);
                    UART_SendByte(Weight_Standard1%10+0x30);
                    Send_Word("\r\n");
                    Send_Word("last data:\r\n");
                    Send_Word("sum: ");
                    UART_SendByte(sum/100+0x30);
                    UART_SendByte(sum%100/10+0x30);
                    UART_SendByte(sum%10+0x30);
                    Send_Word(" bad: ");
                    UART_SendByte(Bad_Num1/100+0x30);
                    UART_SendByte(Bad_Num1%100/10+0x30);
                    UART_SendByte(Bad_Num1%10+0x30);
                    Send_Word("\r\n");
                    //循环一下发送数组里的数
}
/*---------------------------------------
时间展示
-----------------------------------------*/
void Timeshow(void)
{
    DS1302_ReadTime();

    LCD_ShowNum(1,9,DS1302_Time[0],2);
    LCD_ShowNum(1,12,DS1302_Time[1],2);
    LCD_ShowNum(1,15,DS1302_Time[2],2);
    LCD_ShowNum(2,9,DS1302_Time[3],2);
    LCD_ShowNum(2,12,DS1302_Time[4],2);
    LCD_ShowNum(2,15,DS1302_Time[5],2);
}
void TimeSet(void)
{

    if(Keynum==1)  //设置时间选择
    {
        TimeSetSelect--;
        if(TimeSetSelect==0)TimeSetSelect=6;
    }
    if(Keynum==5)   //加只要判断过大界限就行
    {
        DS1302_Time[TimeSetSelect-1]++;
        if(DS1302_Time[0]>99){DS1302_Time[0]=0;}
        if(DS1302_Time[1]>12){DS1302_Time[1]=1;}
        if(DS1302_Time[1]==1 || DS1302_Time[1]==3 || DS1302_Time[1]==5 || DS1302_Time[1]==7 || DS1302_Time[1]==8
           || DS1302_Time[1]==10 || DS1302_Time[1]==12)
        {
            if(DS1302_Time[2]>31){DS1302_Time[2]=1;}
        }
        else if(DS1302_Time[1]==4 || DS1302_Time[1]==6 ||DS1302_Time[1]==9 ||DS1302_Time[1]==11)
        {
            if(DS1302_Time[2]>30){DS1302_Time[2]=1;}
        }
        else if(DS1302_Time[1]==2)
        {
            if(DS1302_Time[0]%4==0) //闰年
            {
                if(DS1302_Time[2]>29){DS1302_Time[2]=1;}
            }
            else
            {
                if(DS1302_Time[2]>28){DS1302_Time[2]=1;}
            }
        }
        if(DS1302_Time[3]>23){DS1302_Time[3]=0;}
        if(DS1302_Time[4]>59){DS1302_Time[4]=0;}
        if(DS1302_Time[5]>59){DS1302_Time[5]=0;}
    }
    if(Keynum==6)  //减要判断过大和过小界限
    {
        DS1302_Time[TimeSetSelect-1]--;
        if(DS1302_Time[0]<0){DS1302_Time[0]=99;}
        if(DS1302_Time[1]<1){DS1302_Time[1]=12;}
        if(DS1302_Time[1]==1 || DS1302_Time[1]==3 || DS1302_Time[1]==5 || DS1302_Time[1]==7 || DS1302_Time[1]==8
           || DS1302_Time[1]==10 || DS1302_Time[1]==12)
        {
            if(DS1302_Time[2]<1){DS1302_Time[2]=31;}
        }
        else if(DS1302_Time[1]==4 || DS1302_Time[1]==6 ||DS1302_Time[1]==9 ||DS1302_Time[1]==11)
        {
            if(DS1302_Time[2]<1){DS1302_Time[2]=30;}
            if(DS1302_Time[2]>31){DS1302_Time[2]=1;}
        }
        else if(DS1302_Time[1]==2)
        {
            if(DS1302_Time[0]%4==0) //闰年
            {
                if(DS1302_Time[2]<1){DS1302_Time[2]=29;}
                if(DS1302_Time[2]>29){DS1302_Time[2]=1;}
            }
            else
            {
                if(DS1302_Time[2]<1){DS1302_Time[2]=28;}
                if(DS1302_Time[2]>28){DS1302_Time[2]=1;}
            }
        }
        if(DS1302_Time[3]<0){DS1302_Time[3]=23;}
        if(DS1302_Time[4]<0){DS1302_Time[4]=59;}
        if(DS1302_Time[5]<0){DS1302_Time[5]=59;}

    }
    if(TimeSetFlashFlag==1 && (TimeSetSelect-1)==0){LCD_ShowString(1,9,"  ");}
    else{LCD_ShowNum(1,9,DS1302_Time[0],2);}
    if(TimeSetFlashFlag==1 && (TimeSetSelect-1)==1){LCD_ShowString(1,12,"  ");}
    else{LCD_ShowNum(1,12,DS1302_Time[1],2);}
    if(TimeSetFlashFlag==1 && (TimeSetSelect-1)==2){LCD_ShowString(1,15,"  ");}
    else{LCD_ShowNum(1,15,DS1302_Time[2],2);}
    if(TimeSetFlashFlag==1 &&(TimeSetSelect-1)==3){LCD_ShowString(2,9,"  ");}
    else{	LCD_ShowNum(2,9,DS1302_Time[3],2);}
    if(TimeSetFlashFlag==1 &&(TimeSetSelect-1)==4){LCD_ShowString(2,12,"  ");}
    else{LCD_ShowNum(2,12,DS1302_Time[4],2);}
    if(TimeSetFlashFlag==1 && (TimeSetSelect-1)==5){LCD_ShowString(2,15,"  ");}
    else{LCD_ShowNum(2,15,DS1302_Time[5],2);}
}
/*---------------------------------------
储存的
-----------------------------------------*/
void AT24C02_Readbyte_Data()
{
 	sum=AT24C02_Readbyte(0);
	Bad_Num1=AT24C02_Readbyte(1);
	Weight_Standard1=AT24C02_Readbyte(2);
	Weight_Standard1|=AT24C02_Readbyte(3)<<8;	//高位16
	ERROR1=AT24C02_Readbyte(4);
	Month=AT24C02_Readbyte(5);
	Day=AT24C02_Readbyte(6);
	Hour=AT24C02_Readbyte(7);
	Minute=AT24C02_Readbyte(8);
}
void AT24C02_Writebyte_Data()
{
							AT24C02_Writebyte(0,sum);
							Delay(5);
							AT24C02_Writebyte(1,Bad_Num);
							Delay(5);
							AT24C02_Writebyte(2,Weight_Standard%256); //低位
							Delay(5);
							AT24C02_Writebyte(3,Weight_Standard/256); //高位
							Delay(5);
							AT24C02_Writebyte(4,ERROR);
							Delay(5);
							AT24C02_Writebyte(5,Month);//月日时分
							Delay(5);
							AT24C02_Writebyte(6,Day);
							Delay(5);
							AT24C02_Writebyte(7,Hour);
							Delay(5);
							AT24C02_Writebyte(8,Minute);
							Delay(5);
							AT24C02_Writebyte(9,m3_Flag);
							Delay(5);
}
void AT24C02_Writebyte_qingling_data()
{
							AT24C02_Writebyte(0,0);
							Delay(5);
							AT24C02_Writebyte(1,0);
							Delay(5);
							AT24C02_Writebyte(2,0); //低位
							Delay(5);
							AT24C02_Writebyte(3,0); //高位
							Delay(5);
							AT24C02_Writebyte(4,0);
							Delay(5);
							AT24C02_Writebyte(5,0);//月日时分
							Delay(5);
							AT24C02_Writebyte(6,0);
							Delay(5);
							AT24C02_Writebyte(7,0);
							Delay(5);
							AT24C02_Writebyte(8,0);
							Delay(5);
}
void AT24C02_Writebyte_qingling()
{
							AT24C02_Writebyte(0,0);
							Delay(5);
							AT24C02_Writebyte(1,0);
							Delay(5);
							AT24C02_Writebyte(2,0); //低位
							Delay(5);
							AT24C02_Writebyte(3,0); //高位
							Delay(5);
							AT24C02_Writebyte(4,0);
							Delay(5);
							AT24C02_Writebyte(5,0);//月日时分
							Delay(5);
							AT24C02_Writebyte(6,0);
							Delay(5);
							AT24C02_Writebyte(7,0);
							Delay(5);
							AT24C02_Writebyte(8,0);
							Delay(5);
							AT24C02_Writebyte(9,0);
							Delay(5);
}
/*---------------------------------------
显示
-----------------------------------------*/
void Display_M2()
{				
	LCD_ShowString(1,1,"MODE 2 MAX:");
	LCD_ShowNum(1,12,Range,4);
	LCD_ShowString(1,16,"G");
	LCD_ShowString(2,1,"WEIGHT:");
	LCD_ShowString(2,16,"G");
	LCD_ShowNum(2,12,Weight_Shiwu,4);

}
void Display_M1()
{
	LCD_ShowString(1,9,"  -  -  ");
	LCD_ShowString(2,9,"  :  :  ");
	LCD_ShowString(1,1,"MODE 1  ");
}
void Display_qingping()
{
	LCD_ShowString(1,1,"                ");
	LCD_ShowString(2,1,"                ");
}
void Display_M3()
{
	LCD_ShowString(1,1,"MODE 3 ERROR:");
	LCD_ShowNum(1,14,ERROR,2);
	LCD_ShowString(1,16,"G");
	LCD_ShowString(2,1,"WEIGHT:");
	LCD_ShowString(2,16,"G");
	LCD_ShowNum(2,12,Weight_Standard,4);
	
}
void Display_M3_Measure()
{
	LCD_ShowString(1,1,"MODE 3 NUM:");
	LCD_ShowNum(1,12,num,3);
	LCD_ShowString(2,1,"WEIGHT:");
	LCD_ShowString(2,16,"G");
	LCD_ShowNum(2,12,Weight_Shiwu,4);
	
}
void Display_M3_Store()
{
 LCD_ShowString(1,1,"save data ...");
}
void Display_M3_NotStore()
{
 LCD_ShowString(1,1,"not save data");
}
void Display_M4_Standard()
{
	LCD_ShowString(1,1,"Weight:");
	LCD_ShowNum(1,8,Weight_Standard1,4);
	LCD_ShowString(1,12,"G");
	LCD_ShowString(2,1,"ERROR:");
	LCD_ShowNum(2,7,ERROR1,2);	
	LCD_ShowString(2,9,"G");
}
void Display_M4_Num()
{
	LCD_ShowString(1,1,"SUM:");
	LCD_ShowNum(1,5,sum,3);
	LCD_ShowString(2,1,"BadNum:");
	LCD_ShowNum(2,8,Bad_Num1,3);	
}
void Display_M4_Send()
{
	LCD_ShowString(1,1,"SEND DATA ?");
	LCD_ShowString(2,1,"YES OR NO");
}
void Display_M4_successful()
{
	LCD_ShowString(1,1,"send successfully");
}
void Display_M4_Time()
{
	LCD_ShowString(1,1,"The Last Time:");
	LCD_ShowNum(2,1,Month,2);
	LCD_ShowString(2,3,"-");
	LCD_ShowNum(2,4,Day,2);	
	LCD_ShowString(2,6,"  ");
	LCD_ShowNum(2,8,Hour,2);
	LCD_ShowString(2,10,":");
	LCD_ShowNum(2,11,Minute,2);
}
void Display_M4_UART()
{
	LCD_ShowString(1,1,"SANDING DATA...");
}

/*---------------------------------------
//获取重量
-----------------------------------------*/
void Get_Weight()
{
	Weight_Shiwu = HX711_Read();  //HX711读取
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//获取净重
	Weight_Shiwu = (unsigned int)((float)(Weight_Shiwu)/GapValue)-qupi; 	//计算实物的实际重量	
	Weight_Shiwu -= 18600;
		if(Weight_Shiwu<=0 ||Weight_Shiwu>5000)
		{
		//LED=1;
		Weight_Shiwu=0;
		}
		if(Weight_Shiwu>0 && Weight_Shiwu<=5000)
		{
			//LED=1;	
			if(Weight_Shiwu > Range && mode==2)		//超重报警
			{
				Buzzer_Time(100);
				//LED=!LED;    				//LED灯取反
				LED=0;
			}
				else if(mode==2 && Weight_Shiwu <= Range)
				{
					LED=1;
				}	
		}
}
