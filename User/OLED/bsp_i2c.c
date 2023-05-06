
/**
 * @Author: Dell
 * @Date: 2021-11-21 12:51:01
 * @LastEditTime: 2022-02-27 15:15:32
 * @LastEditors: Dell
 * @Description: 
 * @FilePath: \MDK-ARMf:\CubeMXProject\XiaoKeFeng\User\bsp_i2c.c
 */

# include "bsp_i2c.h"


void I2C_Delay(uint32_t t)
{
    volatile uint32_t tmp = t;
    while(tmp--);
}

/**
* @describe:iic启动函数
* 当SCL为高电平时，SDA有高电平变为低电平
*/
void I2C_Start(void)
{
	SCL_H();         //拉高时钟总线并保持稳定
	SDA_H();         //拉高数据总线
	I2C_Delay(100);    
	SDA_L();				 //拉低数据总线
	I2C_Delay(100);
}

/**
* @describe：iic停止信号
* 当SCL为高电平时，SDA由低电平变高电平
*/
void I2C_Stop(void)
{
	SCL_H();
	SDA_L();
	I2C_Delay(100);
	SDA_H();
	I2C_Delay(100);
}

/**
* @describe: 获取从机的应答信号
*/

int I2C_GetACK(void)
{
	int i = 0;
	SCL_L(); 					//拉低SCL
	SDA_H(); 					//开漏输出模式，拉高SDA释放SDA总线，高电平引脚电平电平由外部设备控制
	I2C_Delay(100);
	SCL_H();
	while(SDA_In()) 
	{
		i++;
		if(i>=1000)
		{
			SCL_L();
			return -1;
		}
	}									//等待产生低电平，出现应答
	SCL_L();    			//等待从机处理其他逻辑
	return 1;
}

/**
* @describe:主机发送应答信号
* 当SDA为低电平时SCL由低电平变为高电平
*/
void I2C_ACK(void)
{
	SDA_L();
	SCL_L();
	I2C_Delay(100);
	SCL_H();
	I2C_Delay(100);
}


/**
* @describe:主机发送非应答信号
* SCL由低电平变为高电平时SDA维持高电平
*/
void I2C_NACK(void)
{
	SCL_L();
	SDA_H();
	I2C_Delay(100);
	SCL_H();
	I2C_Delay(100);
}
/**
* @describe：向设备写入数据
*/
void I2C_WriteByte(uint8_t data)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		SCL_L();               //拉低SCL
		I2C_Delay(100);
		if(data&0x80)
		{
			SDA_H();
		}
		else
		{
			SDA_L();
		}
		data<<=1;
		SCL_H();
		I2C_Delay(100);
	}
	I2C_GetACK();            //获取应答
}

/**
* @describe：读取I2C数据
*/

uint8_t I2C_ReadByte()
{
	uint8_t data = 0,i;
	SDA_H();                 //释放SDA总线
	for(i=0;i<8;i++)
	{
		SCL_L();
		I2C_Delay(100);
		SCL_H();
		data<<=1;
		if(SDA_In())
		{
			data |= 0x01;
		}
		else
		{
			data &= ~(0x01);
		}
	}
	I2C_ACK();
	return data;
}

