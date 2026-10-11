#include <REGX52.H>
#include "Delay.h"
#include "ds1302.h"
#include "weight.h"
#include "lcd1602.h"
sbit DS1302_SCLK = P3^6;  //看原理图ds1302模块的3个代表定义方便查看,防止模块冲突
sbit DS1302_IO = P3^4;
sbit DS1302_CE = P3^5;  //单片机上电是默认1
sbit LED=P2^4;
//unsigned char i;
//获取独立按键建码，范围0-4，无按键按时返回0
unsigned char Key()
{
	
	unsigned char Keynum=0;
	P1_7=0;
	
	if(P3_1==0){Delay(20);while(P3_1==0);Delay(20);Keynum=1;}
	if(P3_0==0){Delay(20);while(P3_0==0);Delay(20);Keynum=2;}
	if(P3_2==0){Delay(20);while(P3_2==0);Delay(20);Keynum=3;}
	if(P3_3==0){Delay(20);while(P3_3==0);Delay(20);Keynum=4;}
	
	if(P1_3==0){Delay(20);while(P1_3==0);Delay(20);Keynum=5;}
	if(P1_2==0){Delay(20);while(P1_2==0);Delay(20);Keynum=6;}
	if(P1_1==0){Delay(20);while(P1_1==0);Delay(20);Keynum=7;}
	if(P1_0==0){Delay(20);while(P1_0==0);Delay(20);Keynum=8;}
	
	return Keynum;
}

void Key_deal() //按键处理
{
			if(mode == 0 && Keynum==8)
			{
					DS1302_CE = 1;
					DS1302_SCLK = 1;
				mode=6;
				Keynum=0;
				Display_qingping();
				LCD_ShowString(1,1,"Temperature:");
				

			}
			
				if(mode==6)
			{
				if(Keynum==8)
				{
					DS1302_CE = 0;
					DS1302_SCLK = 0;
				Display_qingping();
				Display_M1();
				mode=0;
				}
				
			}

        if(Keynum==1 && (mode == 0 || mode == 1))			//设置运行模式选择
        {

            i++;
            if(mode == 0)
            {
                mode = 1;
            } //mode=0时间展示界面
            else if(i>6)
            {
                i=0;
            }
            if(mode == 1 && i==0)
            {
                mode = 0;
                DS1302_SetTime();
            }  //调时间后调好在重新写入
        }

        if(Keynum==2 && (mode == 0 || mode == 2)) //按键2转换界面
        {
            Display_qingping();
            mode+=2;mode%=4;
            if(mode == 0)
            {
                qupi=0;
                Display_M1();
            }
        }
        if(mode == 2)
        {
            if(Keynum==5){Range+=100;if(Range>5000){Range=100;}}
            if(Keynum==6){Range-=100;if(Range==0){Range=5000;}}
            if(Keynum==7)
            {
                Get_Maopi();
            }
        }
        if(Keynum==3 && (mode == 0||mode == 3))
        {
            Display_qingping();
            mode = 3;
            m3++;
					if(m3==1){AT24C02_Writebyte_qingling();}
            if(m3>3){m3=0;}
        }
        if(mode==3)
        {
            if(m3==0)
            {
								m3_qingling=0;
                Display_M3_Store();
                Delay(1000);
                Display_qingping();
                Display_M1();
                mode=0;

							sum=Weight_Store_Num + Bad_Num;
                Month=DS1302_Time[1];
                Day=DS1302_Time[2];
                Hour=DS1302_Time[3];
                Minute=DS1302_Time[4];
                
						AT24C02_Writebyte_Data();
            }
            if(m3==1) //调整标重
            {
							if(m3_qingling==0){AT24C02_Writebyte_qingling_data();m3_qingling=1;}
                m3_Flag=1;
                if(Keynum==5){Weight_Standard+=200;if(Weight_Standard>5000){Weight_Standard=200;}}
                if(Keynum==6){Weight_Standard-=200;if(Weight_Standard<=0){Weight_Standard=5000;}}
            }
            if(m3==2) //调整容错
            {
                if(Keynum==5){ERROR++;if(ERROR>10){ERROR=0;}}
                if(Keynum==6){ERROR--;if(ERROR>10){ERROR=10;}}
            }
            if(m3==3) //开始测量
            {
								
                if(Weight_Standard==0)
                {
                    Weight_Standard=200;
                }
                else
                {
                    num=(unsigned int)(Weight_Shiwu/Weight_Standard);
                }

                if(num==0 && Weight_Shiwu!=0)
                {
                    num=1;
                }
                FanWei_Min=Weight_Standard-ERROR;
                FanWei_Max=Weight_Standard+ERROR;
                if(Weight_Flag==1) //每1秒判断储存一次
                {
                    num_Flag++;
                    if((Weight_Shiwu/num)<=FanWei_Max && (Weight_Shiwu/num)>=FanWei_Min)
                    {
                        Weight_Shiwu_Flag1=Weight_Shiwu;
                    }

                    if(((Weight_Shiwu/num)<FanWei_Min || (Weight_Shiwu/num)>FanWei_Max)&& Weight_Shiwu!=0)
                    {
                        //Bad[Bad_Num]=Weight_Shiwu;//== mode=0;
                        Bad_Num++;
                        LED=!LED;
											

                    }
                    if(Weight_Shiwu_Flag2==Weight_Shiwu_Flag1 && Weight_Shiwu_Flag2!=0)
                    {
                       //Weight_Store[Weight_Store_Num] = Weight_Shiwu_Flag2;

                        Weight_Store_Num++;
                        if(Weight_Store_Num>5)
												{Weight_Store_Num=0;}
                        LED=!LED;
                        
                    }

                    Weight_Flag=0;
                }
                if(num_Flag%2 != 0)
                {
                    Weight_Shiwu_Flag2=Weight_Shiwu_Flag1;
                }

            }
        }

        if(Keynum==4 && mode==0 && m3_Flag==0) //未储存数据
        {
            Display_qingping();
            Display_M3_NotStore();
            Delay(1000);
            Display_qingping();
            Display_M1();
        }
        if(Keynum==4 && mode==0 && m3_Flag==1) //储存了数据
        {
            mode=5;
           // m3_Flag=0;
        }
        if(mode==5 && (Keynum==4|| Keynum==8))
        {
						AT24C02_Readbyte_Data();
					if(sum<Bad_Num1){sum=Bad_Num1;}
            Display_qingping();
					if(Keynum==4)
					{m4++;}
            if(m4==4)
            {
                if(Keynum==8)
                {
									UART_Send_num();
                    Display_qingping();
                    Display_M4_UART();//已发送数据显示
                    Delay(1000);
                    Display_qingping();
										LED=!LED;
										k8=1;
                }
            }//发送数据
            if(m4==5)
            {			
                Display_M1();
                mode=0;
                m4=0;
							k8=0;
            }
            switch(m4)
            {
                case 1:Display_M4_Time();break;
                case 2:Display_M4_Standard();break;
                case 3:Display_M4_Num();break;
                case 4:if(k8==0){ Display_M4_Send();}else{Display_M4_successful();}break;
            }
        }

        switch(mode)
        {
            case 0: Timeshow();break;
            case 1: TimeSet();break;
            case 2: Display_M2();Get_Weight();break;
            case 3: if(m3==3){Display_M3_Measure();Get_Weight();}else{Display_M3();Get_Weight();}break;
                //case 4: Display_M3_Measure();break;
            case 5: break;
					case 6:						if(T<0)
						{
							LCD_ShowChar(2,1,'-'); 
							T = -T;				
						}
						else
						{
							LCD_ShowChar(2,1,'+');
						}
						LCD_ShowNum(2,2,T,3);
						LCD_ShowString(2,5," C");break;
        }
}