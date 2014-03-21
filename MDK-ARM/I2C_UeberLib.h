/*
 * I2C_UeberLib.h
 *
 *  Created on: Feb 21, 2012
 *      Author: gerobe
 */
/***************************************************************************
 **     I2C_UEBER-LIB
 **
 **     Provides a simple I2C function library for experimenation with
 **		numerous I2C devices
 **
 **     Tested with ATOLLIC C/C++ Compiler Version 2.2.1
 **
 **		Based and depends on ST Peripheral LIB V3.5.0
 **
 ***************************************************************************/

#ifndef __STDAM_I2C_H
#define __STDAM_I2C_H


extern void str_out(char *p);

#define ACK 1
#define NO_ACK 0

#define I2C_DIR_TX 1
#define I2C_DIR_RX 0

#define I2C_Speed              400000

/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define sEE_FLAG_TIMEOUT         ((uint32_t)0x4000)
#define sEE_LONG_TIMEOUT         ((uint32_t)(100 * sEE_FLAG_TIMEOUT))

/* Maximum number of trials for sEE_WaitEepromStandbyState() function */
#define sEE_MAX_TRIALS_NUMBER     500

#define sEE_OK                    0
#define sEE_FAIL                  1


void I2C1_I2C2_BUS_Init(void);

u8 i2c_start(I2C_TypeDef* I2Cx);
u8 i2c_write(I2C_TypeDef* I2Cx, u8 data);
void i2c_stop(I2C_TypeDef* I2Cx);
u8 i2c_read(I2C_TypeDef* I2Cx, u8 ack_mode);
u8 i2c_write7bitAddress(I2C_TypeDef* I2Cx, u8 data, u8 mode);

void I2C_Configuration(I2C_TypeDef* I2Cx);


#endif
