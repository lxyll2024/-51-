#include <REGX52.H>
//双向二线制同步串行总线I2c总线
//时序图，6个拼图，组成一个数据帧
//AT24C02.c 可以建个模块，用来下载开关掉电后依旧可以储存 = e2prom
sbit I2C_SCL = P2^1;	//拉高,周期
sbit I2C_SDA = P2^0;	//1 0变化，接收发送

void I2C_Start(void)		//开始发送数据 初始化		I2C数据帧
{
		I2C_SDA = 1; 			//两条数据线都是高电平开始在给低电频为了能接上六个图 先1后0图呈下降趋势
		I2C_SCL = 1;			//同上，SCL = 1 期间SDA可下降，所以先SDA=0
		I2C_SDA = 0;			//两条线开始工作
		I2C_SCL = 0;			//同上
}
void I2C_Stop(void)		//停止，数据帧结束保证上升趋势0-1，但此时未知它是否在0，自己设所以
{
		I2C_SDA = 0;		//先置0变低，保证SCL处高电平期间才可以变化
		I2C_SCL = 1;		//变化条件
		I2C_SDA = 1;		//0-1上升趋势结束		
}
void I2C_Sendbyte(unsigned char byte)		//发送字节
{
	unsigned char i;
	for(i = 0;i < 8;i++)
	{																	//SDA开始是0，发送后有变化为1，没变化为0
		I2C_SDA = byte&(0x80>>i);		//与最高位比较，0为0,1为1,发送字节一共有八个，发送完才停，左移一个个比较是否为1
		I2C_SCL = 1;								//比较完确定数值，存的条件，SCL拉一下
		I2C_SCL = 0;						//拉高后立刻降低
	}
}

unsigned char I2C_Receivebyte(void)		//接收字节
{
		unsigned char i,byte = 0x00;
		
		I2C_SDA = 1;			//接收前先置1
	
		for(i = 0;i < 8;i++)
	{
		I2C_SCL = 1;			//拉高
		if(I2C_SDA){byte|= (0x80>>i);}		//如果读取1，把byte高位置1 ,前面发送的有1变化的读取接收
		I2C_SCL = 0;								//接收后变低关闭变动
	}
		return byte;			//得到完整的8位字节接收
	
}

void I2C_Sendack(unsigned char ackbit)		//应答位发送,1应答，0不应答
{
		I2C_SDA = ackbit;
		I2C_SCL = 1;		//SDA变化条件
		I2C_SCL = 0;
}

unsigned char I2C_Receiveack(void)		//应答接收
{
		unsigned char ackbit;		//默认为0
		I2C_SDA = 1;
		I2C_SCL = 1;	
		ackbit = I2C_SDA;		//SDA是位，接收应答
		I2C_SDA = 0;				//位清0
		I2C_SCL = 0;
		return ackbit;
}