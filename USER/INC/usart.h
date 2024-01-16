#ifndef USART_H	//避免重复包含，条件编译                           
#define USART_H
#include "stm32f4xx.h"

typedef struct{
u8 U1_REC[20];
u8 U1_RECLen ;
u8 U1_FLAG ;//用来判断数据是否已经接收完毕 
}U1_RECV;


void  USART1_Init(u32 baudrate);
void  USART1_SendByte(u8 Data);
u8 USART1_ReceiveByte(void);
void  USART1_RECPackage(U1_RECV* P);
#endif 
