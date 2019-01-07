


#include "display.h"

#define band 80

void Start_Display()
{
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_MAGENTA);
	BSP_LCD_FillRect(0,band,BSP_LCD_GetXSize(),BSP_LCD_GetYSize()-band);
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(0,24,(uint8_t *)"PIOTR", LEFT_MODE);
	BSP_LCD_DisplayStringAt(0,24,(uint8_t *)"JAKUB", RIGHT_MODE);
	BSP_LCD_DisplayStringAt(0,0,(uint8_t *)"DSP", CENTER_MODE);
	BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);

}



