#include "USARTHandler.h"

#include "MyUtils.h"

USARTHandler::USARTHandler()
{
    input_buffer = new char[129];
    single_buffer = new char;
    buffer_write_pos = 0;
    memset(input_buffer, 0, 129);
    m_display_ = 0;
}

USARTHandler::~USARTHandler()
{
    delete[] input_buffer;
    delete single_buffer;
}

void USARTHandler::handle()
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
    {
        *single_buffer = (char)USART_ReceiveData(USART1);
        if(*single_buffer == 0x02)
        {
            //Message start, ignore but reset buffer
            *single_buffer = 0;
            memset(input_buffer, 0, 128);
            buffer_write_pos = 0;
        }
        else if(*single_buffer == 0x03)
        {
            if(strlen(input_buffer) > 1)
            {
                //Decode Message and forward to Display
                if(input_buffer[0] == 'W')
                {
                    //A write message
                    // Position:
                    // 0 = W - Write Message
                    // 1 = H / S - Write to Header or Slot
                    // 2 = Slot Number or 0
                    // 3+4 = color
                    // 5+128 = text
                    
                    //The the type
                    char type = input_buffer[1];
                    
                    //Get the slot
                    uint8_t slot = input_buffer[2];
                    
                    //Get the color that should be used
                    uint16_t color = (input_buffer[3] << 8) | input_buffer[4];
                    
                    //Compact the buffer, so only the message is left
                    input_buffer[123] = 0;
                    for(uint8_t i = 0; i < 123; i++) //124 as the first 5 bytes are meta data
                    {
                        input_buffer[i] = input_buffer[i+5];
                    }
                    
                    if(type == 'H') //Write to Header
                    {
                        if(m_display_ != 0)
                            m_display_->write_header(color, input_buffer);
                    }
                    if(type == 'S') //Write to Slot
                    {
                        if(m_display_ != 0)
                            m_display_->write_slot(slot, color, input_buffer);
                    }
                }
            }
            *single_buffer = 0;
            memset(input_buffer, 0, 128);
            buffer_write_pos = 0;
        }
        else
        {
            input_buffer[buffer_write_pos] = *single_buffer;
            *single_buffer = 0;
            buffer_write_pos++;
            if(buffer_write_pos > 127) buffer_write_pos = 0;
        }
    }
}

void USARTHandler::notify_ready()
{
    char buffer = 0x02;
    utils::USARTWrite(buffer);
    buffer = 'S';
    utils::USARTWrite(buffer);
    buffer = 0x03;
    utils::USARTWrite(buffer);
}

void USARTHandler::setDisplay(MyDisplay* display)
{
    m_display_ = display;
}

/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configure USART1 
*******************************************************************************/
void USARTHandler::USART_Configuration(void)
{ 
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure; 

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
    /*
    *  USART1_TX -> PA9 , USART1_RX ->	PA10
    */				
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);		   

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure); 
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

/*******************************************************************************
* Function Name  : TIM_Configuration
* Description    : TIM_Configuration program.
*******************************************************************************/
void USARTHandler::TIM_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=2000;

    TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configuration the nested vectored interrupt controller.
*******************************************************************************/
void USARTHandler::NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
     
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
