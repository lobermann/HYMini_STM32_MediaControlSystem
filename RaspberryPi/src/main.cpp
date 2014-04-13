#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <string.h>
#include <stdint.h>

uint16_t color_conv(char r, char g, char b)
{
	return ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3);
};

int send_write_message(int fd, char type, char slot, uint16_t color, std::string text)
{
	const char stx = 0x02;
	const char etx = 0x03;

	std::string write_header("W"); //Write Message
	write_header += type;
    write_header += slot;	
	write_header += (color >> 8) & 0xFF;
	write_header += color & 0xFF; //In that color
	write_header += text; //This text
	write_header = stx + write_header + etx; //Wrap within stx and etx
	std::cout << "Writing: " << write_header << std::endl;
	int tx_len = write(fd, write_header.c_str(), write_header.size());
	std::cout << write_header.size() << " : " << tx_len << std::endl;
	usleep(10000);
	return tx_len;
}

int main()
{
    int uart0_filestream = -1;
    uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
    if (uart0_filestream == -1)
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
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    //fcntl(uart0_filestream, F_SETFL, 0);

    std::cout << "Waiting for Data..." << std::endl;
    
    char* rx_buffer = new char[129];
    std::string msg_buffer("");

	const char stx = 0x02;
	const char etx = 0x03;

    while(true)
    {
	  	if (uart0_filestream != -1)
	  	{
			memset(rx_buffer, 0, 129);
	  		int rx_length = read(uart0_filestream, rx_buffer, 128);

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
				//Write the buffer to the msg buffer for processing
				msg_buffer += rx_buffer;

				//Process every message
				std::string single_msg;
				for(std::string::iterator iter = msg_buffer.begin(); iter != msg_buffer.end(); ++iter)
				{
					if(*iter == 0x02) //If start of message
					{
						single_msg.clear();
					}
					else if(*iter == 0x03) //If end of message
					{
						std::cout << "Got message: " << single_msg << std::endl;

						//Process the message
						if(single_msg.at(0) == 'T')
						{
							//Touch event
						}
						else if(single_msg.at(0) == 'S')
						{
							//Startup event
							std::cout << "Got startup message" << std::endl;
							
							send_write_message(uart0_filestream, 'H', char(0), color_conv(0x00, 0xFF, 0x3C), "Media Control System Ready");

							send_write_message(uart0_filestream, 'S', char(1), color_conv(0xFF, 0xFF, 0xFF), "Hitradio OE3");
							send_write_message(uart0_filestream, 'S', char(2), color_conv(0xFF, 0xFF, 0xFF), "Kronehit");
							send_write_message(uart0_filestream, 'S', char(3), color_conv(0xFF, 0xFF, 0xFF), "FM 4");
							send_write_message(uart0_filestream, 'S', char(4), color_conv(0xFF, 0xFF, 0xFF), "Antenne Steiermark");
							send_write_message(uart0_filestream, 'S', char(5), color_conv(0xFF, 0xFF, 0xFF), "Pirate Radio");
							send_write_message(uart0_filestream, 'S', char(6), color_conv(0xFF, 0xFF, 0xFF), "EVE Radio");

							single_msg.clear();
						}
						else
						{
							std::cout << "Unimplemented Message" << std::endl;
						}
					}
					else
					{
						single_msg += *iter;
					}
				}
				msg_buffer.clear();
	  		}
	  	}
		usleep(5000);
	}

	return 0;
}

