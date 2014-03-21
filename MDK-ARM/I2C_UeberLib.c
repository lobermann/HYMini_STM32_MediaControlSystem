/***************************************************************************
 **     I2C_UEBER-LIB
 **
 **     Provides a simple I2C function library for experimentation with
 **		numerous I2C devices
 **
 **     Tested with ATOLLIC C/C++ Compiler Version 2.2.1
 **
 **		Based and depends on ST Peripheral LIB V3.5.0
 **
 ***************************************************************************/


#include "stm32f10x.h"
#include "I2C_UeberLib.h"
#include <stm32f10x_i2c.h>

// Pin Definitions

#define I2C1_SCL_PIN				GPIO_Pin_6
#define I2C1_SDA_PIN				GPIO_Pin_7
#define I2C2_SCL_PIN				GPIO_Pin_10
#define I2C2_SDA_PIN				GPIO_Pin_11
#define ALL_I2C_PINS 				I2C1_SCL_PIN | I2C1_SDA_PIN | I2C2_SCL_PIN | I2C2_SDA_PIN


//*******************************************************************************************

#define I2C_OWN_ADDRESS7     0x10

__IO uint32_t  sEETimeout = sEE_LONG_TIMEOUT;

// Begin of initialisation **********************************************************

/**
 * @brief  Initializes both I2C1 and I2C2 buses
 * @param  None
 * @retval None
 */
void I2C1_I2C2_BUS_Init(void)
{
	u32 delay= 144000;
	GPIO_InitTypeDef  GPIO_InitStructure;


	I2C_DeInit(I2C1);			// Reset all I2C1 Bus Hardware
	I2C_DeInit(I2C2);			// Reset all I2C2 Bus Hardware

	/* I2C Periph clock enable for I2C1 and I2C2*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 | RCC_APB1Periph_I2C2, ENABLE);

	/* GPIO Periph clock enable for I2C1 and I2C2*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* GPIO Periph clock enable for GPIOD*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	/* I2C configuration for I2C1 and I2C2*/
	I2C_Configuration(I2C1);
	I2C_Configuration(I2C2);

	GPIO_DeInit(GPIOB);

	/* GPIO configuration for I2C1 and I2C2*/
	GPIO_InitStructure.GPIO_Pin = ALL_I2C_PINS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// Note: The following is only required for my test hardware to turn on 3.3V power to sensors

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// Turn on switched 3.3V supply to peripheral hardware by setting pin 11 to low
	GPIOD->ODR &= 0xFFFFF7FF;

	// A 200ms wait is required here to allow my switched external hardware to power up before being accessed.
	// This is only required for my hardware - Ignore otherwise unless you experience a bus lockup
	// due to the high speed of the STM32. Leave it in initially until things work.
	while(delay--);


}


/**
 * @brief  I2C Configuration
 * @param  None
 * @retval None
 */
void I2C_Configuration(I2C_TypeDef* I2Cx)
{
	I2C_InitTypeDef  I2C_InitStructure;

	/* I2C configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C_OWN_ADDRESS7;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;

	/* Apply I2C configuration after enabling it */
	I2C_Init(I2Cx, &I2C_InitStructure);

	/* I2C Peripheral Enable */
	I2C_Cmd(I2Cx, ENABLE);
}






/**
 * @brief
 * @param  None
 * @retval None
 */
u32 sEE_TIMEOUT_UserCallback(u8 data)
{
//	static u16 cnt=0;
//	char buffer[32]={'\0'};
//	char message[] = { "\r\nEEPROM Timeout Error at" };
	// Block communication and all processes
//	sprintf(buffer, "\r\n%s %u, %u \r\n", message, data, ++cnt);
//	str_outD(buffer);
	return 1;
}


// End of Initialisation ********************************************************


// Low level I2C functions

//**********************************************************************************************
// i2c_start(I2C_TypeDef* I2Cx)
//**********************************************************************************************
u8 i2c_start(I2C_TypeDef* I2Cx)
{
	I2C_GenerateSTART(I2Cx, ENABLE);
	// Test on EV5 and clear it
	sEETimeout = sEE_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback(10);
	}
	return 0;
}

//**********************************************************************************************
// i2c_write7bitAddress(I2C_TypeDef* I2Cx, u8 data, u8 mode)
//**********************************************************************************************
u8 i2c_write7bitAddress(I2C_TypeDef* I2Cx, u8 data, u8 mode)
{
	u8 status=0;
	u32 event_type_test=0;

	// Send EEPROM address for write
	if(mode == I2C_DIR_TX)
	{
		event_type_test = I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED;
		I2C_Send7bitAddress(I2Cx, data, I2C_Direction_Transmitter);
	}
	else //
	{
		event_type_test = I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED;
		I2C_Send7bitAddress(I2Cx, data, I2C_Direction_Receiver);
	}
	// Test on EV6 and clear it
	sEETimeout = sEE_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2Cx, event_type_test))
	{
		if((sEETimeout--) == 0) // return sEE_TIMEOUT_UserCallback(12);
		{
			return 1; // sEE_TIMEOUT_UserCallback(12);
		}
	}
	return status;
}



//**********************************************************************************************
// i2c_write(I2C_TypeDef* I2Cx, u8 data)
//**********************************************************************************************
u8 i2c_write(I2C_TypeDef* I2Cx, u8 data)
{
	u8 status=0;
	/* Clear EV6 by setting again the PE bit */
	I2C_SendData(I2Cx, data);
	// Test TxE bit in EV8 and clear it
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	return status;
}



//**********************************************************************************************
// i2c_stop(I2C_TypeDef* I2Cx)
//**********************************************************************************************
void i2c_stop(I2C_TypeDef* I2Cx)
{
	/* Send STOP condition */
	I2C_GenerateSTOP(I2Cx, ENABLE);
}



//**********************************************************************************************
// i2c_read(I2C_TypeDef* I2Cx, u8 ack_mode)
//**********************************************************************************************
u8 i2c_read(I2C_TypeDef* I2Cx, u8 ack_mode)
{
	u8 buffer=0;

	/* Test on EV7 and clear it */
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

	// Read I2C byte from slave
	buffer = I2C_ReceiveData(I2Cx);

	// Disable Acknowledgement when ack_mode = NACK
	if(ack_mode == NO_ACK)
		I2C_AcknowledgeConfig(I2Cx, DISABLE);

	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_SLAVE_BYTE_RECEIVED));

	return buffer;
}




