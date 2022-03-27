#include "spi.h"
#include "sys.h"

void SPI_WriteByte(u8 byte)
{	
	u8 i;
  for(i=0;i<8;i++)
	{			  
		if(byte&0x80)
		{
		  OLED_MOSI_Set();
		}
		else
		{
		  OLED_MOSI_Clr();
		}
		OLED_CLK_Clr();
		OLED_CLK_Set();
		byte<<=1;   
	}				 		  
}
