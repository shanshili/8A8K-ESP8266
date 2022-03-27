#ifndef __OLED_H
#define __OLED_H			  	 
#include "8a8k.h"
#include "sys.h"
 
//--------------OLED参数定义---------------------
#define PAGE_SIZE    8
#define XLevelL		   0x00
#define XLevelH		   0x10
#define YLevel       0xB0
#define	Brightness	 0xFF 
#define WIDTH 	     128
#define HEIGHT 	     64	


//-------------写命令和数据定义-------------------
#define OLED_CMD     0	//写命令
#define OLED_DATA    1	//写数据 
   						  
//-----------------OLED端口定义----------------
sbit OLED_CS = P1^3;    //片选信号           P13
sbit OLED_DC = P1^2;   //数据/命令控制信号  P12
sbit OLED_RST = P3^3;   //复位信号          P33

//-----------------OLED端口操作定义---------------- 
#define OLED_CS_Clr()  OLED_CS = 0
#define OLED_CS_Set()  OLED_CS = 1

#define OLED_DC_Clr()  OLED_DC = 0
#define OLED_DC_Set()  OLED_DC = 1
 					   
#define OLED_RST_Clr()  OLED_RST = 0
#define OLED_RST_Set()  OLED_RST = 1

//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);     							   		    
void OLED_Set_Pos(u8 x, u8 y);
void OLED_Reset(void);	   							   		    
void OLED_Init(void);
void OLED_Set_Pixel(u8 x, u8 y,u8 color);
void OLED_Display(void);
void OLED_Clear(unsigned dat);  

#endif