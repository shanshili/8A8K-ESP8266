#ifndef _SPI_H_
#define _SPI_H_
#include "8a8k.h"
#include "sys.h"


//本测试程序使用的是软件模拟SPI接口驱动
//SPI的数据引脚定义和时钟引脚定义都可以任意修改
sbit OLED_MOSI = P1^5;   //OLED屏SPI写数据引脚 D1
sbit OLED_CLK = P1^7;    //OLED屏SPI时钟引脚   D0

#define OLED_MOSI_Clr()  OLED_MOSI = 0
#define OLED_MOSI_Set()  OLED_MOSI = 1 

#define OLED_CLK_Clr()   OLED_CLK = 0
#define OLED_CLK_Set()   OLED_CLK = 1

//-------------spi操作函数-----------------/
void SPI_WriteByte(u8 byte);

#endif
