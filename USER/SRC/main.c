#include "main.h"

VL53L0X_Dev_t VL53L0x_dev;
VL53L0X_RangingMeasurementData_t VL53L0x_data;
extern const unsigned char gImage_pyy[];
vu16 Distance = 0;   //保存测距数据
int main(void)
{
	LCD_Init();
  int  high;
	char buff[20];
	char TEMP[20];
	LCD_Display_STRING( 0,0,32,"AAAAAAAAAAAA",BRRED);
  VL53L0x_Init(&VL53L0x_dev);
	LCD_Display_STRING( 0,32,32,"BBBBBBBBBBBB",BRRED);
//	LCD_Display_STRING( 0,0,32,(u8*)"南京信盈达",BRRED);
	
	
	while(1){
//	VL53L1_single_test(&VL53L0x_dev,&VL53L0x_data);
//	sprintf(buff,"DISTANCE:%d",Distance);
//  LCD_Display_STRING( 0,0,32,(u8*)buff,BRRED);
	}
	          
	
}
