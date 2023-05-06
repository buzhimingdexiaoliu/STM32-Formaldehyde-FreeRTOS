/*
 * @Author: Dell
 * @Date: 2022-02-27 12:51:01
 * @LastEditTime: 2022-03-01 21:57:32
 * @LastEditors: Please set LastEditors
 * @Description: 
 * @FilePath: \MDK-ARMf:\CubeMXProject\XiaoKeFeng\User\ESP8266\esp8266.h
 */
# ifndef __ESP8266_H
# define __ESP8266_H
# include "main.h"
# include "usart.h"
# include "stdio.h"
# include "string.h"
# define WIFI_NAME "DELL"
# define WIFI_PASSWORD "DELL123456"
# define ClientID "2202271515"   //需要保证不重复故此处使用22-02-27 13:55
# define USERNAME ""
# define PASSWORD ""
# define HOST "118.190.217.222" //服务器IP
#define Topic "from/SGP30/Data"


void ESP8266_Send(char *str);
uint8_t ESP8266_Init();
void ESP8266_SETMode();
void ESP8266_ConnectAP();
void ESP8266_ConfigMQTT();
void ESP8266_Connect();
void ESP8266_Pub(char *topic,char* message);
# endif