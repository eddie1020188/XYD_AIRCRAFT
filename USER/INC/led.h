#ifndef LED_H
#define LED_H

#include "stm32f4xx.h"

#define LED1_ON  GPIO_ResetBits(GPIOC,GPIO_Pin_4)
#define LED1_OFF GPIO_SetBits(GPIOC,GPIO_Pin_4);


#define LED2_ON  GPIO_ResetBits(GPIOC,GPIO_Pin_5);
#define LED2_OFF GPIO_SetBits(GPIOC,GPIO_Pin_5);


#define LED3_ON  GPIO_ResetBits(GPIOC,GPIO_Pin_6);
#define LED3_OFF GPIO_SetBits(GPIOC,GPIO_Pin_6);

#define LED4_ON  GPIO_ResetBits(GPIOC,GPIO_Pin_7);
#define LED4_OFF GPIO_SetBits(GPIOC,GPIO_Pin_7);

void LED_Init(void);
#endif
