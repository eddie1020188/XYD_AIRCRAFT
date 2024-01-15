#ifndef	_KEY_H_
#define	_KEY_H_

#include "stm32f4xx.h"
#include "systick.h"
//����KEY1����ʱΪ��(���¸ߵ�ƽ)
#define KEY1_Press		(GPIOA->IDR & (1<<0))
//����KEY2����ʱΪ��(���µ͵�ƽ)
#define KEY2_Press		!(GPIOE->IDR & (1<<2))
//����KEY3����ʱΪ��(���µ͵�ƽ)
#define KEY3_Press		!(GPIOE->IDR & (1<<3))
//����KEY4����ʱΪ��(���µ͵�ƽ)
#define KEY4_Press		!(GPIOE->IDR & (1<<4))

//������������ö�����ͣ�ֻ��������ֵ����ʶ����
//��߳���Ŀɶ���
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