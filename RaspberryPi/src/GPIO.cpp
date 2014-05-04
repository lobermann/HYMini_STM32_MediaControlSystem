/* 
 * File:   GPIO.cpp
 * Author: Lukas
 * 
 * Created on 19. April 2014, 00:09
 */

#include "GPIO.h"

#include <fstream>
#include <iostream>
#include <sstream>

GPIO::GPIO()
{
    this->gpionum = "4"; //GPIO4 is default
}

GPIO::GPIO(std::string gnum)
{
    this->gpionum = gnum; //Instatiate GPIOClass object for GPIO pin number "gnum"
}

int GPIO::export_gpio()
{
    std::string export_str = "/sys/class/gpio/export";
    std::ofstream exportgpio(export_str.c_str()); // Open "export" file. Convert C++ string to C string. Required for all Linux pathnames
    if (exportgpio < 0)
    {
        std::cout << " OPERATION FAILED: Unable to export GPIO" << this->gpionum << " ." << std::endl;
        return -1;
    }

    exportgpio << this->gpionum; //write GPIO number to export
    exportgpio.close(); //close export file
    return 0;
}

int GPIO::unexport_gpio()
{
    std::string unexport_str = "/sys/class/gpio/unexport";
    std::ofstream unexportgpio(unexport_str.c_str()); //Open unexport file
    if (unexportgpio < 0)
    {
        std::cout << " OPERATION FAILED: Unable to unexport GPIO" << this->gpionum << " ." << std::endl;
        return -1;
    }

    unexportgpio << this->gpionum; //write GPIO number to unexport
    unexportgpio.close(); //close unexport file
    return 0;
}

int GPIO::setdir_gpio(std::string dir)
{

    std::string setdir_str = "/sys/class/gpio/gpio" + this->gpionum + "/direction";
    std::ofstream setdirgpio(setdir_str.c_str()); // open direction file for gpio
    if (setdirgpio < 0)
    {
        std::cout << " OPERATION FAILED: Unable to set direction of GPIO" << this->gpionum << " ." << std::endl;
        return -1;
    }

    setdirgpio << dir; //write direction to direction file
    setdirgpio.close(); // close direction file
    return 0;
}

int GPIO::setval_gpio(std::string val)
{

    std::string setval_str = "/sys/class/gpio/gpio" + this->gpionum + "/value";
    std::ofstream setvalgpio(setval_str.c_str()); // open value file for gpio
    if (setvalgpio < 0)
    {
        std::cout << " OPERATION FAILED: Unable to set the value of GPIO" << this->gpionum << " ." << std::endl;
        return -1;
    }

    setvalgpio << val; //write value to value file
    setvalgpio.close(); // close value file 
    return 0;
}

int GPIO::getval_gpio(std::string& val)
{

    std::string getval_str = "/sys/class/gpio/gpio" + this->gpionum + "/value";
    std::ifstream getvalgpio(getval_str.c_str()); // open value file for gpio
    if (getvalgpio < 0)
    {
        std::cout << " OPERATION FAILED: Unable to get value of GPIO" << this->gpionum << " ." << std::endl;
        return -1;
    }

    getvalgpio >> val; //read gpio value

    if (val != "0")
        val = "1";
    else
        val = "0";

    getvalgpio.close(); //close the value file
    return 0;
}

bool GPIO::getval_gpio()
{
    std::string val;
    std::string getval_str = "/sys/class/gpio/gpio" + this->gpionum + "/value";
    std::ifstream getvalgpio(getval_str.c_str()); // open value file for gpio
    if (getvalgpio < 0)
    {
        std::cout << " OPERATION FAILED: Unable to get value of GPIO" << this->gpionum << " ." << std::endl;
        return -1;
    }

    getvalgpio >> val; //read gpio value
    getvalgpio.close(); //close the value file

    if (val != "0")
        return true;
    else
        return false;
}

std::string GPIO::get_gpionum()
{
    return this->gpionum;
}