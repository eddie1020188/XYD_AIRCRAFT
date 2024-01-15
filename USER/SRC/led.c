#include "led.h"

/*******************************************************************
 * Function  :LED��ʼ�� 
 * Parameter :void 
 * Return    :void
 * Comment   :LED1--PC4 LED2--PC5 LED3--PC6 LED4 -- PC7 �͵�ƽ����
********************************************************************/
void LED_Init(void)
{
	//����ĵ�Դ����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode     =GPIO_Mode_OUT;//���
	GPIO_InitStruct.GPIO_OType    =GPIO_OType_PP;//����
	GPIO_InitStruct.GPIO_Pin      =GPIO_Pin_4 |GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_PuPd     =GPIO_PuPd_NOPULL;//����
	GPIO_InitStruct.GPIO_Speed    =GPIO_Low_Speed;//����
  GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
}
