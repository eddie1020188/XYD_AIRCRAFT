#include "key.h"

/*
函数名：key_init
功  能：key的初始化
				KEY1	--	PA0(按下高电平，松开低电平)
				KEY2	--	PE2(按下低电平，松开高电平)
				KEY3	--	PE3(按下低电平，松开高电平)
				KEY4	--	PE4(按下低电平，松开高电平)
参  数：无
返回值：无
*/
void key_init(void)
{
	//激活GPIOA
	RCC->AHB1ENR |= (1<<0);
	//配置PA0为输入
	GPIOA->MODER &=~(3<<(2*0));
	//配置PA0不使用内置电阻
	GPIOA->PUPDR &=~(3<<(2*0));
	
	//激活GPIOE
	RCC->AHB1ENR |= (1<<4);
	//配置PE2为输入
	GPIOE->MODER &=~(3<<(2*2));
	//配置PE2不使用内置电阻
	GPIOE->PUPDR &=~(3<<(2*2));
	
	//配置PE3为输入
	GPIOE->MODER &=~(3<<(2*3));
	//配置PE3不使用内置电阻
	GPIOE->PUPDR &=~(3<<(2*3));
	
	//配置PE4为输入
	GPIOE->MODER &=~(3<<(2*4));
	//配置PE4不使用内置电阻
	GPIOE->PUPDR &=~(3<<(2*4));
}


/*
函数名：key_scan
功  能：key的扫描
参  数：int mode，规定0表示不区分按键时长，1表示区分按键时长
返回值：表示识别的按键序号0~4
*/
int key_scan(int mode)
{
	if(KEY1_Press)
	{
		
		delay_ms(15);
		
		while(KEY1_Press){
		return KEY1;}
	}

	//若无按键按下，返回KEY_NONE
	return KEY_NONE;
}

