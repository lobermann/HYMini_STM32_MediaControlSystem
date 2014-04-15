
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
#include "MyUtils.h"

char* usart_buffer;
char* ready_buffer;
uint8_t usart_write_pos = 0;
bool data_ready = false;

uint8_t debug = 0;

//Interrupt Handler for the USART
extern "C" void USART1_IRQHandler()
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        char c = USART1->DR;
        if(c == 2)
        {
            memset(usart_buffer, 0, 129);
            usart_write_pos = 0;
        }
        else if(c == 3)
        {
            debug++;
            memcpy(ready_buffer, usart_buffer, 128);
            data_ready = true;
            if(debug == 2)
            {
                debug = 2;
            }
            memset(usart_buffer, 0, 129);
            usart_write_pos = 0;
        }
        else
        {
            usart_buffer[usart_write_pos++] = c;
            if(usart_write_pos > 128)
            {
                usart_write_pos = 0;
                memset(usart_buffer, 0, 129);
            }
        }
    }
    
    USART_ClearITPendingBit(USART1, USART_IT_RXNE); 
    //USART_ClearITPendingBit(USART1, USART_IT_TXE); 
}

int main()
{
    usart_buffer = new char[129];
    ready_buffer = new char[129];
    memset(usart_buffer, 0, 129);
    memset(ready_buffer, 0, 129);
    
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

    //usart_handler->TIM_Configuration();
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

        if(data_ready)
        {
            data_ready = false;
            usart_handler->setBuffer(ready_buffer);
            memset(ready_buffer, 0, 129);
            if(debug == 2)
            {
                debug = 2;
            }
            usart_handler->handle();
            usart_handler->notify_receive_ready();
        }

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
