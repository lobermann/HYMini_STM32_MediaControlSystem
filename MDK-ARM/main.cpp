
extern "C"
{
	#include "stm32f10x.h"
	#include "TouchPanel.h"
	#include "systick.h"
	#include "GLCD.h"
	
	#include <stm32f10x_i2c.h>
	#include "stm32f10x_rcc.h"
	#include "stm32f10x_usart.h"
}
#include <stdlib.h>
#include <string.h>
#include "Display1.h"

int main()
{
	/*
	Notes:
	Colors: 0xRGBA
	*/
	
	SystemInit();		
	LCD_Initializtion();
	delay_init();
	TP_Init();
	//TouchPanel_Calibrate();
	
	Coordinate a_screen[3];
	a_screen[0].x = 0x0267;
	a_screen[1].x = 0x0c99;
	a_screen[2].x = 0x08fa;
	
	a_screen[0].y = 0x0351;
	a_screen[1].y = 0x036d;
	a_screen[2].y = 0x0bde;
	
	//Manually callibrate the touchpanel, maybe store and get this from the flash
	setCalibrationMatrix(&DisplaySample[0], &a_screen[0], &matrix);

	LCD_Clear(0x000000);
	
	Display1* dp = new Display1();

	dp->draw();

	uint8_t write = 0;
	//bool toggle = false;
  while (1)	
  {
    getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;
		//Draw a Point where display is touched
		LCD_SetPoint(display.x, display.y, utils::color_conv(0xd0,0xff,0xda));
		dp->inject_touch(display.x, display.y);
  }
}


#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

