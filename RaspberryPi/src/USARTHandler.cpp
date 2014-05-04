/* 
 * File:   USARTHandler.cpp
 * Author: Lukas
 * 
 * Created on 01. Mai 2014, 01:01
 */

#include "USARTHandler.h"

#include "utils.h"

USARTHandler::USARTHandler()
{ 
    m_uart_fd_ = -1;
    m_rx_buffer_ = new char[129];
}

USARTHandler::~USARTHandler()
{ 
    delete[] m_rx_buffer_;
}

void USARTHandler::init()
{
    m_uart_fd_ = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY); //Open in non blocking read/write mode
    if (m_uart_fd_ == -1)
    {
        printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
    }

    //CONFIGURE THE UART
    //The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
    //	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
    //	CSIZE:- CS5, CS6, CS7, CS8
    //	CLOCAL - Ignore modem status lines
    //	CREAD - Enable receiver
    //	IGNPAR = Ignore characters with parity errors
    //	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
    //	PARENB - Parity enable
    //	PARODD - Odd parity (else even)
    struct termios options;
    tcgetattr(m_uart_fd_, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(m_uart_fd_, TCIFLUSH);
    tcsetattr(m_uart_fd_, TCSANOW, &options);
}

bool USARTHandler::read()
{
    bool received_something = false;
    
    if(m_uart_fd_ != -1)
    {
        memset(m_rx_buffer_, 0, 129);
        int rx_length = ::read(m_uart_fd_, m_rx_buffer_, 128);

        if (rx_length < 0)
        {
            //An error occured (will occur if there are no bytes)
        }
        else if (rx_length == 0)
        {
            //No data waiting
        }
        else
        {
            std::cout << "Usart received " << rx_length << std::endl;
            
            //Write the buffer to the msg buffer for processing
            m_input_buffer_ += m_rx_buffer_;
            
            std::cout << "Received : " << string_to_hex(m_input_buffer_) << std::endl;
            
            bool got_start = false;
            
            //Process every message
            std::string single_msg;
            for (std::string::iterator iter = m_input_buffer_.begin(); iter != m_input_buffer_.end();)
            {
                if (*iter == 0x02) //If start of message
                {
                    single_msg.clear();
                    iter = m_input_buffer_.erase(iter);
                    got_start = true;
                }
                else if (*iter == 0x03) //If end of message
                {
                    received_something = true;

                    //Process the message
                    if (single_msg.at(0) == 'T')
                    {
                        //Touch event
                        if(single_msg.at(1) == (char)11)
                        {
                            
                        }
                        else if(single_msg.at(1) == (char)12)
                        {
                            
                        }
                        else if(single_msg.at(1) == (char)13)
                        {
                            
                        }
                        else if(single_msg.at(1) == (char)14)
                        {
                            
                        }
                        else if(single_msg.at(1) == (char)15)
                        {
                            
                        }
                        else if(single_msg.at(1) == (char)16)
                        {
                            
                        }
                    }
                    else if (single_msg.at(0) == 'S')
                    {
                        //Startup event
                        std::cout << "Got startup message" << std::endl;

                        send_write_message(m_uart_fd_, 'H', char(0), color_conv(0x00, 0xFF, 0x3C), "Media Control System Ready");

                        send_write_message(m_uart_fd_, 'S', char(11), color_conv(0xFF, 0xFF, 0xFF), "Hitradio OE3");
                        send_write_message(m_uart_fd_, 'S', char(12), color_conv(0xFF, 0xFF, 0xFF), "Kronehit");
                        send_write_message(m_uart_fd_, 'S', char(13), color_conv(0xFF, 0xFF, 0xFF), "FM 4");
                        send_write_message(m_uart_fd_, 'S', char(14), color_conv(0xFF, 0xFF, 0xFF), "Antenne Steiermark");
                        send_write_message(m_uart_fd_, 'S', char(15), color_conv(0xFF, 0xFF, 0xFF), "Pirate Radio");
                        send_write_message(m_uart_fd_, 'S', char(16), color_conv(0xFF, 0xFF, 0xFF), "Soundportal Graz");

                        single_msg.clear();
                    }
                    else if (single_msg.at(0) == 'A')
                    {
                        
                    }
                    else
                    {
                        std::cout << "Unimplemented Message" << std::endl;
                    }
                    got_start = false;
                    single_msg.clear();
                    iter = m_input_buffer_.erase(iter);
                }
                else
                {
                    if(got_start)
                    {
                        single_msg += *iter;
                    }
                    iter = m_input_buffer_.erase(iter);
                }
            }
        }
    }
    
    return received_something;
}

void USARTHandler::send_write_message(int fd, char type, char slot, uint16_t color, std::string text)
{
    std::string write_header; //Write Message
    write_header += (char)0x00;
    write_header += "W";
    write_header += type;
    write_header += slot;
    write_header += (color >> 8) & 0xFF;
    write_header += color & 0xFF; //In that color
    write_header += (char)text.size()+1; //Text length, including checksum
    write_header += text; //This text
    
    //Create simple checksum
    char checksum = (char)0x00;
    std::string::iterator iter = write_header.begin();
    iter++; //Skip the 0x00
    for(;iter != write_header.end(); ++iter)
    {
        checksum = checksum ^ *iter;
    }
    
    write_header += checksum;
    
    //TODO: should not send messages greater then 128
   
    std::cout << "Writing: " << string_to_hex(write_header) << std::endl;
    std::cout << "Writing: ";
    for (std::string::iterator iter = write_header.begin(); iter != write_header.end(); ++iter)
    {
        std::cout << " " << *iter << " ";
    }
    std::cout << std::endl;
    
    int tx_len = write(fd, write_header.c_str(), write_header.size());
    
    //TODO: Send remaining data if some is left
    
    std::cout << write_header.size() << " : " << tx_len << std::endl;
    
    usleep(10000);
}
