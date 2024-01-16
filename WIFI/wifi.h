#ifndef WIFI_H
#define WIFI_H

#include "stm32f4xx.h"
#include "usart2.h"
#include "lcd.h"
_Bool WIFI_TCP_Init(void);
char WIFI_SendAT(char *AT,char*respond);
void WIFI_ConnectAP(char *ssid,char*pwd);
void WIFI_ExitTransmit(void);
void WIFI_ConnectServer(char *type,char* remote_IP,char* remote_Port);
_Bool WIFI_ConnectTalientek(char *numb,char*pwd);
#endif
