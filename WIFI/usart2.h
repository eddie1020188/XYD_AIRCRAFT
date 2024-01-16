#ifndef USART2_H
#define USART2_H
#include "stm32f4xx.h"
typedef struct
{
	char RevBuf[500];//������յ�����
	u8 RevOver;//������ɱ�־λ
	u8 RevLen;//������յĳ���
}ESP12rec;

extern ESP12rec ESP12rev;
void USART2_Init(u32 bound);
void USART2_SendByte(u8 data);
void USART2_SendStr(char *buff);
#endif
