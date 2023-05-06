/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "usart.h"
#include "sgp30.h"
#include "esp8266.h"
#include "bsp_oled.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define LOG 0
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TOPIC "/SGP30/Data"
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
psgp30_data_t g_psgp30_data; 
  //定队列句柄
QueueHandle_t queue1;  
extern char* status;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId SGP30TaskHandle;
osThreadId LEDTaskHandle;
osThreadId OLEDTaskHandle;
osThreadId PubTaskHandle;
osThreadId BLETaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void SGP30TaskStart(void const * argument);
void LEDTaskStart(void const * argument);
void OLEDTaskStart(void const * argument);
void PubTaskStart(void const * argument);
void BLETaskStart(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  //SGP30模块初始化
  #if LOG
	printf("start success... ...\r\n");
  #endif
  SGP30_Init();   //sgp30初始化
  ESP8266_Init();
  HAL_Delay(1000);
  ESP8266_ConnectAP();
  HAL_Delay(1000);
  ESP8266_Connect();
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  //创建队列 
  queue1 = xQueueCreate(10,sizeof(struct sgp30_data_t));
  if(queue1 == pdFALSE)
  {
    #if LOG
    printf("队列创建失败\r\n");
    #endif
  }
  else
  {
    #if LOG
     printf("队列queue1创建成功\r\n");
     #endif
  }
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityIdle, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of SGP30Task */
  osThreadDef(SGP30Task, SGP30TaskStart, osPriorityIdle, 0, 128);
  SGP30TaskHandle = osThreadCreate(osThread(SGP30Task), NULL);

  /* definition and creation of LEDTask */
  osThreadDef(LEDTask, LEDTaskStart, osPriorityIdle, 0, 128);
  LEDTaskHandle = osThreadCreate(osThread(LEDTask), NULL);

  /* definition and creation of OLEDTask */
  osThreadDef(OLEDTask, OLEDTaskStart, osPriorityIdle, 0, 128);
  OLEDTaskHandle = osThreadCreate(osThread(OLEDTask), NULL);

  /* definition and creation of PubTask */
  osThreadDef(PubTask, PubTaskStart, osPriorityIdle, 0, 128);
  PubTaskHandle = osThreadCreate(osThread(PubTask), NULL);

  /* definition and creation of BLETask */
  osThreadDef(BLETask, BLETaskStart, osPriorityIdle, 0, 128);
  BLETaskHandle = osThreadCreate(osThread(BLETask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  默认任务 unuse
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  
  for(;;)
  {
    //HAL_UART_Transmit(&huart2,ch,sizeof(ch),1000);
		//printf("StartDefaultTask\r\n");
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_SGP30TaskStart */
/**
* @brief SGP30检测任务
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SGP30TaskStart */
void SGP30TaskStart(void const * argument)
{
  /* USER CODE BEGIN SGP30TaskStart */
  /* Infinite loop */
  uint32_t sgp30_dat;
  uint32_t CO2Data,TVOCData;//定义CO2浓度变量与TVOC浓度变量
  static uint16_t count = 30;
  static uint32_t average_CO2=0,average_TVOS = 0,ave_count = 5;
  for(;;)
  {
    #if LOG
    printf("SGP30TaskStart\r\n");
    #endif
    SGP30_Write(0x20,0x08);
	  sgp30_dat = SGP30_Read();//读取SGP30的值
	  CO2Data = (sgp30_dat & 0xffff0000) >> 16;
	  TVOCData = sgp30_dat & 0x0000ffff;
    if(CO2Data == 400 && TVOCData == 0 && count)
    {
//      SGP30_Write(0x20,0x08);
//      sgp30_dat = SGP30_Read();//读取SGP30的值
//      CO2Data = (sgp30_dat & 0xffff0000) >> 16;//取出CO2浓度值
//      TVOCData = sgp30_dat & 0x0000ffff;			 //取出TVOC值
      count--;
      #if LOG
      printf("正在检测中...\r\n");
      #endif
      
    }else{
//      SGP30_Write(0x20,0x08);
//      sgp30_dat = SGP30_Read();//读取SGP30的值
//      CO2Data = (sgp30_dat & 0xffff0000) >> 16;//取出CO2浓度值
//      TVOCData = sgp30_dat & 0x0000ffff;       //取出TVOC值**/
        while(ave_count)
        {
            SGP30_Write(0x20,0x08);
            sgp30_dat = SGP30_Read();//读取SGP30的值
            CO2Data = (sgp30_dat & 0xffff0000) >> 16;//取出CO2浓度值
            TVOCData = sgp30_dat & 0x0000ffff;  
            average_CO2 += CO2Data;
            average_TVOS += TVOCData;
            ave_count --; 
        }
        sgp30_data_t sgp30_data;
        #if LOG
        printf("CO2:%.2fppm\r\nTVOC:%.2fppb\r\n",average_CO2/5.00,average_TVOS/5.00);
        #endif
        sgp30_data.CO2 = average_CO2/5.00;
        sgp30_data.TVOS = average_TVOS/5.00;
        xQueueSend(queue1,(void*)&sgp30_data,portMAX_DELAY);

          /**if((average_TVOS/5.00)>30)
          {
            HAL_GPIO_WritePin(GPIOA,GREEN_Pin,0);
            HAL_GPIO_WritePin(GPIOA,BLUE_Pin,0);
            HAL_GPIO_WritePin(GPIOA,RED_Pin,1);
          }
          else if((average_TVOS/5.00)>20)
          {
            HAL_GPIO_WritePin(GPIOA,GREEN_Pin,0);
            HAL_GPIO_WritePin(GPIOA,BLUE_Pin,1);
            HAL_GPIO_WritePin(GPIOA,RED_Pin,0);
          }
          else if((average_TVOS/5.00)>0)
          {
            HAL_GPIO_WritePin(GPIOA,GREEN_Pin,1);
            HAL_GPIO_WritePin(GPIOA,BLUE_Pin,0);
            HAL_GPIO_WritePin(GPIOA,RED_Pin,0);
          }*/
          /*uint8_t sendData = 10;
          xReturn = xQueueSend(queue1,&sendData,portMAX_DELAY);
          if(xReturn!=pdPASS)
          {
            printf("Send Error\r\n");
          }
          else{
            printf("Send Success\r\n");
          }*/
        average_TVOS =0;
        average_CO2 = 0;
        ave_count =5;
    }
    osDelay(500);
  }
  /* USER CODE END SGP30TaskStart */
}

/* USER CODE BEGIN Header_LEDTaskStart */
/**
* @brief LED任务
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LEDTaskStart */
void LEDTaskStart(void const * argument)
{
  /* USER CODE BEGIN LEDTaskStart */
  /* Infinite loop */
  BaseType_t xReturn;
  sgp30_data_t sgp30_data;
  char CO2[128];
  char TVOS[128];
  for(;;)
  {
    #if LOG
    printf("Start LEDTask\r\n");
    #endif
    xReturn =  xQueueReceive(queue1,(void*)&sgp30_data,portMAX_DELAY);
    if(xReturn)
    {
     /* sprintf(ch,"{\"CO2\":%.2f,\"TVOS\":%.2f}",sgp30_data.CO2,sgp30_data.TVOS);
      ESP8266_Pub(TOPIC,ch);
      memset(ch,0,sizeof(ch));*/
      #if LOG
      printf("sgp30_data->CO2:%.2f,sgp30_data->TVOS:%.2f\r\n",sgp30_data.CO2,sgp30_data.TVOS);
      #endif
			//不同浓度对应不同的颜色
      if(sgp30_data.TVOS>30)
      {
        HAL_GPIO_WritePin(GPIOA,GREEN_Pin,0);
        HAL_GPIO_WritePin(GPIOA,RED_Pin,1);
        HAL_GPIO_WritePin(GPIOA,BLUE_Pin,0);
      }
      else if (sgp30_data.TVOS>20)
      {
        /* code */
        HAL_GPIO_WritePin(GPIOA,GREEN_Pin,0);
        HAL_GPIO_WritePin(GPIOA,RED_Pin,0);
        HAL_GPIO_WritePin(GPIOA,BLUE_Pin,1);
      }
      else if (sgp30_data.TVOS>0)
      {
        /* code */
        HAL_GPIO_WritePin(GPIOA,GREEN_Pin,1);
        HAL_GPIO_WritePin(GPIOA,RED_Pin,0);
        HAL_GPIO_WritePin(GPIOA,BLUE_Pin,0);
      } 
    }
    osDelay(1);
  }
  /* USER CODE END LEDTaskStart */
}

/* USER CODE BEGIN Header_OLEDTaskStart */
/**
* @brief Function implementing the OLEDTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OLEDTaskStart */
void OLEDTaskStart(void const * argument)
{
  /* USER CODE BEGIN OLEDTaskStart */
  /* Infinite loop */
  BaseType_t xReturn;
  sgp30_data_t sgp30_data;
  char CO2[64];
  char TVOS[64];
  for(;;)
  {
     xReturn =  xQueueReceive(queue1,(void*)&sgp30_data,portMAX_DELAY);
     if(xReturn)
     {
       //printf("___sgp30_data->CO2:%.2f,sgp30_data->TVOS:%.2f\r\n",sgp30_data.CO2,sgp30_data.TVOS);
       #if LOG
       printf("Start OLEDTask\r\n");
       #endif
        sprintf(CO2,"CO2:%2.f",sgp30_data.CO2);
        sprintf(TVOS,"TVOS:%2.f",sgp30_data.TVOS);
        OLED_Clear();
        OLED_ShowString(4,0,CO2);   //显示CO2浓度
        OLED_ShowString(6,0,TVOS);  //显示甲醛等气体浓度
        memset(CO2,0,sizeof(CO2));
        memset(TVOS,0,sizeof(TVOS));
     }
    osDelay(1);
  }
  /* USER CODE END OLEDTaskStart */
}

/* USER CODE BEGIN Header_PubTaskStart */
/**
* @brief Function implementing the PubTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PubTaskStart */
void PubTaskStart(void const * argument)
{
  /* USER CODE BEGIN PubTaskStart */
  /* Infinite loop */
  BaseType_t xReturn;
  sgp30_data_t sgp30_data;
  char ch1[16];
  char ch[32];
	
  for(;;)
  {
    xReturn = xQueueReceive(queue1,(void*)&sgp30_data,portMAX_DELAY);
    if(xReturn)
    {
      #if LOG
      printf("Start PublishTask\r\n");
      #endif
      sprintf(ch1,"%.2f\\,%.2f",sgp30_data.CO2,sgp30_data.TVOS);
      //ESP8266_Pub(Topic,ch1);
      sprintf(ch,"AT+MQTTPUB=0,\"%s\",\"%s\",0,0\r\n",Topic,ch1);  //上传至云服务器
	    ESP8266_Send(ch);
      memset(ch,0,32);
      memset(ch1,0,sizeof(ch1));
     // printf("1111\r\n");
    }
    osDelay(1);
  }
  /* USER CODE END PubTaskStart */
}

/* USER CODE BEGIN Header_BLETaskStart */
/**
* @brief Function implementing the BLETask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_BLETaskStart */
void BLETaskStart(void const * argument)
{
  /* USER CODE BEGIN BLETaskStart */
  /* Infinite loop */
  BaseType_t xReturn;
  sgp30_data_t sgp30_data;
  uint8_t ch[18];
  uint8_t ch1[18];
  for(;;)
  {
    xReturn = xQueueReceive(queue1,(void*)&sgp30_data,portMAX_DELAY);
    if(xReturn)
    {
      int data = sgp30_data.TVOS;
      if(data<10)
      {
        sprintf(ch,"AT+TEST=2,0282,0%x\r\n",data);
        sprintf(ch1,"AT+TEST=6,0282,0%x\r\n",data);
      }
       else
      {
        sprintf(ch,"AT+TEST=2,0282,%d\r\n",data);
        sprintf(ch1,"AT+TEST=6,0282,%d\r\n",data);
      }
      HAL_UART_Transmit(&huart1,ch,19,1000);     //向地址位0x2的蓝牙设备发送数据
      osDelay(100);
      HAL_UART_Transmit(&huart1,ch1,19,1000);    //向地址为0xA的蓝牙设备发送数据
      memset(ch,0,sizeof(ch));
      memset(ch1,0,sizeof(ch1));
      }
    osDelay(1);
  }
  /* USER CODE END BLETaskStart */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

