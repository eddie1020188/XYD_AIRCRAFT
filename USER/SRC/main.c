#include "main.h"

VL53L0X_Dev_t VL53L0x_dev;
VL53L0X_RangingMeasurementData_t VL53L0x_data;
extern const unsigned char gImage_pyy[];
vu16 Distance = 0;   //±£´æ²â¾àÊý¾Ý
int main(void)
{
	 char buff[20];
	_Bool FLAG;
	LCD_Init();
	USART1_Init(115200);
	USART2_Init(115200);
	key_init();
  VL53L0x_Init(&VL53L0x_dev);
	LCD_Display_STRING( 0,0,32,"WIFI",BRRED);
	LCD_Display_STRING( 0,32,32,"CONNECTING",BRRED);
	LCD_Display_STRING( 0,64,32,"WAIT",BRRED);
	FLAG = WIFI_TCP_Init();
	
	while(1){

	if(FLAG){
		
	  while(1){
			VL53L1_single_test(&VL53L0x_dev,&VL53L0x_data);
			sprintf(buff,"DISTANCE:%dMM",Distance);
			LCD_ClearPartial(130,64,240,96, WHITE);
			LCD_Display_STRING( 0,64,32,(u8*)buff,BRRED);
			USART2_SendStr(buff);
			delay_ms(1000);
		 }
	 }
	 
  }
 
	          
	
}
