#include "usart.h"
#include <stdio.h>
  

	 
#pragma import(__use_no_semihosting_swi) //ȡ��������״̬

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) 
{
 //�滻Ϊ�������ض���ķ��͵��ֽں���
 USART1_SendByte(ch);
  return (ch);
}

int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
  //�滻Ϊ�������ض���ķ��͵��ֽں���
	USART1_SendByte(ch);
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}


/*
������	USART1_Init 
���ܣ�	USART1��ʼ��
������ 	u32 baudrate �����
����ֵ����
��ע��  U1_TX(APB2)  --  PA9
        U1_RX  --  PA10	
*/

void  USART1_Init(u32 baudrate)
{
    // 1	��ʱ�ӣ�USART1 + GPIO��
    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE);
	  RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1, ENABLE);
	
	  // 2  ��GPIO��������  ģʽ��Ϊ����ģʽ
    GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.GPIO_Mode  =   GPIO_Mode_AF;
	  GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP  ;
	  GPIO_InitStruct.GPIO_Pin   =   GPIO_Pin_9  | GPIO_Pin_10;
	  GPIO_InitStruct.GPIO_PuPd  =   GPIO_PuPd_NOPULL ;
	  GPIO_InitStruct.GPIO_Speed =   GPIO_High_Speed;
    GPIO_Init (GPIOA, &GPIO_InitStruct);	
	
    // 3  ��ָ����GPIO���Ÿ���ΪUSART1
	  GPIO_PinAFConfig  (GPIOA,GPIO_PinSource9,GPIO_AF_USART1 ) ;
	  GPIO_PinAFConfig  (GPIOA,GPIO_PinSource10,GPIO_AF_USART1 ) ;
	
	  // 4  USART����
	  USART_InitTypeDef USART_InitStruct;
	  USART_InitStruct.USART_BaudRate = baudrate;
	  USART_InitStruct.USART_HardwareFlowControl =  USART_HardwareFlowControl_None  ;
		USART_InitStruct.USART_Mode = USART_Mode_Rx  | USART_Mode_Tx  ;
		USART_InitStruct.USART_Parity = USART_Parity_No ;
		USART_InitStruct.USART_StopBits = USART_StopBits_1 ;
		USART_InitStruct.USART_WordLength = USART_WordLength_8b  ;
		
	  // 5  USART��ʼ��
		USART_Init (USART1,&USART_InitStruct);
		
		// 6  ʹ��USART1
		USART_Cmd (USART1, ENABLE);
 
}


void  USART1_SendByte(u8 Data)
	
//��װһ�����ͺ��� while��������
{
	//�ȴ�ĳ��λTXE��λ��
	 while(USART_GetFlagStatus  (USART1,USART_FLAG_TXE) == RESET){}
   
	//�ٵ��ùٷ���װ�ķ��ͺ���
   USART_SendData (USART1, Data);
}


u8 USART1_ReceiveByte(void)
{
	u8 data;
	//�ȴ�RXNE��λ�� �������ݼĴ�����������
	while(USART_GetFlagStatus  (USART1,USART_FLAG_RXNE) == RESET){}
	
  data = USART_ReceiveData (USART1);
	
	return  data;
		
}


/*
������	USART1_RECPackage 
���ܣ�	USART1�������ݰ�����
������ 	��
����ֵ����
��ע�� 
*/

void  USART1_RECPackage(U1_RECV* P)
{
    //�ж��Ƿ���Ҫ�������� (�������ݼĴ������Ƿ�������)
	  if(USART_GetFlagStatus  (USART1,USART_FLAG_RXNE) == SET)
		{
			P->U1_REC[P->U1_RECLen] = USART_ReceiveData (USART1);
		 // U1_REC[U1_RECLen] = USART_ReceiveData (USART1);
			P->U1_RECLen++;
		}
		if(USART_GetFlagStatus  (USART1,USART_FLAG_IDLE) == SET)
		{
		  P->U1_FLAG = 1;//��Ǳ��������Ѿ�������� ���Կ�ʼ��������
			USART1->SR ;//����˳���ȡDR �� SR�������IDLE��״̬
			USART1->DR ;
		}
	  
}

