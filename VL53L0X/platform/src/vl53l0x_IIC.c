#include "vl53l0x_IIC.h"

void VL53L0X_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(	VL_SDA_RCC, ENABLE );	
	GPIO_InitStructure.GPIO_Pin = VL_SDA_PIN;               //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //50Mhz速度
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(VL_SDA_IOx, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(	VL_SCL_RCC, ENABLE );	
	GPIO_InitStructure.GPIO_Pin = VL_SCL_PIN;               //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //50Mhz速度
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(VL_SCL_IOx, &GPIO_InitStructure);
	
	GPIO_SetBits(VL_SDA_IOx,VL_SDA_PIN);//SDA输出高	
	GPIO_SetBits(VL_SCL_IOx,VL_SCL_PIN);//SCL输出高	
}

void VL_IIC_Start(void)
{
	VL_SDA_OUT();//sda线输出
	VL_IIC_SDA=1;	  	  
	VL_IIC_SCL=1;
	delay_us(4);
 	VL_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	VL_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}

//产生IIC停止信号
void VL_IIC_Stop(void)
{
	VL_SDA_OUT();//sda线输出
	VL_IIC_SCL=0;
	VL_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	VL_IIC_SCL=1; 
	VL_IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}

u8 VL_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	VL_SDA_IN();  //SDA设置为输入  
	VL_IIC_SDA=1;delay_us(1);	   
	VL_IIC_SCL=1;delay_us(1);	 
	while(VL_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			VL_IIC_Stop();
			return 1;
		}
	}
	VL_IIC_SCL=0;//时钟输出0 	   
	return 0;  
}

//产生ACK应答
void VL_IIC_Ack(void)
{
	VL_IIC_SCL=0;
	VL_SDA_OUT();
	VL_IIC_SDA=0;
	delay_us(2);
	VL_IIC_SCL=1;
	delay_us(2);
	VL_IIC_SCL=0;
}

//不产生ACK应答		    
void VL_IIC_NAck(void)
{
	VL_IIC_SCL=0;
	VL_SDA_OUT();
	VL_IIC_SDA=1;
	delay_us(2);
	VL_IIC_SCL=1;
	delay_us(2);
	VL_IIC_SCL=0;
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void VL_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	VL_SDA_OUT(); 	    
    VL_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			VL_IIC_SDA=1;
		else
			VL_IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);  
		VL_IIC_SCL=1;
		delay_us(2); 
		VL_IIC_SCL=0;	
		delay_us(2);
    }	 
} 

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 VL_IIC_Read_Byte(void)
{
	unsigned char i,receive=0;
	VL_SDA_IN();//SDA设置为输入
	VL_IIC_SDA = 1;
	delay_us(4);
	for(i=0;i<8;i++ )
	{
		receive<<=1;
		VL_IIC_SCL=0; 
		delay_us(4);
	    VL_IIC_SCL=1;
		delay_us(4);
		if(VL_READ_SDA)
			receive |= 0x01;   
	    delay_us(4); //1
	}	
    VL_IIC_SCL = 0;	
	return receive;
}

//IIC写一个字节数据
u8 VL_IIC_Write_1Byte(u8 SlaveAddress, u8 REG_Address,u8 REG_data)
{
	VL_IIC_Start();
	VL_IIC_Send_Byte(SlaveAddress);
	if(VL_IIC_Wait_Ack())
	{
		VL_IIC_Stop();//释放总线
		return 1;//没应答则退出

	}
	VL_IIC_Send_Byte(REG_Address);
	VL_IIC_Wait_Ack();	
	delay_us(5);
	VL_IIC_Send_Byte(REG_data);
	VL_IIC_Wait_Ack();	
	VL_IIC_Stop();

	return 0;
}

//IIC读一个字节数据
u8 VL_IIC_Read_1Byte(u8 SlaveAddress, u8 REG_Address,u8 *REG_data)
{
	VL_IIC_Start();
	VL_IIC_Send_Byte(SlaveAddress);//发写命令
	if(VL_IIC_Wait_Ack())
	{
		 VL_IIC_Stop();//释放总线
		 return 1;//没应答则退出
	}		
	VL_IIC_Send_Byte(REG_Address);
	VL_IIC_Wait_Ack();
	delay_us(5);
	VL_IIC_Start(); 
	VL_IIC_Send_Byte(SlaveAddress|0x01);//发读命令
	VL_IIC_Wait_Ack();
	*REG_data = VL_IIC_Read_Byte();
	VL_IIC_Stop();

	return 0;
}

//I2C读多个字节数据
uint8_t VL_I2C_Read_nByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t *buf, uint16_t len)
{
	VL_IIC_Start();
	VL_IIC_Send_Byte(SlaveAddress);//发写命令
	if(VL_IIC_Wait_Ack()) 
	{
		VL_IIC_Stop();//释放总线
		return 1;//没应答则退出
	}
	VL_IIC_Send_Byte(REG_Address);
	VL_IIC_Wait_Ack();
	delay_us(5);
	VL_IIC_Start(); 
	VL_IIC_Send_Byte(SlaveAddress|0x01);//发读命令
	VL_IIC_Wait_Ack();
	while(len)
	{
		*buf = VL_IIC_Read_Byte();
		if(1 == len)
		{
			VL_IIC_NAck();
		}
		else
		{
			VL_IIC_Ack();
		}
		buf++;
		len--;
	}
	VL_IIC_Stop();

	return STATUS_OK;
}

//I2C写多个字节数据
uint8_t VL_I2C_Write_nByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t *buf, uint16_t len)
{
	VL_IIC_Start();
	VL_IIC_Send_Byte(SlaveAddress);//发写命令
	if(VL_IIC_Wait_Ack()) 
	{
		VL_IIC_Stop();//释放总线
		return 1;//没应答则退出
	}
	VL_IIC_Send_Byte(REG_Address);
	VL_IIC_Wait_Ack();
	while(len--)
	{
		VL_IIC_Send_Byte(*buf++);
		VL_IIC_Wait_Ack();
	}
	VL_IIC_Stop();

	return STATUS_OK;
}

