#include <REGX52.H>
#include "i2c.h"

#define AT24C02_ADDRESS 0xA0  //重定义 w(写)地址 固定的
/*----------------------------------------*
//前面这些是为了能传数据成功类似//完成I2C配置的设置
//at24c02.c完成，下面测试步
------------------------------------------*/
//void AT24C02_Writebyte(unsigned char wordaddress,Data)		//输入写字节地址,字节数据先//写字节输入
//{
//		unsigned char ack;
//		I2C_Start();									//开始用数据传输
//		I2C_Sendbyte(AT24C02_ADDRESS);		//用数据发送（写）地址，存以后写的字节数据
//		ack = I2C_Receiveack();						//接收应答//按自己写的它是1的，但它测试变0了，说明给了应答电脑
//		if(ack == 0)P2 = 0x00;						//更改写地址发现它默认1，没有应答，也就是说写地址上有数据，会有应答		
//}
//unsigned char AT24C02_Readbyte(unsigned char wordaddress)
//{
//		unsigned char Data;
//	
//		return Data;
//}

/*---------------------------------------------*
数据帧，字节写data步骤
s(开始)->s（发送字节）:address+w(写地址(从机地址))->RA(接收应答)->
s（发送字节）:wordaddress（字节地址）->RA(接收应答)->
s（发送字节）:data->stop（结束）
-----------------------------------------------*/
void AT24C02_Writebyte(unsigned char wordaddress,Data)		//输入写字节地址,字节数据先//写字节输入
{
		I2C_Start();									//开始用数据传输
		I2C_Sendbyte(AT24C02_ADDRESS);		//用数据发送（写）地址，存以后写的字节数据
		I2C_Receiveack();						//接收应答//按自己写的它是1的，但它测试变0了，说明电脑给了应答
		I2C_Sendbyte(wordaddress);
		I2C_Receiveack();						//接下来看看要在地址上写入什么数据
		I2C_Sendbyte(Data);
		I2C_Receiveack();		
		I2C_Stop();									//结束后字节写好了	
		
}
///*---------------------------------------------*
//数据帧，字节读取data步骤
//s(开始)->s（发送字节）:address+w(写地址)->RA(接收应答)->
////s（发送字节）:wordaddress（字节地址）->RA(接收应答)->
//* ！*s(开始)//不结束原因保证数据帧只有1帧，多了会重复就乱了->
//s（发送字节）:address+R(读地址(从机地址))->RA(接收应答)
//->R(接收字节):data->SA（1）(发送应答，SDA=1)->stop（结束）
//-----------------------------------------------*/
unsigned char AT24C02_Readbyte(unsigned char wordaddress)	//读取数据
{
		unsigned char Data;
		I2C_Start();									//开始用数据传输
		I2C_Sendbyte(AT24C02_ADDRESS);		//用数据发送（写）地址，存以后写的字节数据
		I2C_Receiveack();						//接收应答//按自己写的它是1的，但它测试变0了，说明电脑给了应答
		I2C_Sendbyte(wordaddress);
		I2C_Receiveack();
		I2C_Start();
		I2C_Sendbyte(AT24C02_ADDRESS|0x01);			//写地址最低位置1变成读地址
		I2C_Receiveack();
		Data = I2C_Receivebyte();			//读取字节
		I2C_Sendack(1);
		I2C_Stop();
		
		return Data;
}
///*------------------------------------*
//上面是数据读取和输入的过程
//--------------------------------------*/