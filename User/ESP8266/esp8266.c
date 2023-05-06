# include "esp8266.h"

uint8_t Rxbuf[512];
uint8_t Rxbuf1[512];

char buf[128];
//通过串口三向8266写入AT指令
#define LOG 1
void ESP8266_Send(char *str)
{
	//不可以用sizeof
	HAL_UART_Transmit(&huart2,(uint8_t*)str,strlen(str),1000);
}

//寻找子串
//找到返回1，否者返回0
uint8_t FindSonStr(char *str1,char * str2)
{
	HAL_Delay(10);
	if(strstr(str1,str2)!=NULL)
	{
		return 1;
	}
	else{
		return 0;
	}
	
}

//向8266发送恢复出厂设置指令
//握手成功返回1，否者返回0
uint8_t ESP8266_Init()
{

	//ESP8266_Send("AT\r\n");
	int count = 5;
	while(count--)
	{
		memset(Rxbuf,0,128);
		ESP8266_Send("AT+RESTORE\r\n");
		HAL_UART_Receive(&huart2,Rxbuf,128,1000);
		#if LOG
	//HAL_UART_Transmit(&huart1,(uint8_t*)Rxbuf,sizeof(Rxbuf),1000);
		printf("AT:%s\r\n",Rxbuf);
		#endif
		if(FindSonStr((char*)Rxbuf,"OK"))
		{

			break;
		}
		HAL_Delay(1000);
	}
	
}

//设置模式

void ESP8266_SETMode()
{
	uint8_t count =5;
	while(count--)
	{
		memset(buf,0,128);
		memset(Rxbuf1,0,512);
		sprintf(buf,"AT+CWMODE=%d\r\n",1);
		ESP8266_Send(buf);
		HAL_UART_Receive(&huart2,(uint8_t*)&Rxbuf1,512,1000);
		#if LOG 
		printf("AT+CWMODE=%s\r\n",Rxbuf1);
		# endif
		//printf("%s",buf);

		if(FindSonStr((char*)Rxbuf1,"OK"))
		{

			break;
		}
		HAL_Delay(1000);
	}
}

//连接热点
void ESP8266_ConnectAP()
{
	ESP8266_SETMode();
	uint8_t count =5;
	char ch[128];
	while(count--)
	{
		memset(Rxbuf,0,128);
		memset(ch,0,128);
		sprintf(ch,"AT+CWJAP=\"%s\",\"%s\"\r\n",WIFI_NAME,WIFI_PASSWORD);
		ESP8266_Send(ch);
		HAL_UART_Receive(&huart2,Rxbuf,512,1000);
		#if LOG
		printf("AT+CWJAP:%s\r\n",Rxbuf);
		#endif
		if(FindSonStr((char*)Rxbuf,"OK")|| FindSonStr((char*)Rxbuf,"CONNECTED"))
		{

			break;
		}
		HAL_Delay(1000);
	}
}

//ESP8266配置MQTT信息
void ESP8266_ConfigMQTT()
{
	char ch[128];
	int count = 5;
	while(count--)
	{
		memset(ch,0,128);
		memset(Rxbuf,0,512);
		//AT+MQTTUSERCFG=<LinkID>,<scheme>,<"client_id">,<"username">,<"password">,<cert_key_ID>,<CA_ID>,<"path">
		sprintf(ch,"AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n",ClientID,USERNAME,PASSWORD);
		ESP8266_Send(ch);
		HAL_UART_Receive(&huart2,Rxbuf,512,1000);
		#if LOG
		printf("AT+MQTTUSERCFG:%s\r\n",Rxbuf);
		#endif
		if(FindSonStr((char*)Rxbuf,"OK"))
		{
			break;
		}
		HAL_Delay(1000);
	}
}
//配置CLIENTID
void CONFIG_CLIENTID()
{
	char ch[128];
	while (1)
	{
		memset(ch,0,128);
		memset(Rxbuf,0,512);
		sprintf(ch,"AT+MQTTCLIENTID=0,\"%s\"\r\n",ClientID);
		ESP8266_Send(ch);
		HAL_UART_Receive(&huart2,Rxbuf,512,1000);
		printf("AT+MQTTCLIENTID:%s\r\n",Rxbuf);
				
		if(FindSonStr((char*)Rxbuf,"OK"))
		{
			break;
		}
		HAL_Delay(1000);
	}
}

//配置CLIENTID
void CONFIG_PASSWORD()
{
	char ch[128];
	while (1)
	{
		memset(ch,0,128);
		memset(Rxbuf,0,512);
		sprintf(ch,"AT+MQTTPASSWORD=0,\"%s\"\r\n",PASSWORD);
		ESP8266_Send(ch);
		HAL_UART_Receive(&huart2,Rxbuf,512,1000);
		//printf("AT+MQTTPASSWORD:%s\r\n",Rxbuf);
				
		if(FindSonStr((char*)Rxbuf,"OK"))
		{
			break;
		}
		HAL_Delay(1000);
	}
}


//配置CLIENTID
void CONFIG_USERNAME()
{
	char ch[128];
	while (1)
	{
		memset(ch,0,128);
		memset(Rxbuf,0,512);
		sprintf(ch,"AT+MQTTUSERNAME=0,\"%s\"\r\n",USERNAME);
		ESP8266_Send(ch);
		HAL_UART_Receive(&huart2,Rxbuf,512,1000);
		printf("AT+MQTTUSERNAME:%s\r\n",Rxbuf);
				
		if(FindSonStr((char*)Rxbuf,"OK"))
		{
			break;
		}
		HAL_Delay(1000);
	}
}
//连接服务器

void ESP8266_Connect()
{
	ESP8266_ConfigMQTT();
	char ch[128];
	uint8_t count = 5;
	while(count--)
	{
		memset(ch,0,128);
		memset(Rxbuf,0,512);
		//AT+MQTTCONN=<LinkID>,<"host">,<port>,<reconnect>
		sprintf(ch,"AT+MQTTCONN=0\,\"%s\"\,1883\,1\r\n",HOST);
		//sprintf(ch,"AT+MQTTCONN=0,%s,1883,1\r\n",HOST);
		ESP8266_Send(ch);
		HAL_UART_Receive(&huart2,Rxbuf,512,1000);
		#if LOG
		printf("AT+MQTTCONN:%s\r\n",Rxbuf);
		#endif
		if(FindSonStr((char*)Rxbuf,"OK"))
		{
			break;
		}
		HAL_Delay(1000);
	}
}

//发布信息
void ESP8266_Pub(char *topic,char* message)
{

	char ch[32];
	memset(ch,0,32);
	//memset(Rxbuf,0,512);
	//AT+MQTTPUB=<LinkID>,<"topic">,<"data">,<qos>,<retain>
	sprintf(ch,"AT+MQTTPUB=0\,\"%s\"\,\"%s\"\,0\,0\r\n",topic,message);
	ESP8266_Send(ch);
	//HAL_UART_Receive(&huart2,Rxbuf,512,1000);
	#if LOG
	//printf("AT+MQTTPUB:%s\r\n",Rxbuf);
	#endif
}

//订阅信息

















   