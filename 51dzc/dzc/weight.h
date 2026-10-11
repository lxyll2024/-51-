#ifndef __weight_H__
#define __weight_H__

extern unsigned int Weight_Maopi,Range,qupi,num;
extern long Weight_Shiwu;
extern unsigned char num_Store,Weight_Store_Num;
extern unsigned char mode,m4,ERROR1,Bad_Num1,Keynum,TimeSetSelect,TimeSetFlashFlag;
extern unsigned int Weight_Standard1,FanWei_Max,FanWei_Min;
extern unsigned char Weight_Shiwu_Flag1,num_Flag,Weight_Shiwu_Flag2;
extern int Weight_Standard;
extern char temp,m3,ERROR,Bad_Num,sum,Month,Day,Hour,Minute,m3_Flag;
//extern unsigned int Bad[],Weight_Store[];
extern unsigned char i,Weight_Flag,KeynumFlag1,k8,m3_qingling;
extern float T;

void Display_M3_Measure();
void Display_qingping();
void Display_M1();
void Display_M2();
void Display_M3();
void Display_M3_Store();
void Display_M3_NotStore();
void Display_M4_Time();
void Display_M4_Standard();
void Display_M4_Num();
void Display_M4_Send();
void UART_Send_num();
void Display_M4_UART();
void Display_M4_successful();
void AT24C02_Readbyte_Data();
void AT24C02_Writebyte_Data();
void Timeshow(void);
void TimeSet(void);
void AT24C02_Writebyte_qingling();
void AT24C02_Writebyte_qingling_data();
	
void Get_Maopi();  //毛皮重量
void Get_Weight();  //获取重量

#endif