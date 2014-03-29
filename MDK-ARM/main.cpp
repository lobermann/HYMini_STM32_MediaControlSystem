
extern "C"
{
	#include "stm32f10x.h"
	#include "TouchPanel.h"
	#include "systick.h"
	#include "GLCD.h"
	
	#include <stm32f10x_i2c.h>
	#include "stm32f10x_rcc.h"
	#include "stm32f10x_usart.h"
}
#include <stdlib.h>
#include <string.h>
#include "Display1.h"

/* Prototypes */
void NVIC_Configuration(void);
void TIM_Configuration(void);
void USART_Configuration(void);

int main()
{
	/*
	Notes:
	Colors: 0xRGBA
	*/
	
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
	
	TIM_Configuration();
	NVIC_Configuration();
	USART_Configuration();

	Display1* dp = new Display1();
	dp->draw();
	
  while (1)	
  {
    getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;
		//Draw a Point where display is touched
		LCD_SetPoint(display.x, display.y, utils::color_conv(0xd0,0xff,0xda));
		
		dp->inject_touch(display.x, display.y);
  }
}

/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configure USART1 
*******************************************************************************/
void USART_Configuration(void)
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

  USART_InitStructure.USART_BaudRate = 115200;
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

extern "C" void DMA2_Channel4_5_IRQHandler()
{
	
}

/*******************************************************************************
* Function Name  : TIM_Configuration
* Description    : TIM_Configuration program.
*******************************************************************************/
void TIM_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7 , ENABLE);
  TIM_DeInit(TIM7);
  TIM_TimeBaseStructure.TIM_Period=2000;

  TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM7, TIM_FLAG_Update);
  TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM7, ENABLE);
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configuration the nested vectored interrupt controller.
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
	 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  													
  NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;	
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
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

