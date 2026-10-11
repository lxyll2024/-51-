#include <REGX52.H>

void URAT_Init(baud)
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFD;		//设定定时初值
	TH1 = 0xFD;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	ES=1;
}
//串口发送1个字节
void UART_SendByte(unsigned char Byte)//发送字节
{
    SBUF=Byte;//发送字节
    while (TI==0);//如果TI=1就让它变回0，发送后电脑自动TI=1会向主机请求中断，手动软件关闭中断复位
    TI=0;//发送中断请求关闭
}
void Send_Word(unsigned char *p)				//发送字符串
{	
 	while(*p)
 	{
	 	SBUF=*(p++);
		while(TI==0)
		{
			;
			;
			;
		};
		TI=0;
	}   
}
//void UART_Init()//串口发送+T1定时器
//{																													       //   0    1   0  0   0    0  0  0
//    SCON=0x40;          //0100 0000串口工作模式1，T1与SMOD决定SMOD SM0/FE SM1 SM2 REN TB8 RB8 TI RI 不接收发送
//    PCON |= 0x80;		//使能波特率倍速位SMOD//最高位置1//4800波速率
//    //串口
//    TMOD &= 0x0F;		//清除定时器1模式位//定时器T1设置//高位置0//8位自动重组初始化
//    TMOD |= 0x20;		//设定定时器1为8位自动重装方式//开始设置
//    TL1 = 0xF4;		//设定定时初值//范围
//    TH1 = 0xF4;		//设定定时器重装值
//    ET1 = 0;		//禁止定时器1中断，分开关静止中断
//    TR1 = 1;		//启动定时器1
//}
//TH1=256-11059200/32/12/9600
//void URAT_Init(baud)
//{
//	PCON &= 0x7F;		//波特率不倍速
//	SCON = 0x50;		//8位数据,可变波特率
//	TMOD &= 0x0F;		//清除定时器1模式位
//	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
//	TL1 = 0xFD;		//设定定时初值
//	TH1 = 0xFD;		//设定定时器重装值
//	ET1 = 0;		//禁止定时器1中断
//	TR1 = 1;		//启动定时器1
//	ES=1;
//}
////串口发送1个字节
//void senddata(char dat)
//{
//	SBUF = dat;
//	while(TI==0);
//	TI = 0;
//}
//void sendstring(unsigned char *str)
//{
//	while(*str!='\0')
//	{
//		senddata(*str++);
//	}
//}
//char putchar(char c)
//	{
//	//输出重定向到串口
//	senddata(c);
//	return c;//返回给函数调用者printf
//}
//void UART_SendByte(unsigned char Byte)//发送字节
//{
//    SBUF=Byte;//发送字节
//    while (TI==0);//如果TI=1就让它变回0，发送后电脑自动TI=1会向主机请求中断，手动软件关闭中断复位
//    TI=0;//发送中断请求关闭
//}
//void UART_Init()//串口发送+T1定时器
//{																													       //   0    1   0  1   0    0  0  0
//    SCON=0x50;          //0100 0000串口工作模式1，T1与SMOD决定SMOD SM0/FE SM1 SM2 REN TB8 RB8 TI RI （REN=1允许接收）
//    PCON |= 0x80;		//使能波特率倍速位SMOD//最高位置1//4800波速率					//sm1 sm2决定工作模式
//    //串口
//    TMOD &= 0x0F;		//清除定时器1模式位//定时器T1设置
//    TMOD |= 0x20;		//设定定时器1为8位自动重装方式
//    TL1 = 0xF4;		//设定定时初值//范围
//    TH1 = 0xF4;		//设定定时器重装值
//    ET1 = 0;		//禁止定时器1中断，分开关静止中断
//    TR1 = 1;		//启动定时器1
//		EA=1;//总开关启动中断
//		ES=1 ;  //启动串口中断
//}
/*************************
串口中断函数模版
***************************/
//void UART_Routine() interrupt 4//串口中断
//{
//    //P1=0x00;//要在缓冲区发送f0发送数据进入中断
//	if(RI==1)//接收中断请求标志RI会自动置1请求中断然后需软件复位
//	{

//		RI=0;
//	}
/***********************

***********************/
//串口初始化
//void UART_Init(unsigned int BAUD)//BAUD是波特率
//{																												//0    0   1  0   0   0   0  0  
//	TMOD=(TMOD&0x0f)|0x20;//TMOD定时器/计数器模式控制寄存器 GATE c/t m1 m0 GATE c/t m1 m0 低位T0计数器设置，高位T1计数器设置 //看定时器图具体
//	//GATE门控位，GATE=0时，TCON中的TRO或TR1为1，启动定时器/计数器工作
//						// GATE=1时,使TCON中的TRO或TR1为1，且需要外部引脚也为高电平，才能工作
//	//C/T：定时/计数模式切换，C/T=0时为定时模式，C/T=1时为计数模式
//	//M1，M0：用来选择定时计/计数器的工作方式，一般使用都是采用16位的计时计数器
//	//有4种模式，00//13位计时器 01//16位计时器 10//8位重组计时器 自动重新载入计数值 11//两组独立计时器，T1 T0不能同时用
//	//模式0-3       8192              65536						256																	256
//	//波特率9600晶振11.0592 SMOD=0
//	TH1=256-11059200/32/12/BAUD//=253=0xfd范围高值
//	TL1=TH1;
//	TR1=1;//定时器1开始计时
//	ET1=0;//定时器只做波特率发生器，不中断
//	SCON=0x50;//模式1  01010000 可变波特率 REN=1允许中断 串口模式
//	ES=1;//串口中断打开
//	PS=0;//设串口为低级中断
//}	
////PCON功率控制寄存器 SMOD SMOD0  ... SMOD=0  /32 不加倍 SMOD=1 /16 加倍
//									//  1    0    000
//void main()
//{
//	UART_Init(9600);//初始化串口
//	EA=1;//打开总开关
//	while(1);
//}
////发送函数
//void SendData(unsigned char dat)
//{
//	SBUF=dat;//写数据到UART数据寄存器
//	while(TI==0);//等待前面数据发送完成TI自动=1发生中断
//	TI=0;//手动关闭中断
//}
//文本模式ASCALL码值对应发送，HEX 16进制
////UART中断服务程序
//	void Uart() interrupt 4
//	{
//		if(RI)
//		{
//		RI=0;
//		temp=SBUF;
//		SBUF=temp;
//		}
//	}
//void helloword()
//{
//	senddata('L');
//	senddata('a');
//	senddata('s');
//	senddata('l');
//	senddata('o');
//	senddata('w');
//	senddata('o');
//	senddata('r');
//	senddata('d');
//}