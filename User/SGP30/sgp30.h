/*
 * @Author: Dell
 * @Date: 2022-02-26 20:01:59
 * @LastEditTime: 2022-02-27 11:21:25
 * @LastEditors: Please set LastEditors
 * @Description: 
 * @FilePath: \MDK-ARMf:\CubeMXProject\XiaoKeFeng\User\SGP30\sgp30.h
 */
#ifndef __HX711_H
#define __HX711_H
#include "sys.h"


#define HAl_lib
#define  SGP30_SCL   PBout(0)
#define  SGP30_SDA   PBout(1)

#ifdef standard_lib
// USART GPIO 引脚宏定义
#define  SGP30_SCL_GPIO_CLK        RCC_APB2Periph_GPIOB
#define  SGP30_SCL_GPIO_PORT       GPIOB
#define  SGP30_SCL_GPIO_PIN        GPIO_Pin_0

#define  SGP30_SDA_GPIO_SDA        RCC_APB2Periph_GPIOB
#define  SGP30_SDA_GPIO_PORT       GPIOB
#define  SGP30_SDA_GPIO_PIN        GPIO_Pin_1
#endif

#define  SGP30_SDA_READ()          HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)

#define SGP30_read  0xb1  //SGP30的读地址
#define SGP30_write 0xb0  //SGP30的写地址

typedef struct sgp30_data_t
{
    /* data */
    float CO2;
    float TVOS;
    
}sgp30_data_t,*psgp30_data_t;


void SGP30_IIC_Start(void);				//发送IIC开始信号
void SGP30_IIC_Stop(void);	  			//发送IIC停止信号
void SGP30_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint16_t SGP30_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t SGP30_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void SGP30_IIC_Ack(void);					//IIC发送ACK信号
void SGP30_IIC_NAck(void);				//IIC不发送ACK信号
void SGP30_IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t SGP30_IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	
void SGP30_Init(void);				  
void SGP30_Write(uint8_t a, uint8_t b);
uint32_t SGP30_Read(void);



#endif

