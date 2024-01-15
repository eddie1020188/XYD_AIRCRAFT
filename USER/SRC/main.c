#include "main.h"

extern const unsigned char gImage_pyy[];

int main(void)
{
	LCD_Init();
  int  high;
	
	char TEMP[20];

	LCD_Display_STRING( 0,0,32,(u8*)"南京信盈达",BRRED);
	
	
	for(high =0;high<100;high++)
	{
		sprintf(TEMP,"HIGH: %d",high);
	  LCD_Display_STRING( 0,32,32,(u8*)TEMP,BRRED);
		delay_ms(100);
	}
	
	while(1){
	

	}
	          
	
}
