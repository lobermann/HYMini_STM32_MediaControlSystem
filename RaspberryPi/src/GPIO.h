/* 
 * File:   GPIO.h
 * Author: Lukas Obermann
 *
 * Based on: http://hertaville.com/2012/11/18/introduction-to-accessing-the-raspberry-pis-gpio-in-c/
 * 
 * Created on 19. April 2014, 00:09
 */

#ifndef GPIO_H
#define	GPIO_H

#include <string>

class GPIO 
{
public:
    GPIO(); // create a GPIO object that controls GPIO4 (default
    GPIO(std::string x); // create a GPIO object that controls GPIOx, where x is passed to this constructor
    int export_gpio(); // exports GPIO
    int unexport_gpio(); // unexport GPIO
    int setdir_gpio(std::string dir); // Set GPIO Direction
    int setval_gpio(std::string val); // Set GPIO Value (putput pins)
    int getval_gpio(std::string& val); // Get GPIO Value (input/ output pins)
    bool getval_gpio();
    std::string get_gpionum(); // return the GPIO number associated with the instance of an object
private:
    std::string gpionum; // GPIO number associated with the instance of an object
};

#endif	/* GPIO_H */

