
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
* @describe:iic��������
* ��SCLΪ�ߵ�ƽʱ��SDA�иߵ�ƽ��Ϊ�͵�ƽ
*/
void I2C_Start(void)
{
	SCL_H();         //����ʱ�����߲������ȶ�
	SDA_H();         //������������
	I2C_Delay(100);    
	SDA_L();				 //������������
	I2C_Delay(100);
}

/**
* @describe��iicֹͣ�ź�
* ��SCLΪ�ߵ�ƽʱ��SDA�ɵ͵�ƽ��ߵ�ƽ
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
* @describe: ��ȡ�ӻ���Ӧ���ź�
*/

int I2C_GetACK(void)
{
	int i = 0;
	SCL_L(); 					//����SCL
	SDA_H(); 					//��©���ģʽ������SDA�ͷ�SDA���ߣ��ߵ�ƽ���ŵ�ƽ��ƽ���ⲿ�豸����
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
	}									//�ȴ������͵�ƽ������Ӧ��
	SCL_L();    			//�ȴ��ӻ����������߼�
	return 1;
}

/**
* @describe:��������Ӧ���ź�
* ��SDAΪ�͵�ƽʱSCL�ɵ͵�ƽ��Ϊ�ߵ�ƽ
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
* @describe:�������ͷ�Ӧ���ź�
* SCL�ɵ͵�ƽ��Ϊ�ߵ�ƽʱSDAά�ָߵ�ƽ
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
* @describe�����豸д������
*/
void I2C_WriteByte(uint8_t data)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		SCL_L();               //����SCL
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
	I2C_GetACK();            //��ȡӦ��
}

/**
* @describe����ȡI2C����
*/

uint8_t I2C_ReadByte()
{
	uint8_t data = 0,i;
	SDA_H();                 //�ͷ�SDA����
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

