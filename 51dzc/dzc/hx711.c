#include "HX711.h"


//****************************************************
//延时
//****************************************************
void Delay__hx711_us(void)
{
	_nop_();
	_nop_();
}

//****************************************************
//HX711读取
//****************************************************
unsigned long HX711_Read(void)	//增益128
{
	unsigned long count;  //2^24 防止外溢
	unsigned char i; 
  	HX711_DOUT=1;       //置高AD转换 
	Delay__hx711_us();    //延时
  	HX711_SCK=0; 				//使能AD（PD_SCK 置低）写入
  	count=0;
	EA = 1;                  //打开中断
  	while(HX711_DOUT);    //AD转换未结束则等待，否则开始读取
	EA = 0;									//关闭中断
  	for(i=0;i<24;i++)      //2^24count移位24次 
	{ 
	  	HX711_SCK=1;      //PD_SCK 置高（发送脉冲）
	  	count=count<<1; 		 //下降沿来时变量Count左移一位，右侧补零
		HX711_SCK=0;         //使能AD（PD_SCK 置低）写入
	  	if(HX711_DOUT)     //置高AD转换 
			count++; 
	} 
 	HX711_SCK=1;   					    //PD_SCK 置高（发送脉冲）
    count=count^0x800000;			//第25个脉冲下降沿来时，转换数据
	Delay__hx711_us();	   			//延时
	HX711_SCK=0;  							//使能AD（PD_SCK 置低）写入
	return(count);
}