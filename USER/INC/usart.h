#ifndef USART_H	//�����ظ���������������                           
#define USART_H
#include "stm32f4xx.h"

typedef struct{
u8 U1_REC[20];
u8 U1_RECLen ;
u8 U1_FLAG ;//�����ж������Ƿ��Ѿ�������� 
}U1_RECV;


void  USART1_Init(u32 baudrate);
void  USART1_SendByte(u8 Data);
u8 USART1_ReceiveByte(void);
void  USART1_RECPackage(U1_RECV* P);
#endif 
