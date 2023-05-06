# include "bsp_oled.h"
# include "bsp_i2c.h"
# include "font.h"

//向OLED写入命令
void OLED_Write_Cmd(uint8_t cmd)
{
	I2C_Start();
	I2C_WriteByte(Write_Addr); //写设备地址
	I2C_WriteByte(0x00); 			 //写控制指令，写命令
	I2C_WriteByte(cmd);				 //写命令
	I2C_Stop();
}

//向OLED写入数据
void OLED_Write_Data(uint8_t data)
{
	I2C_Start();
	I2C_WriteByte(Write_Addr); //写设备地址
	I2C_WriteByte(0x40); 			 //写控制指令，写命令
	I2C_WriteByte(data);				 //写命令
	I2C_Stop();
}

//向OLED写入多个数据
void OLED_Write_Datas(uint8_t* buf,uint8_t length)
{
	uint8_t i;
	I2C_Start();
	I2C_WriteByte(Write_Addr); //写设备地址
	I2C_WriteByte(0x40); 			 //写控制指令，写命令
	for(i=0;i<length;i++)
	{
		I2C_WriteByte(buf[i]);	 //写命令
	}
	I2C_Stop();
}



/**********基础命令**********/

//设置对比度 1-256

void OLED_Set_Contrast(uint8_t data)
{
	OLED_Write_Cmd(0x81);
	OLED_Write_Cmd(data);
}
//是否全部显示
void OLED_EntireDisplay()
{
	# ifdef EntireDisplay_On
		OLED_Write_Cmd(0xA5);
	# endif
	# ifdef EntireDisplay_Off	
		OLED_Write_Cmd(0xA4);
	# endif
}

//设置显示模式 正常模式和反转模式
void OLED_SetDisplayMode()
{
	# ifdef NormalMode
		OLED_Write_Cmd(0xA6);
	# else
		OLED_Write_Cmd(0xA7);
	# endif
}

//设置是否显示

void OLED_SetDisplay()
{
	# ifdef Display_On
	OLED_Write_Cmd(0xAF);
	# elif
	OLED_Write_Cmd(0xAE);
	# endif
}


/***********滚动命令Start***********/
//预留
/**************end******************/


/***********地址设置命令***********/


//设置列坐标

void OLED_SetCol_Start(uint8_t addr)
{
	if(addr>0x7f) 
	{
		return ;
	}
	if(AddrMode != Page_Addr_Mode)
	{
		return ;
	}
	OLED_Write_Cmd(0x00+(addr&0x0f));
	OLED_Write_Cmd(0x10+(addr>>4));
}

//设置模式
void OLED_SetMode(OLED_Mode mode)
{
	if((mode!= Page_Addr_Mode)&&(mode!=H_Addr_Mode)&&(mode!=V_Addr_Mode))
	{
 
		return;
	}
	OLED_Write_Cmd(0x20);
	OLED_Write_Cmd(mode);
}

//设置列起止地址
void OLED_Set_ColAddr(uint8_t start,uint8_t end)
{
	if(start>128||end>128) 
	{
		return;
	}
	OLED_Write_Cmd(0x21);
	OLED_Write_Cmd(start);
	OLED_Write_Cmd(end);
}

//设置行起止地址
void OLED_SetPageAddr(uint8_t start,uint8_t end)
{
	if(start>7||end>7) 
	{
		return;
	}
	OLED_Write_Cmd(0x22);
	OLED_Write_Cmd(start);
	OLED_Write_Cmd(end);
}

//设置行坐标
void OLED_SetPage_Start(uint8_t addr)
{
	if(addr>7) 
	{
		return;
	}
	
	if(AddrMode != Page_Addr_Mode)
	{
		return ;
	}
	OLED_Write_Cmd(0xB0+addr);
}

//设置坐标

void OLED_SetPosition(uint8_t page,uint8_t col)
{
	
		OLED_SetPage_Start(page);
		OLED_SetCol_Start(col);
}

/*************硬件配置***********/
void OLED_SetStartLine(uint8_t startLine)
{
	if(startLine>63)
	{
		return ;
	}
	OLED_Write_Cmd(0x7f&startLine);
}
//设置OLED重映射
void OLED_SetReMap()
{
	# ifdef No_ReMap
	OLED_Write_Cmd(0xA0);
	# endif
	
	# ifdef ReMap
	OLED_Write_Cmd(0xA1);
	# endif
}
//设置多路比率
void OLED_SetMUXRatio(uint8_t data)
{
	if(data<15||data>64)
	{
		return;
	}
	OLED_Write_Cmd(0xA8);
	OLED_Write_Cmd(data);
}

//设置扫描模式
void OLED_SetScanMode()
{
	# ifdef ScanMode_Normal
	OLED_Write_Cmd(0xC0);
	# endif
	# ifdef ScanMode_Remapper
	OLED_Write_Cmd(0xC8);
	# endif
}

//设置显示偏移
void OLED_SetDisplayOffset(uint8_t offset)
{
	if(offset<0||offset>63)
	{
		return;
	}
	OLED_Write_Cmd(0xD3);
	OLED_Write_Cmd(offset);
}

void OLED_SetCOMPins(isSequential mode1, isRemapper mode2)
{
	if(mode1!=Sequential || mode1!=Alternative)
	{
		return ;
	}
	if(mode2!= NoRemapper || mode2 != Remapper)
	{
		return ;
	}
	OLED_Write_Cmd(0xDA);
	OLED_Write_Cmd(0x02 | (mode1 << 4) | (mode2 << 5));
}


/**********设置时间相关命令*****************/

//设置时钟和分频系数
void OLED_SetClockFrequency(uint8_t clock,uint8_t freq)
{
	if(clock>0x0f||freq>0x0f)
	{
		return ;
	}
	OLED_Write_Cmd(0xD5);
	OLED_Write_Cmd(clock+(freq<<4));
}

//设置预分频系数

void OLED_Set_Period(uint8_t per1,uint8_t per2)
{
	if(per1>0x0f||per2>0x0f)
	{
		return ;
	}
	OLED_Write_Cmd(0xD9);
	OLED_Write_Cmd(per1+(per2<<4));
}
//设置V_com水平
void OLED_SetVcomLevel(Level level)
{
	if(level!=Low_Level&&level!=Mid_Level&&level!=High_Level)
	{
		return;
	}
	OLED_Write_Cmd(0xDB);
	OLED_Write_Cmd(level<<4);
}

/**电荷碰撞**/
void OLED_SetChargePump()
{
	OLED_Write_Cmd(0x8D);
	# ifdef ChargePump_Enable
	OLED_Write_Cmd(0x14);
	# elif ChargePump_Disable
	OLED_Write_Cmd(0x00);
	# endif
}

//初始化
void OLED_Init()
{
	HAL_Delay(100);
	OLED_SetMode(Page_Addr_Mode);							//设置内存模式					
	OLED_SetMUXRatio(0x3f);                   //设置多路复用
	OLED_SetDisplayOffset(0x00);							//偏移量	
	OLED_SetStartLine(0x40);									//开始行
	OLED_SetReMap();													//设置是否重映射
	OLED_SetScanMode();												//设置扫描模式
	OLED_SetCOMPins(0x01,0x00);								//设置引脚配置
	OLED_Set_Contrast(0x7f);									//设置对比度
	OLED_EntireDisplay();											//设置是否全屏显示
	OLED_SetDisplayMode();										//设置显示模式
	OLED_SetClockFrequency(0x00,0x08);				//设置时钟频率
	OLED_SetChargePump();											//设置电荷碰撞
	OLED_SetVcomLevel(Mid_Level);
	OLED_SetDisplay();												//打开显示
}

void OLED_Clear(void)
{
    uint8_t i = 0;
    uint8_t buf[128] = {0x00};
    
    for(i=0; i<8; i++)
    {
        OLED_SetPosition(i, 0);
        OLED_Write_Datas(&buf[0], 128);
    }
}

void OLED_ShowChar(uint8_t x,uint8_t y, uint8_t ch)
{
	OLED_SetPosition(x,y);
	OLED_Write_Datas((uint8_t*)&ascii_font[ch][0],8);
	OLED_SetPosition(x+1,y);
	OLED_Write_Datas((uint8_t*)&ascii_font[ch][8],8);
}


void OLED_ShowString(uint8_t x,uint8_t y, uint8_t *ch)
{
	while(*ch!='\0')
	{
		OLED_ShowChar(x,y,*ch);
		y+=8;
		if(y>127)
		{
			x+=2;
		}
		if(x>7)
		{
			x = 0;
		}
		ch++;
	}
}

void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t *hanzi)
{
	uint8_t i,j;
	//每行最多显示8个汉字
	for(i=0;i<8;i++)
	{
		OLED_SetPosition(x,y);
		OLED_Write_Datas((hanzi+i*32),16);
		OLED_SetPosition(x+1,y);
		OLED_Write_Datas((hanzi+i*32+16),16);
		y+=16;
		if(y>127)
		{
			x+=2;
		}
		if(x >7)
		{
			x = 0;
		}
	}
}
