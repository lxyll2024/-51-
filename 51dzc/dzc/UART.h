#ifndef __UART_H__
#define __UART_H__
void URAT_Init(baud);
void UART_SendByte(unsigned char Byte);
void Send_Word(unsigned char *p);
//void UART_Init();
//void UART_SendByte(unsigned char Byte);
//void URAT_Init(baud);
//void senddata(char dat);
#endif
