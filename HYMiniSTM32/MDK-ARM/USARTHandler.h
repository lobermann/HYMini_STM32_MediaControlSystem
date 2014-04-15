#ifndef USARTHANDLER
#define USARTHANDLER

extern "C"
{
	#include "stm32f10x.h"
	#include "stm32f10x_rcc.h"
	#include "stm32f10x_usart.h"
	#include "stm32f10x_dma.h"
}

#include <stdlib.h>
#include <string.h>
#include "MyDisplay.h"

class USARTHandler
{
public:
	USARTHandler();
	~USARTHandler();

	void NVIC_Configuration(void);
	void TIM_Configuration(void);
	void USART_Configuration(void);

	void handle();
    void notify_ready();
    void notify_receive_ready();
    void setDisplay(MyDisplay* display);
    void setBuffer(char* data);

private:
	char* input_buffer;
	char* single_buffer;
	uint8_t buffer_write_pos;

    MyDisplay* m_display_;

};

#endif 
