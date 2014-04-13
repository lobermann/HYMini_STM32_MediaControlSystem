
extern "C"
{
    #include "stm32f10x.h"
    #include "TouchPanel.h"
    #include "systick.h"
    #include "GLCD.h"
}
#include <stdlib.h>
#include <string.h>
#include "USARTHandler.h"
#include "Display1.h"

//Interrupt Handler for the USART
extern "C" void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        //Seems to get better performance when reading
        //the USART Buffer from the main loop ...
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

int main()
{
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

    USARTHandler* usart_handler = new USARTHandler();

    usart_handler->TIM_Configuration();
    usart_handler->NVIC_Configuration();
    usart_handler->USART_Configuration();

    Display1* dp = new Display1();
    dp->draw();
    usart_handler->setDisplay(dp);
    
    //Notify the Pi that we are ready
    usart_handler->notify_ready();
	
    while (1)	
    {
        getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;

        usart_handler->handle();

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
