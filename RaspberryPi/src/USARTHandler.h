/* 
 * File:   USARTHandler.h
 * Author: Lukas
 *
 * Created on 01. Mai 2014, 01:01
 */

#ifndef USARTHANDLER_H
#define	USARTHANDLER_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <list>
#include <string>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <fstream>

class USARTHandler 
{
public:
    USARTHandler();
    virtual ~USARTHandler();
    
    void init();
    bool read();
    
    void send_write_message(int fd, char type, char slot, uint16_t color, std::string text);
private:
    int m_uart_fd_;
    char* m_rx_buffer_;
    std::string m_input_buffer_;
    
    std::list<std::string> m_output_buffer_;
};

#endif	/* USARTHANDLER_H */

