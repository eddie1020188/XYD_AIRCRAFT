#include "usart2.h"
#include "main.h"
/*******************************************************************
 * Function  : usart2初始化
 * Parameter : u32 bound
 * Return    : void
 * Comment   : U2_TX--PA2  U2_RX--PA3
********************************************************************/
void USART2_Init(u32 bound)
{
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	
	//USART-GPIO配置
	GPIO_InitTypeDef GPIO_InitStructure;                     //新建GPIO结构体
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Low_Speed;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);                      //初始化PA9,PA10
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);  //PA-09映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //PA-10映射
	
	USART_InitTypeDef USART_InitStructure;                   //新建串口结构体
	USART_InitStructure.USART_BaudRate             =bound;
	USART_InitStructure.USART_HardwareFlowControl  =USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                 =USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity               =USART_Parity_No;
	USART_InitStructure.USART_StopBits             =USART_StopBits_1;
	USART_InitStructure.USART_WordLength           =USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStructure);
	 
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	NVIC_InitTypeDef  NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel                    =USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd                 =ENABLE; 
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority  =2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority         =2;
  NVIC_Init(&NVIC_InitStruct);
	USART_Cmd(USART2,ENABLE);
}

/*******************************************************************
 * Function  : USART2发送单个字节
 * Parameter : u8 data
 * Return    : void
 * Comment   : TXE置1，可以发送数据
********************************************************************/
void USART2_SendByte(u8 data)
{
	USART_SendData(USART2,data);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
}

/*******************************************************************
 * Function  : usart2发送字符串
 * Parameter : char* buff
 * Return    : void
 * Comment   : 
********************************************************************/
void USART2_SendStr(char *buff)
{
	char data;
	do
	{
		USART2_SendByte(*buff);
		data=*buff;
		buff++;
	}while(data!='\0');
}
ESP12rec ESP12rev={0};
void USART2_IRQHandler(void)
{
	u8 data;
	if(USART_GetITStatus(USART2,USART_IT_RXNE))
	{
		data=USART_ReceiveData(USART2);
		if(data==0)
		{
			ESP12rev.RevBuf[ESP12rev.RevLen++]=' ';
		}
		else
		{
		  ESP12rev.RevBuf[ESP12rev.RevLen++]=data;
		}
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
	else if(USART_GetITStatus(USART2,USART_IT_IDLE))
	{
		ESP12rev.RevBuf[ESP12rev.RevLen]='\0';
		printf("%s\r\n",ESP12rev.RevBuf);
		ESP12rev.RevLen=0;
		ESP12rev.RevOver=1;
		USART2->SR;
		USART2->DR;
	}
}
