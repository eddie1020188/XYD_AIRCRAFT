#ifndef	_KEY_H_
#define	_KEY_H_

#include "stm32f4xx.h"
#include "systick.h"
//仅当KEY1按下时为真(按下高电平)
#define KEY1_Press		(GPIOA->IDR & (1<<0))
//仅当KEY2按下时为真(按下低电平)
#define KEY2_Press		!(GPIOE->IDR & (1<<2))
//仅当KEY3按下时为真(按下低电平)
#define KEY3_Press		!(GPIOE->IDR & (1<<3))
//仅当KEY4按下时为真(按下低电平)
#define KEY4_Press		!(GPIOE->IDR & (1<<4))

//构建了无名的枚举类型（只用其中数值，标识符）
//提高程序的可读性
enum {
	KEY_NONE = 0,
	KEY1,
	KEY2,
	KEY3,
	KEY4,
};

void key_init(void);
int key_scan(int mode);

#endif