#include "led.h"

/*******************************************************************
 * Function  :LED初始化 
 * Parameter :void 
 * Return    :void
 * Comment   :LED1--PC4 LED2--PC5 LED3--PC6 LED4 -- PC7 低电平点亮
********************************************************************/
void LED_Init(void)
{
	//外设的电源开关
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode     =GPIO_Mode_OUT;//输出
	GPIO_InitStruct.GPIO_OType    =GPIO_OType_PP;//推挽
	GPIO_InitStruct.GPIO_Pin      =GPIO_Pin_4 |GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_PuPd     =GPIO_PuPd_NOPULL;//浮空
	GPIO_InitStruct.GPIO_Speed    =GPIO_Low_Speed;//低速
  GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
}
