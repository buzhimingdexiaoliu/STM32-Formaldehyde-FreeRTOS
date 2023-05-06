/*
 * @Author: your name
 * @Date: 2022-02-27 16:50:53
 * @LastEditTime: 2022-02-27 17:06:58
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \MDK-ARMf:\CubeMXProject\XiaoKeFeng\User\OLED\bsp_i2c.h
 */
# ifndef __BSP_I2C_H
# define __BSP_I2C_H
# include "main.h"


/**********功能宏定义*************/
# define I2C_Port GPIOB
# define SDA  GPIO_PIN_4
# define SCL  GPIO_PIN_5

# define SCL_H() 	HAL_GPIO_WritePin(I2C_Port,SCL,1)
# define SDA_H()	HAL_GPIO_WritePin(I2C_Port,SDA,1)

# define SCL_L() 	HAL_GPIO_WritePin(I2C_Port,SCL,0)
# define SDA_L()	HAL_GPIO_WritePin(I2C_Port,SDA,0)

# define SDA_In() HAL_GPIO_ReadPin(I2C_Port,SDA)


void I2C_Start(void);
void I2C_Stop(void);
int I2C_GetACK(void);
void I2C_ACK(void);
void I2C_NACK(void);
void I2C_WriteByte(uint8_t data);
uint8_t I2C_ReadByte();
# endif

