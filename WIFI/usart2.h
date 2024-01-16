#ifndef USART2_H
#define USART2_H
#include "stm32f4xx.h"
typedef struct
{
	char RevBuf[500];//保存接收的数据
	u8 RevOver;//接收完成标志位
	u8 RevLen;//保存接收的长度
}ESP12rec;

extern ESP12rec ESP12rev;
void USART2_Init(u32 bound);
void USART2_SendByte(u8 data);
void USART2_SendStr(char *buff);
#endif
