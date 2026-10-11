#ifndef __i2c_H__
#define __i2c_H__

void I2C_Start(void);											//开始发送数据 初始化		I2C数据帧
void I2C_Stop(void);											//停止，数据帧结束保证上升趋势0-1，但此时未知它是否在0，自己设所以
void I2C_Sendbyte(unsigned char byte);		//发送字节
unsigned char I2C_Receivebyte(void);			//接收字节
void I2C_Sendack(unsigned char ackbit);		//应答位发送,1应答，0不应答
unsigned char I2C_Receiveack(void);				//应答接收

#endif