#ifndef __GUI_H__
#define __GUI_H__

#include "sys.h"

void GUI_Fill(u8 sx,u8 sy,u8 ex,u8 ey,u8 color);
void GUI_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size,u8 mode);
void GUI_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 Size,u8 mode);
void GUI_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size,u8 mode);
void GUI_ShowFont16(u8 x,u8 y,u8 *s,u8 mode);
void GUI_ShowFont24(u8 x,u8 y,u8 *s,u8 mode);
void GUI_ShowFont32(u8 x,u8 y,u8 *s,u8 mode);
void GUI_ShowCHinese(u8 x,u8 y,u8 hsize,u8 *str,u8 mode);
void GUI_DrawBMP(u8 x,u8 y,u8 width, u8 height, u8 BMP[], u8 mode); 
#endif

