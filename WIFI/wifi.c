
#include "wifi.h"
#include "main.h"

#define IPHONE 0
#define XYD 1

#if IPHONE 

#define WIFI_ssid  "Eddie"
#define WIFI_pwd   "sjw19980807"
#endif

#if XYD 

#define WIFI_ssid  "TP-LINK_XYD333"
#define WIFI_pwd   "xydedu333"

#endif

#define Remote_IP "cloud.alientek.com"
#define Remote_Port "59666"

#define ClientID "a1l9hrXixQO.HYZ_Smart|securemode=2,signmethod=hmacsha256,timestamp=1688625698773|"
#define username "HYZ_Smart&a1l9hrXixQO"
#define Password "9049db3501cf864ad59b61e175c433a5f512c500d0c3f56b9ed7323bc59d2c69"

#define Publish_Topic "/sys/a1l9hrXixQO/HYZ_Smart/thing/event/property/post"
#define Subscirbe_Topic "/sys/a1l9hrXixQO/HYZ_Smart/thing/service/property/set"

#define TALIENTEK_NUMB "29113171219058142211"
#define TALIENTEK_PWD  "12345678"




/*******************************************************************
 * Function  : WIFIģ���ʼ��
 * Parameter : void
 * Return    : ����ֵ1��ʾ���ӳɹ�
 * Comment   : ��
********************************************************************/

_Bool WIFI_TCP_Init(void)
{
	 _Bool RET = 0;
	 u8 ret;
	
	 WIFI_SendAT("AT+RESTORE\r\n","OK");
	 delay_ms(2000);
	 WIFI_SendAT("AT\r\n","OK");
	 WIFI_ExitTransmit();
	 WIFI_SendAT("AT\r\n","OK");
	 WIFI_SendAT("AT+CWMODE=1\r\n","OK");
	 WIFI_ConnectAP(WIFI_ssid,WIFI_pwd);
	 WIFI_ConnectServer("TCP",Remote_IP,Remote_Port);
	 RET = WIFI_ConnectTalientek(TALIENTEK_NUMB,TALIENTEK_PWD);
   
}

/*******************************************************************
 * Function  : wifi����ATָ��
 * Parameter : char *AT,char *respond
 * Return    : char
 * Comment   : 1 ����ʱ 2����ʧ��  0����ɹ�
********************************************************************/
char WIFI_SendAT(char *AT,char*respond)
{
	 u16 time=0;
	USART2_SendStr(AT);
	while(!ESP12rev.RevOver)
	{
		delay_ms(1);
		time++;
		if(time>1000)
		{
			printf("-----TIME OUT!!!----- \r\n");
			
			return 1;
		}
	}
	ESP12rev.RevOver=0;
	if(strstr(ESP12rev.RevBuf,respond)==NULL)
	{
		return 2;
	}
	else
		return 0;
}

/*******************************************************************
 * Function  : WIFI����AP
 * Parameter : char *ssid,char *pwd
 * Return    : void
 * Comment   : "AT+CWJAP=\"ssid\",\"pwd\"\r\n"
********************************************************************/
void WIFI_ConnectAP(char *ssid,char*pwd)
{
	u16 time = 0;
	u8 ret=WIFI_SendAT("AT+CIFSR\r\n","0.0.0.0");
	if(ret==0)//����Ϊ��û��������
	{
		char buff[50]="AT+CWJAP=";
		strcat(buff,"\"");
		strcat(buff,WIFI_ssid);
		strcat(buff,"\",\"");
		strcat(buff,WIFI_pwd);
		strcat(buff,"\"\r\n");
		USART2_SendStr(buff);
		while(1)
		{
			delay_ms(1);
			time ++;
			if(time > 10000)
			{
			LCD_Display_STRING(0,0,32,(u8*)"CONNECT AP FAILED",BRRED); 
			return ;
			}
			if(ESP12rev.RevOver==1)
			{
				if(strstr(ESP12rev.RevBuf,"OK")!=NULL)
				{
					printf("AP Connect Success\r\n");
					ESP12rev.RevOver=0;
					return ;
				}
				ESP12rev.RevOver=0;
			}
		}
	}
	else
	{
		printf("AP Have Connect Success\r\n");
	}
}


/*******************************************************************
 * Function  : WIFI���ӷ�����
 * Parameter : char *type��char* remote_IP  char* remote_Port
 * Return    : void
 * Comment   : "AT+CIPSTART=\"type\",\"remote_IP\",remote_Port\r\n"
********************************************************************/
void WIFI_ConnectServer(char *type,char* remote_IP,char* remote_Port)
{

		char buff[100]="AT+CIPSTART=";
		strcat(buff,"\"");
		strcat(buff,type);
		strcat(buff,"\",\"");
		strcat(buff,remote_IP);
		strcat(buff,"\",");
	  strcat(buff,remote_Port);
	  strcat(buff,"\r\n");
		USART2_SendStr(buff);
		while(1)
		{
			if(ESP12rev.RevOver==1)
			{
				if(strstr(ESP12rev.RevBuf,"CONNECT")!=NULL)
				{
					printf("Server Connect Success\r\n");
					ESP12rev.RevOver=0;
					return ;
				}
				if(strstr(ESP12rev.RevBuf,"CLOSED")!=NULL)
				{
					printf("Server Connect Failed\r\n");
					ESP12rev.RevOver=0;
					return ;
				}
				ESP12rev.RevOver=0;
			}
		}
	}

/*******************************************************************
 * Function  : WIFI�˳�͸��ģʽ
 * Parameter : void
 * Return    : void
 * Comment   : ��+++��
********************************************************************/
void WIFI_ExitTransmit(void)
{
		USART2_SendStr("+++");
		delay_ms(500);
		delay_ms(500);
		WIFI_SendAT("AT+CIPCLOESE\r\n","OK");
}


/*******************************************************************
 * Function  : WIFI����ԭ����
 * Parameter : char *numb,char *pwd
 * Return    : void
 * Comment   : "AT+ATKCLDSTA=\"numb\",\"pwd\"\r\n"
********************************************************************/
_Bool WIFI_ConnectTalientek(char *numb,char*pwd)
{
	u16 time = 0;
    LCD_Display_STRING(0,0,32,(u8*)"TALIENTEK",BRRED); 
	  LCD_Display_STRING(0,32,32,(u8*)"CONNECTING",BRRED); 
		char buff[50]="AT+ATKCLDSTA=";
		strcat(buff,"\"");
		strcat(buff,numb);
		strcat(buff,"\",\"");
		strcat(buff,pwd);
		strcat(buff,"\"\r\n");
		USART2_SendStr(buff);
		while(1){
			delay_ms(1);
			time ++;
			if(time > 5000){
		  LCD_Display_STRING(0,0,32,(u8*)"TALIENTEK",BRRED); 
	    LCD_Display_STRING(0,32,32,(u8*)"CONNECT FAILED",BRRED); 
			return 0;}
			if(ESP12rev.RevOver==1){
				if(strstr(ESP12rev.RevBuf,"OK")!=NULL){
					printf("TALIENTEK Connect Success\r\n");
					LCD_Display_STRING(0,0,32,(u8*)"TALIENTEK",BRRED); 
	        LCD_Display_STRING(0,32,32,(u8*)" CONNECTED",BRRED); 
					ESP12rev.RevOver=0;
					return 1;}
				ESP12rev.RevOver=0;
			}
		}
	}

