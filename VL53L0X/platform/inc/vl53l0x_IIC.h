#ifndef __VL53L0X_IIC_H
#define __VL53L0X_IIC_H

#include "stdint.h"
#include "systick.h"
#include "stm32f4xx.h"
//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����
#define VL_SDA_RCC  RCC_AHB1Periph_GPIOB
#define VL_SDA_PIN  GPIO_Pin_11
#define VL_SDA_IOx  GPIOB  
#define VL_SCL_RCC  RCC_AHB1Periph_GPIOB
#define VL_SCL_PIN  GPIO_Pin_10
#define VL_SCL_IOx  GPIOB  

//��SDA��Ӧ��GPIO�� PB11����Ϊ��������ģʽ
#define VL_SDA_IN()  {GPIOB->MODER&=0xFF3FFFFF;GPIOB->PUPDR&=0xFF3FFFFF;}
//��SDA��Ӧ��GPIO�� PB11����Ϊͨ���������ģʽ
#define VL_SDA_OUT() {GPIOB->MODER&=0xFF3FFFFF;GPIOB->MODER|=(1<<20);\
GPIOB->PUPDR&=0xFF3FFFFF;}
#define VL_IIC_SCL    PBout(10)      //SCL
#define VL_IIC_SDA    PBout(11)      //SDA	 
#define VL_READ_SDA   PBin(11) 		 //����SDA 

//Status
#define STATUS_OK       0x00
#define STATUS_FAIL     0x01

void VL53L0X_IIC_Init(void);
uint8_t VL_I2C_Read_nByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t *buf, uint16_t len);
uint8_t VL_I2C_Write_nByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t *buf, uint16_t len);

#endif 

