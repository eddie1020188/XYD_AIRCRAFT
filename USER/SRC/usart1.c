#include "usart1.h"
#include "main.h"
int fputc(int ch,FILE*f)
{
	USART1_SendByte(ch);
	return ch;
}
/*******************************************************************
 * Function  : USART1初始化
 * Parameter : u32 bound
 * Return    : void
 * Comment   : PA9--USART1_TX  PA10--USART1_RX
********************************************************************/
void USART1_Init(u32 bound)
{
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	
	//USART-GPIO配置
	GPIO_InitTypeDef GPIO_InitStructure;                     //新建GPIO结构体
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);                      //初始化PA9,PA10
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);  //PA-09映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //PA-10映射
	
	USART_InitTypeDef USART_InitStructure;                   //新建串口结构体
	USART_InitStructure.USART_BaudRate             =bound;
	USART_InitStructure.USART_HardwareFlowControl  =USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                 =USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity               =USART_Parity_No;
	USART_InitStructure.USART_StopBits             =USART_StopBits_1;
	USART_InitStructure.USART_WordLength           =USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	 USART1_Config();
	USART_Cmd(USART1,ENABLE);
}

/*******************************************************************
 * Function  : USART1发送单个字节
 * Parameter : u8 data
 * Return    : void
 * Comment   : TXE置1，可以发送数据
********************************************************************/
void USART1_SendByte(u8 data)
{
	USART_SendData(USART1,data);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}

/*******************************************************************
 * Function  : USART1接收单个字节
 * Parameter : void
 * Return    : u8
 * Comment   : RXXE置1，可以接收数据
********************************************************************/
u8 USART1_RecByte(void)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET);
	return USART_ReceiveData(USART1);
	
}
/*******************************************************************
 * Function  : USART1中断配置
 * Parameter : void
 * Return    : void
 * Comment   : RXNE IDLE
********************************************************************/
void USART1_Config(void)
{
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
//	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	NVIC_InitTypeDef  NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel                    =USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd                 =ENABLE; 
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority  =2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority         =2;
  NVIC_Init(&NVIC_InitStruct);
}

#if 0

USART1_Rec Rec={0};
//_Bool EXTI_FLASH_FLAG = 0;
void USART1_IRQHandler(void)
{
	char data;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		
		data=USART1_RecByte();
		Rec.buff[Rec.len]=data;
		Rec.len++;
	}
	if(USART_GetITStatus(USART1,USART_IT_IDLE))
	{
		printf("数据内容:%s\r\n",Rec.buff);
		printf("数据长度:%d\r\n",Rec.len);
		Rec.len=0;
		memset(Rec.buff,0,sizeof(Rec.buff));
		USART1->SR;
		USART1->DR;
	}
}

#endif

void USART1_IRQHandler(void)
{
	u8 data;
	static u32 addr = 0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		
		data=USART_ReceiveData(USART1);
	  W25Q64_Program(addr,&data,1);
    addr ++;
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//RXNE清零
	}

}
