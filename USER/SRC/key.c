#include "key.h"

/*
��������key_init
��  �ܣ�key�ĳ�ʼ��
				KEY1	--	PA0(���¸ߵ�ƽ���ɿ��͵�ƽ)
				KEY2	--	PE2(���µ͵�ƽ���ɿ��ߵ�ƽ)
				KEY3	--	PE3(���µ͵�ƽ���ɿ��ߵ�ƽ)
				KEY4	--	PE4(���µ͵�ƽ���ɿ��ߵ�ƽ)
��  ������
����ֵ����
*/
void key_init(void)
{
	//����GPIOA
	RCC->AHB1ENR |= (1<<0);
	//����PA0Ϊ����
	GPIOA->MODER &=~(3<<(2*0));
	//����PA0��ʹ�����õ���
	GPIOA->PUPDR &=~(3<<(2*0));
	
	//����GPIOE
	RCC->AHB1ENR |= (1<<4);
	//����PE2Ϊ����
	GPIOE->MODER &=~(3<<(2*2));
	//����PE2��ʹ�����õ���
	GPIOE->PUPDR &=~(3<<(2*2));
	
	//����PE3Ϊ����
	GPIOE->MODER &=~(3<<(2*3));
	//����PE3��ʹ�����õ���
	GPIOE->PUPDR &=~(3<<(2*3));
	
	//����PE4Ϊ����
	GPIOE->MODER &=~(3<<(2*4));
	//����PE4��ʹ�����õ���
	GPIOE->PUPDR &=~(3<<(2*4));
}


/*
��������key_scan
��  �ܣ�key��ɨ��
��  ����int mode���涨0��ʾ�����ְ���ʱ����1��ʾ���ְ���ʱ��
����ֵ����ʾʶ��İ������0~4
*/
int key_scan(int mode)
{
	if(KEY1_Press)
	{
		
		delay_ms(15);
		
		while(KEY1_Press){
		return KEY1;}
	}

	//���ް������£�����KEY_NONE
	return KEY_NONE;
}

