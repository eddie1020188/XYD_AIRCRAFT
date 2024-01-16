#include "usart.h"
#include <stdio.h>
  

	 
#pragma import(__use_no_semihosting_swi) //取消半主机状态

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) 
{
 //替换为开发者重定向的发送单字节函数
 USART1_SendByte(ch);
  return (ch);
}

int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
  //替换为开发者重定向的发送单字节函数
	USART1_SendByte(ch);
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}


/*
函数：	USART1_Init 
功能：	USART1初始化
参数： 	u32 baudrate 填波特率
返回值：无
备注：  U1_TX(APB2)  --  PA9
        U1_RX  --  PA10	
*/

void  USART1_Init(u32 baudrate)
{
    // 1	开时钟（USART1 + GPIO）
    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE);
	  RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1, ENABLE);
	
	  // 2  对GPIO进行配置  模式改为复用模式
    GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.GPIO_Mode  =   GPIO_Mode_AF;
	  GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP  ;
	  GPIO_InitStruct.GPIO_Pin   =   GPIO_Pin_9  | GPIO_Pin_10;
	  GPIO_InitStruct.GPIO_PuPd  =   GPIO_PuPd_NOPULL ;
	  GPIO_InitStruct.GPIO_Speed =   GPIO_High_Speed;
    GPIO_Init (GPIOA, &GPIO_InitStruct);	
	
    // 3  将指定的GPIO引脚复用为USART1
	  GPIO_PinAFConfig  (GPIOA,GPIO_PinSource9,GPIO_AF_USART1 ) ;
	  GPIO_PinAFConfig  (GPIOA,GPIO_PinSource10,GPIO_AF_USART1 ) ;
	
	  // 4  USART配置
	  USART_InitTypeDef USART_InitStruct;
	  USART_InitStruct.USART_BaudRate = baudrate;
	  USART_InitStruct.USART_HardwareFlowControl =  USART_HardwareFlowControl_None  ;
		USART_InitStruct.USART_Mode = USART_Mode_Rx  | USART_Mode_Tx  ;
		USART_InitStruct.USART_Parity = USART_Parity_No ;
		USART_InitStruct.USART_StopBits = USART_StopBits_1 ;
		USART_InitStruct.USART_WordLength = USART_WordLength_8b  ;
		
	  // 5  USART初始化
		USART_Init (USART1,&USART_InitStruct);
		
		// 6  使能USART1
		USART_Cmd (USART1, ENABLE);
 
}


void  USART1_SendByte(u8 Data)
	
//封装一个发送函数 while阻塞特性
{
	//等待某个位TXE置位后
	 while(USART_GetFlagStatus  (USART1,USART_FLAG_TXE) == RESET){}
   
	//再调用官方封装的发送函数
   USART_SendData (USART1, Data);
}


u8 USART1_ReceiveByte(void)
{
	u8 data;
	//等待RXNE置位后 接收数据寄存器中有数据
	while(USART_GetFlagStatus  (USART1,USART_FLAG_RXNE) == RESET){}
	
  data = USART_ReceiveData (USART1);
	
	return  data;
		
}


/*
函数：	USART1_RECPackage 
功能：	USART1接收数据包函数
参数： 	无
返回值：无
备注： 
*/

void  USART1_RECPackage(U1_RECV* P)
{
    //判断是否需要接收数据 (接收数据寄存器中是否有数据)
	  if(USART_GetFlagStatus  (USART1,USART_FLAG_RXNE) == SET)
		{
			P->U1_REC[P->U1_RECLen] = USART_ReceiveData (USART1);
		 // U1_REC[U1_RECLen] = USART_ReceiveData (USART1);
			P->U1_RECLen++;
		}
		if(USART_GetFlagStatus  (USART1,USART_FLAG_IDLE) == SET)
		{
		  P->U1_FLAG = 1;//标记本次数据已经接收完毕 可以开始处理数据
			USART1->SR ;//按照顺序读取DR 和 SR可以清除IDLE的状态
			USART1->DR ;
		}
	  
}

