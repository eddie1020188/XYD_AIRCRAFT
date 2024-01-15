#include "systick.h"

/*******************************************************************
 * Function  : SysTick毫秒级延时
 * Parameter : u16 ms
 * Return    : void
 * Comment   : 外部参考时钟 21MHZ,计数器是24位
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
 * Function  : SysTick微秒级延时
 * Parameter : u32 us
 * Return    : void
 * Comment   : 外部参考时钟 21MHZ,计数器是24位
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
