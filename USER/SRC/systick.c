#include "systick.h"

/*******************************************************************
 * Function  : SysTick���뼶��ʱ
 * Parameter : u16 ms
 * Return    : void
 * Comment   : �ⲿ�ο�ʱ�� 21MHZ,��������24λ
********************************************************************/
void delay_ms(u16 ms)
{
	SysTick->CTRL &=~(1<<2);
	SysTick->LOAD =ms*21000-1;
	SysTick->VAL=0;
	SysTick->CTRL |=1<<0;
	while((SysTick->CTRL &(1<<16))==0);
	SysTick->CTRL &=~(1<<0);
}

/*******************************************************************
 * Function  : SysTick΢�뼶��ʱ
 * Parameter : u32 us
 * Return    : void
 * Comment   : �ⲿ�ο�ʱ�� 21MHZ,��������24λ
********************************************************************/
void delay_us(u32 us)
{
	SysTick->CTRL &=~(1<<2);
	SysTick->LOAD =us*21-1;
	SysTick->VAL=0;
	SysTick->CTRL |=1<<0;
	while((SysTick->CTRL &(1<<16))==0);
	SysTick->CTRL &=~(1<<0);
}
