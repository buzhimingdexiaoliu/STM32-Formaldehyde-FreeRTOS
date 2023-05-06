# ifndef __BSP_OLED_H
# define __BSP_OLED_H

# define Write_Addr 0x78
# define Read_Addr  0x79
# define AddrMode       2
# define NormalMode
# define EntireDisplay_Off
# define Display_On
# define ReMap            //重映射
# define ScanMode_Remapper     //正常扫描
# define ChargePump_Enable   //使能电荷碰撞


typedef enum OLED_Mode
{
	
	H_Addr_Mode = 0,       //水平地址模式
	V_Addr_Mode = 1,					//垂直地址模#式
	Page_Addr_Mode = 2,     //页地址模式
}OLED_Mode;

typedef enum isSequential
{
	Sequential = 0,
	Alternative,
}isSequential;

typedef enum isRemapper
{
	NoRemapper = 0,
	Remapper   = 1,
}isRemapper;

typedef enum Level
{
	Low_Level = 0,
	Mid_Level = 2,
	High_Level= 3,
}Level;


void OLED_Write_Cmd(unsigned char cmd);
void OLED_Write_Data(unsigned char data);
void OLED_Write_Datas(unsigned char* buf,unsigned char length);
void OLED_Set_Contrast(unsigned char data);
void OLED_EntireDisplay();
void OLED_SetDisplayMode();
void OLED_SetDisplay();
void OLED_SetCol_Start(unsigned char addr);
void OLED_SetMode(OLED_Mode mode);
void OLED_Set_ColAddr(unsigned char start,unsigned char end);
void OLED_SetPageAddr(unsigned char start,unsigned char end);
void OLED_SetPage_Start(unsigned char addr);
void OLED_SetPosition(unsigned char page,unsigned char col);
void OLED_SetStartLine(unsigned char startLine);
void OLED_SetReMap();
void OLED_SetMUXRatio(unsigned char data);
void OLED_SetScanMode();
void OLED_SetDisplayOffset(unsigned char offset);
void OLED_SetCOMPins(isSequential mode1, isRemapper mode2);
void OLED_SetClockFrequency(unsigned char clock,unsigned char freq);
void OLED_Set_Period(unsigned char per1,unsigned char per2);
void OLED_SetVcomLevel(Level level);
void OLED_SetChargePump();
void OLED_Init();
void OLED_Clear(void);
void OLED_ShowChar(unsigned char x,unsigned char y, unsigned char ch);
void OLED_ShowString(unsigned char x,unsigned char y, unsigned char *ch);
void OLED_ShowChinese(unsigned char x,unsigned char y,unsigned char *hanzi);
# endif