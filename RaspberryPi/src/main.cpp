#include <iostream>
#include <unistd.h>
#include <string>
#include <string.h>
#include <stdint.h>
#include <list>
#include <ctime>
#include <signal.h>

#include "GPIO.h"
#include "USARTHandler.h"

int main()
{
    //GPIO 17 for powering the STM
    GPIO gpio17("17");
    gpio17.export_gpio();
    gpio17.setdir_gpio("out");
    
    //GPIO 22 for the button
    GPIO gpio22("22");
    gpio22.export_gpio();
    gpio22.setdir_gpio("in");
    
    //Init the USART Handler
    USARTHandler usart;
    usart.init();
    
    //Turn on the STM
    gpio17.setval_gpio("0");
    std::time_t stm_on_time = std::time(0);
    bool stm_on_status = true;
    
    //Ignore the child signals, so that they are removed by the kernel after finished
    signal(SIGCHLD, SIG_IGN);

    while (true)
    {
        //Turn off the display after 30 seconds idle time
        if( stm_on_status && (std::time(0) - stm_on_time) > 30)
        {
            gpio17.setval_gpio("1");
            stm_on_status = false;
        }
        
        if(usart.read())
        {
            //If we received something, reset the display timer
            if(stm_on_status)
                stm_on_time = std::time(0);
        }
        
        if(!stm_on_status && !gpio22.getval_gpio())
        {
            //Button was pressed, turn on the display
            gpio17.setval_gpio("0");
            stm_on_time = std::time(0);
            stm_on_status = true;
        }
        
        usleep(10000); //Sleep 10 milliseconds
    }

    return 0;
}

