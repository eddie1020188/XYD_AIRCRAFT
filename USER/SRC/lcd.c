//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F407ZGT6,正点原子Explorer STM32F4开发板,主频168MHZ，晶振12MHZ
//QDtech-TFT液晶驱动 for STM32 IO模拟
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/08/22
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接          3.3V         //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为4线制SPI总线
//     LCD模块                STM32单片机    
//       SDA         接          PB5          //液晶屏SPI总线数据写信号
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 					      STM32单片机 
//       BLK         接          PB13         //液晶屏背光控制信号，如果不需要控制，接3.3V
//       SCL         接          PB3          //液晶屏SPI总线时钟信号
//       DC          接          PB14         //液晶屏数据/命令控制信号
//       RES         接          PB12         //液晶屏复位控制信号
//       CS          接          PB15         //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机 
//      T_IRQ        接          PB1          //触摸屏触摸中断信号
//      T_DO         接          PB2          //触摸屏SPI总线读信号
//      T_DIN        接          PF11         //触摸屏SPI总线写信号
//      T_CS         接          PC5          //触摸屏片选控制信号
//      T_CLK        接          PB0          //触摸屏SPI总线时钟信号
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

	   
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//画笔颜色,背景颜色
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
	LCD_SetCursor(x,y);//设置光标位置 
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
函数：	void LCD_ClearPartial(u16 xStar, u16 yStar, u16 xEnd, u16 yEnd, u16 Color)
功能：	清楚部分屏幕显示
参数： 	
返回值：
备注：       
***************************************************************************/
	void LCD_ClearPartial(u16 xStar, u16 yStar, u16 xEnd, u16 yEnd, u16 Color)
{
    unsigned int i, m;
    LCD_SetWindows(xStar, yStar, xEnd-1, yEnd-1); // 设置要清除的窗口区域
    LCD_RS_SET;
    // 通常情况下，控制LCD的片选信号是必要的，但是您的代码中这部分被注释掉了。
    // 如果需要片选信号，取消以下注释。
    // LCD_CS_CLR; 

    for(i = yStar; i <= yEnd; i++)
    {
        for(m = xStar; m <= xEnd; m++)
        {
            // 发送颜色数据到LCD
            // 高位字节
            SPIv_WriteData(Color >> 8);
            // 低位字节
            SPIv_WriteData(Color);
        }
    }

    // 如果使用片选信号，取消以下注释。
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
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
	LCD_GPIOInit();//LCD GPIO初始化	
 	LCD_RESET(); //LCD 复位
//************* ST7789初始化**********//	
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
  LCD_direction(USE_HORIZONTAL);//设置LCD显示方向
	LCD_LED=1;//点亮背光	 
	LCD_Clear(WHITE);//清全屏白色
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

	LCD_WriteRAM_Prepare();	//开始写入GRAM			
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
 * Function  : LCD显示图片
 * Parameter : xs 填图片的起始行坐标 ys 填起始点纵坐标
 * Return    : void
 * Comment   : 图片取模软件会把每个像素点的颜色提取出来 
               我们只需告诉LCD 在哪个区域显示
               给小于等于这个范围的数据让其显示图片
********************************************************************/
void LCD_DisPlay_Picture(u8 xs,u8 ys,u8*buff)
{
	u8 wigth;//图片的宽度
	u8 high;//图片的高度
	wigth=buff[3];
	high=buff[5];
	//设置区域
	LCD_SetWindows(xs, ys,xs+wigth-1,ys+high-1);
	//发送数据 前八个数组是关于图片信息的 
	buff+=8;
	for(u16 i=0;i<high*wigth;i++)
	{
		/*buff[0]+ buff[1] 第一个点  buff[2] + buff[3] 第二个点 buff[4] + buff[5] 第二个点*/
		Lcd_WriteData_16Bit(((buff[2*i]<<8)|buff[2*i+1]));
	}
}


/*******************************************************************
 * Function  : LCD屏显示一个汉字
 * Parameter : u8 xs,u8 ys,u8 size,u8 *buff,u16 color
 * Return    : void
 * Comment   : 汉字取模软件给的一个字节数据8位 1位对应1个像素点
               仅仅告诉你是否需要点亮该像素点 颜色需要自己填充
********************************************************************/
void LCD_Display_Chinese(u8 xs,u8 ys,u8 size,u8 *buff,u16 color)
{
	//设置地址
   LCD_SetWindows(xs, ys,xs+size-1,ys+size-1);
	 
	//判断数据的每个位，发送颜色
	for(u16 i=0;i<size*size/8;i++)//数组的元素个数
	{
		for(u8 j=0;j<8;j++)//每个元素操作的次数
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
 * Function  : LCD屏显示一个字符
 * Parameter : u8 xs,u8 ys,u8 size,u8 *buff,u16 color
 * Return    : void
 * Comment   : 
********************************************************************/
void LCD_Display_ASCII(u8 xs,u8 ys,u8 size,u8 *buff,u16 color)
{
	if(size!=16 &&size !=24 && size !=32)  return ;
		if(size==24)
	{
	//设置地址
		LCD_SetWindows(xs, ys,xs+16-1,ys+size-1);
	 
		//数组的元素个数
		for(u16 i=0;i<48;i++){
		//每个元素操作的次数
		for(u8 j=0;j<8;j++){
			if(buff[i]&(0x80>>j)){
				Lcd_WriteData_16Bit(color);}
			else{
				Lcd_WriteData_16Bit(0XFFFF);}
		}		
	 }
  }
	
	else{
	//设置地址
	LCD_SetWindows(xs, ys,xs+size/2-1,ys+size-1);
	
	//判断数据的每个位，发送颜色
	for(u16 i=0;i<size*size/16;i++)//数组的元素个数
	{
		for(u8 j=0;j<8;j++)//每个元素操作的次数
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
 * Function  : LCD显示字符串（基于自建库）
 * Parameter : u8 xs,u8 ys,u8 size,u8 *buff,u16 color
 * Return    : 
 * Comment   : "南京 欢迎你 123"
********************************************************************/

void LCD_Display_STRING( u8 xs,u8 ys,u8 size,u8 *buff,u16 color)
{
	 while(*buff){
    //基于ASCII判断是否是字符
	  if(*buff>=0xA1&&*(buff+1)>=0xA2&&*buff<=0xf7&&*(buff+1)<=0xfe){
			for(u8 i=0;i<strlen(hzk);i++){
				//字符库有这个字符
				if(*buff==hzk[2*i]&&*(buff+1)==hzk[2*i+1]){
					LCD_Display_Chinese(xs,ys,size,(u8*)(hzm+i*size*size/8),color);
					//个人理解：找到成功显示后 结束本轮寻找 避免浪费CPU资源
				  break;}
			  }
		//一轮操作之后 让地址偏移一位到下一个字符
		buff+=2;
		//写完一个字 让列地址偏移一个字的大小
		xs += size;
		if(xs + size  > 239){
		ys += size;
		xs  = 0;}
		}
		else if(*buff <= 0xF7){
			for(u8 i=0;i<strlen(zfk);i++){
				//字符库有这个字符
				if(*buff==zfk[i]){
					LCD_Display_ASCII(xs,ys,size,(u8*)(zfm+i*size*size/16),color);
					//个人理解：找到成功显示后 结束本轮寻找 避免浪费CPU资源
				  break;}
			  }
		//一轮操作之后 让地址偏移一位到下一个字符
		buff++;
		//写完一个字 让列地址偏移一个字的大小
		xs += size/2;
		if(xs + size/2  > 239){
			ys += size;
			xs  = 0;}
		}
		}

	}

	
	



