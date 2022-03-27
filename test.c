#include "stdlib.h"
#include "stdio.h"
#include "oled.h"
#include "gui.h"
#include "test.h"
#include "delay.h"


/*****************************************************************************
 * @name       :void TEST_MainPage(void)
 * @date       :2018-08-27 
 * @function   :Drawing the main Interface of the Comprehensive Test Program
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TEST_MainPage(void)
{	
	GUI_ShowString(28,0,"OLED TEST",16,1);
	GUI_ShowString(12,2,"0.96\" SSD1306",16,1);
	GUI_ShowString(40,4,"64X128",16,1);
	GUI_ShowString(4,6,"www.lcdwiki.com",16,1);
	Delay800ms();		
	Delay800ms();
}

/*****************************************************************************
 * @name       :void Test_Color(void)
 * @date       :2018-08-27 
 * @function   :Color fill test(white,black)
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Color(void)
{
	 GUI_Fill(0,0,WIDTH-1,7,0);
//	 GUI_ShowString(10,1,"BLACK",16,1);
	 Delay800ms();
	 GUI_Fill(0,0,WIDTH-1,7,1);
	 Delay800ms();
}

/*****************************************************************************
 * @name       :void TEST_English(void)
 * @date       :2018-08-27 
 * @function   :English display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
//void TEST_English(void)
//{
//	GUI_ShowString(0,0,"6x8:abcdefghijklmnopqrstuvwxyz",8,1);
//	GUI_ShowString(0,2,"8x16:abcdefghijklmnopqrstuvwxyz",16,1);
//	ms_delay(1000);
//	GUI_ShowString(0,0,"6x8:ABCDEFGHIJKLMNOPQRSTUVWXYZ",8,1);
//	GUI_ShowString(0,2,"8x16:ABCDEFGHIJKLMNOPQRSTUVWXYZ",16,1);
//	ms_delay(1500);
//}

/*****************************************************************************
 * @name       :void TEST_Number_Character(void) 
 * @date       :2018-08-27 
 * @function   :number and character display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
//void TEST_Number_Character(void) 
//{
//	GUI_Fill(0,0,WIDTH-1,3,0);
//	GUI_ShowString(0,0,"6x8:!\"#$%&'()*+,-./:;<=>?@[]\\^_`~{}|",8,1);
//	GUI_ShowNum(30,2,1234567890,10,8,1);
//	ms_delay(1000);
//	OLED_Clear(0); 
//  GUI_ShowString(0,0,"8x16:!\"#$%&'()*+,-./:;<=>?@[]\\^_`~{}|",16,1);
//	GUI_ShowNum(40,4,1234567890,10,16,1);
//	ms_delay(1500);
//	OLED_Clear(0);
//}

/*****************************************************************************
 * @name       :void TEST_Chinese(void)
 * @date       :2018-08-27
 * @function   :chinese display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
//void TEST_Chinese(void)
//{	
//	GUI_ShowString(45,0,"16x16",8,1);
//	GUI_ShowCHinese(16,2,16,"全动电子技术",1);
//	ms_delay(1000);
//	OLED_Clear(0);
//	GUI_ShowString(45,0,"24x24",8,1);
//	GUI_ShowCHinese(16,2,24,"全动电子",1);
//	ms_delay(1000);
//	OLED_Clear(0);
//	GUI_ShowString(45,0,"32x32",8,1);
//	GUI_ShowCHinese(0,2,32,"全动电子",1);	
//  ms_delay(1000);
//	OLED_Clear(0);
//}

/*****************************************************************************
 * @name       :void TEST_BMP(void)
 * @date       :2018-08-27 
 * @function   :BMP monochromatic picture display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
//void TEST_BMP(void)
//{
//	GUI_DrawBMP(0,0,128,64, BMP2, 1);
//	ms_delay(1000);
//}
