//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F407ZGT6,����ԭ��Explorer STM32F4������,��Ƶ168MHZ������12MHZ
//QDtech-TFTҺ������ for STM32 IOģ��
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/08/22
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================��Դ����================================================//
//     LCDģ��                STM32��Ƭ��
//      VCC          ��          3.3V         //��Դ
//      GND          ��          GND          //��Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ4����SPI����
//     LCDģ��                STM32��Ƭ��    
//       SDA         ��          PB5          //Һ����SPI��������д�ź�
//=======================================Һ���������߽���==========================================//
//     LCDģ�� 					      STM32��Ƭ�� 
//       BLK         ��          PB13         //Һ������������źţ��������Ҫ���ƣ���3.3V
//       SCL         ��          PB3          //Һ����SPI����ʱ���ź�
//       DC          ��          PB14         //Һ��������/��������ź�
//       RES         ��          PB12         //Һ������λ�����ź�
//       CS          ��          PB15         //Һ����Ƭѡ�����ź�
//=========================================������������=========================================//
//���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
//	   LCDģ��                STM32��Ƭ�� 
//      T_IRQ        ��          PB1          //�����������ж��ź�
//      T_DO         ��          PB2          //������SPI���߶��ź�
//      T_DIN        ��          PF11         //������SPI����д�ź�
//      T_CS         ��          PC5          //������Ƭѡ�����ź�
//      T_CLK        ��          PB0          //������SPI����ʱ���ź�
**************************************************************************************************/		
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	
#include "lcd.h"
#include "stdlib.h"
#include "systick.h"	 
#include "spi.h"

	   
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

//������ɫ,������ɫ
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 

/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u8 data)
{ 
//   LCD_CS_CLR;     
	 LCD_RS_CLR;	  
   SPIv_WriteData(data);
//   LCD_CS_SET;	
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u8 data)
{
//   LCD_CS_CLR;
	 LCD_RS_SET;
   SPIv_WriteData(data);
//   LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void Lcd_WriteData_16Bit(u16 Data)
{	
//   LCD_CS_CLR;
   LCD_RS_SET;  
   SPIv_WriteData(Data>>8);
	 SPIv_WriteData(Data);
//   LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//���ù��λ�� 
	Lcd_WriteData_16Bit(POINT_COLOR); 
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(u16 Color)
{
  unsigned int i,m;  
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
//	LCD_CS_CLR;
	LCD_RS_SET;
	for(i=0;i<lcddev.height;i++)
	{
    for(m=0;m<lcddev.width;m++)
    {	
		SPIv_WriteData(Color>>8);
	 	SPIv_WriteData(Color);
		}
	}
//	 LCD_CS_SET;
} 
/**************************************************************************
������	void LCD_ClearPartial(u16 xStar, u16 yStar, u16 xEnd, u16 yEnd, u16 Color)
���ܣ�	���������Ļ��ʾ
������ 	
����ֵ��
��ע��       
***************************************************************************/
	void LCD_ClearPartial(u16 xStar, u16 yStar, u16 xEnd, u16 yEnd, u16 Color)
{
    unsigned int i, m;
    LCD_SetWindows(xStar, yStar, xEnd-1, yEnd-1); // ����Ҫ����Ĵ�������
    LCD_RS_SET;
    // ͨ������£�����LCD��Ƭѡ�ź��Ǳ�Ҫ�ģ��������Ĵ������ⲿ�ֱ�ע�͵��ˡ�
    // �����ҪƬѡ�źţ�ȡ������ע�͡�
    // LCD_CS_CLR; 

    for(i = yStar; i <= yEnd; i++)
    {
        for(m = xStar; m <= xEnd; m++)
        {
            // ������ɫ���ݵ�LCD
            // ��λ�ֽ�
            SPIv_WriteData(Color >> 8);
            // ��λ�ֽ�
            SPIv_WriteData(Color);
        }
    }

    // ���ʹ��Ƭѡ�źţ�ȡ������ע�͡�
    // LCD_CS_SET;
}
/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_GPIOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	      
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_12| GPIO_Pin_13|GPIO_Pin_14;//| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_12| GPIO_Pin_13|GPIO_Pin_14);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay_ms(20);	
	LCD_RST_SET;
	delay_ms(20);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD_Init(void)
{  
	LCD_GPIOInit();//LCD GPIO��ʼ��	
 	LCD_RESET(); //LCD ��λ
//************* ST7789��ʼ��**********//	
	LCD_WR_REG(0x36); 
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0x3A); 
	LCD_WR_DATA(0x05);

	LCD_WR_REG(0xB2);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x33);
	LCD_WR_DATA(0x33);

	LCD_WR_REG(0xB7); 
	LCD_WR_DATA(0x35);  

	LCD_WR_REG(0xBB);
	LCD_WR_DATA(0x19);

	LCD_WR_REG(0xC0);
	LCD_WR_DATA(0x2C);

	LCD_WR_REG(0xC2);
	LCD_WR_DATA(0x01);

	LCD_WR_REG(0xC3);
	LCD_WR_DATA(0x12);   

	LCD_WR_REG(0xC4);
	LCD_WR_DATA(0x20);  

	LCD_WR_REG(0xC6); 
	LCD_WR_DATA(0x0F);    

	LCD_WR_REG(0xD0); 
	LCD_WR_DATA(0xA4);
	LCD_WR_DATA(0xA1);

	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0xD0);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x2B);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x54);
	LCD_WR_DATA(0x4C);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x1F);
	LCD_WR_DATA(0x23);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA(0xD0);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x51);
	LCD_WR_DATA(0x2F);
	LCD_WR_DATA(0x1F);
	LCD_WR_DATA(0x1F);
	LCD_WR_DATA(0x20);
	LCD_WR_DATA(0x23);

	LCD_WR_REG(0x21); 

	LCD_WR_REG(0x11); 
	//Delay (120); 

	LCD_WR_REG(0x29); 	
  LCD_direction(USE_HORIZONTAL);//����LCD��ʾ����
	LCD_LED=1;//��������	 
	LCD_Clear(WHITE);//��ȫ����ɫ
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA((xStar+lcddev.xoffset)>>8);
	LCD_WR_DATA(xStar+lcddev.xoffset);		
	LCD_WR_DATA((xEnd+lcddev.xoffset)>>8);
	LCD_WR_DATA(xEnd+lcddev.xoffset);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA((yStar+lcddev.yoffset)>>8);
	LCD_WR_DATA(yStar+lcddev.yoffset);		
	LCD_WR_DATA((yEnd+lcddev.yoffset)>>8);
	LCD_WR_DATA(yEnd+lcddev.yoffset);

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM			
}   

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;
			lcddev.wramcmd=0x2C;
	switch(direction){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			lcddev.xoffset=0;
			lcddev.yoffset=0;
			LCD_WriteReg(0x36,0);//BGR==1,MY==0,MX==0,MV==0
		break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			lcddev.xoffset=0;
			lcddev.yoffset=0;
			LCD_WriteReg(0x36,(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;
      lcddev.xoffset=0;
			lcddev.yoffset=80;			
			LCD_WriteReg(0x36,(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			lcddev.xoffset=80;
			lcddev.yoffset=0;
			LCD_WriteReg(0x36,(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;	
		default:break;
	}		
}	 

/*******************************************************************
 * Function  : LCD��ʾͼƬ
 * Parameter : xs ��ͼƬ����ʼ������ ys ����ʼ��������
 * Return    : void
 * Comment   : ͼƬȡģ������ÿ�����ص����ɫ��ȡ���� 
               ����ֻ�����LCD ���ĸ�������ʾ
               ��С�ڵ��������Χ������������ʾͼƬ
********************************************************************/
void LCD_DisPlay_Picture(u8 xs,u8 ys,u8*buff)
{
	u8 wigth;//ͼƬ�Ŀ��
	u8 high;//ͼƬ�ĸ߶�
	wigth=buff[3];
	high=buff[5];
	//��������
	LCD_SetWindows(xs, ys,xs+wigth-1,ys+high-1);
	//�������� ǰ�˸������ǹ���ͼƬ��Ϣ�� 
	buff+=8;
	for(u16 i=0;i<high*wigth;i++)
	{
		/*buff[0]+ buff[1] ��һ����  buff[2] + buff[3] �ڶ����� buff[4] + buff[5] �ڶ�����*/
		Lcd_WriteData_16Bit(((buff[2*i]<<8)|buff[2*i+1]));
	}
}


/*******************************************************************
 * Function  : LCD����ʾһ������
 * Parameter : u8 xs,u8 ys,u8 size,u8 *buff,u16 color
 * Return    : void
 * Comment   : ����ȡģ�������һ���ֽ�����8λ 1λ��Ӧ1�����ص�
               �����������Ƿ���Ҫ���������ص� ��ɫ��Ҫ�Լ����
********************************************************************/
void LCD_Display_Chinese(u8 xs,u8 ys,u8 size,u8 *buff,u16 color)
{
	//���õ�ַ
   LCD_SetWindows(xs, ys,xs+size-1,ys+size-1);
	 
	//�ж����ݵ�ÿ��λ��������ɫ
	for(u16 i=0;i<size*size/8;i++)//�����Ԫ�ظ���
	{
		for(u8 j=0;j<8;j++)//ÿ��Ԫ�ز����Ĵ���
		{
			if(buff[i]&(0x80>>j))
			{
				Lcd_WriteData_16Bit(color);
			}
			else
			{
				Lcd_WriteData_16Bit(0Xffff);
			}
		}
	}
}


/*******************************************************************
 * Function  : LCD����ʾһ���ַ�
 * Parameter : u8 xs,u8 ys,u8 size,u8 *buff,u16 color
 * Return    : void
 * Comment   : 
********************************************************************/
void LCD_Display_ASCII(u8 xs,u8 ys,u8 size,u8 *buff,u16 color)
{
	if(size!=16 &&size !=24 && size !=32)  return ;
		if(size==24)
	{
	//���õ�ַ
		LCD_SetWindows(xs, ys,xs+16-1,ys+size-1);
	 
		//�����Ԫ�ظ���
		for(u16 i=0;i<48;i++){
		//ÿ��Ԫ�ز����Ĵ���
		for(u8 j=0;j<8;j++){
			if(buff[i]&(0x80>>j)){
				Lcd_WriteData_16Bit(color);}
			else{
				Lcd_WriteData_16Bit(0XFFFF);}
		}		
	 }
  }
	
	else{
	//���õ�ַ
	LCD_SetWindows(xs, ys,xs+size/2-1,ys+size-1);
	
	//�ж����ݵ�ÿ��λ��������ɫ
	for(u16 i=0;i<size*size/16;i++)//�����Ԫ�ظ���
	{
		for(u8 j=0;j<8;j++)//ÿ��Ԫ�ز����Ĵ���
		{
			if(buff[i]&(0x80>>j))
			{
				Lcd_WriteData_16Bit(color);
			}
			else
			{
				Lcd_WriteData_16Bit(0XFFFF);
			}
		}
	}
 }
}


/*******************************************************************
 * Function  : LCD��ʾ�ַ����������Խ��⣩
 * Parameter : u8 xs,u8 ys,u8 size,u8 *buff,u16 color
 * Return    : 
 * Comment   : "�Ͼ� ��ӭ�� 123"
********************************************************************/

void LCD_Display_STRING( u8 xs,u8 ys,u8 size,u8 *buff,u16 color)
{
	 while(*buff){
    //����ASCII�ж��Ƿ����ַ�
	  if(*buff>=0xA1&&*(buff+1)>=0xA2&&*buff<=0xf7&&*(buff+1)<=0xfe){
			for(u8 i=0;i<strlen(hzk);i++){
				//�ַ���������ַ�
				if(*buff==hzk[2*i]&&*(buff+1)==hzk[2*i+1]){
					LCD_Display_Chinese(xs,ys,size,(u8*)(hzm+i*size*size/8),color);
					//������⣺�ҵ��ɹ���ʾ�� ��������Ѱ�� �����˷�CPU��Դ
				  break;}
			  }
		//һ�ֲ���֮�� �õ�ַƫ��һλ����һ���ַ�
		buff+=2;
		//д��һ���� ���е�ַƫ��һ���ֵĴ�С
		xs += size;
		if(xs + size  > 239){
		ys += size;
		xs  = 0;}
		}
		else if(*buff <= 0xF7){
			for(u8 i=0;i<strlen(zfk);i++){
				//�ַ���������ַ�
				if(*buff==zfk[i]){
					LCD_Display_ASCII(xs,ys,size,(u8*)(zfm+i*size*size/16),color);
					//������⣺�ҵ��ɹ���ʾ�� ��������Ѱ�� �����˷�CPU��Դ
				  break;}
			  }
		//һ�ֲ���֮�� �õ�ַƫ��һλ����һ���ַ�
		buff++;
		//д��һ���� ���е�ַƫ��һ���ֵĴ�С
		xs += size/2;
		if(xs + size/2  > 239){
			ys += size;
			xs  = 0;}
		}
		}

	}

	
	



