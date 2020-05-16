/*
 * CCS811.h
 *
 *  Created on: Nov 29, 2018
 *      Author: jains
 */

#ifndef CCS811_H_
#define CCS811_H_

#ifndef LIB_ADAFRUIT_CCS811_H
#define LIB_ADAFRUIT_CCS811_H

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "em_i2c.h"

uint16_t buffs[4];
uint8_t buff_8[4];

//#if (ARDUINO >= 100)
// #include "Arduino.h"
//#else
// #include "WProgram.h"
//#endif

//#include <Wire.h>

/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
    #define CCS811_ADDRESS                (0x5A)

#define WRITE 0x00
#define READ 0x01
/*=========================================================================*/

/*=========================================================================
    REGISTERS
    -----------------------------------------------------------------------*/
enum
{
    CCS811_STATUS = 0x00,
    CCS811_MEAS_MODE = 0x01,
    CCS811_ALG_RESULT_DATA = 0x02,
    CCS811_RAW_DATA = 0x03,
    CCS811_ENV_DATA = 0x05,
    CCS811_NTC = 0x06,
    CCS811_THRESHOLDS = 0x10,
    CCS811_BASELINE = 0x11,
    CCS811_HW_ID = 0x20,
    CCS811_HW_VERSION = 0x21,
    CCS811_FW_BOOT_VERSION = 0x23,
    CCS811_FW_APP_VERSION = 0x24,
    CCS811_ERROR_ID = 0xE0,
    CCS811_SW_RESET = 0xFF,
};

//bootloader registers
enum
{
	CCS811_BOOTLOADER_APP_ERASE = 0xF1,
	CCS811_BOOTLOADER_APP_DATA = 0xF2,
	CCS811_BOOTLOADER_APP_VERIFY = 0xF3,
	CCS811_BOOTLOADER_APP_START = 0xF4
};

enum
{
	CCS811_DRIVE_MODE_IDLE = 0x00,
	CCS811_DRIVE_MODE_1SEC = 0x01,
	CCS811_DRIVE_MODE_10SEC = 0x02,
	CCS811_DRIVE_MODE_60SEC = 0x03,
	CCS811_DRIVE_MODE_250MS = 0x04,
};

uint8_t ERROR;
uint8_t DATA_READY;
uint8_t APP_VALID;
uint8_t FW_MODE;

uint8_t INT_DATARDY;
uint8_t DRIVE_MODE;
uint8_t INT_THRESH;

uint8_t WRITE_REG_INVALID;
uint8_t READ_REG_INVALID;
uint8_t MEASMODE_INVALID;
uint8_t MAX_RESISTANCE;
uint8_t HEATER_FAULT;
uint8_t HEATER_SUPPLY;

uint8_t _i2caddr;
float _tempOffset;

uint16_t _TVOC;
uint16_t _eCO2;


/*=========================================================================*/

#define CCS811_HW_ID_CODE			0x81

#define CCS811_REF_RESISTOR			100000

/**************************************************************************/
/*!
    @brief  Class that stores state and functions for interacting with CCS811 gas sensor chips
*/
/**************************************************************************/

int8_t begin(void);

void setEnvironmentalData(uint8_t humidity, double temperature);

//calculate temperature based on the NTC register
double calculateTemperature();

void setThresholds(uint16_t low_med, uint16_t med_high, uint8_t hysteresis);

void SWReset();

void setDriveMode(uint8_t mode);
void enableInterrupt();
void disableInterrupt();

/**************************************************************************/
/*!
 @brief  returns the stored total volatile organic compounds measurement. This does does not read the sensor. To do so, call readData()
 @returns TVOC measurement as 16 bit integer
 */
/**************************************************************************/
//uint16_t getTVOC() { return _TVOC; }
/**************************************************************************/
/*!
 @brief  returns the stored estimated carbon dioxide measurement. This does does not read the sensor. To do so, call readData()
 @returns eCO2 measurement as 16 bit integer
*/
/**************************************************************************/

//uint16_t geteCO2() { return _eCO2; }
/**************************************************************************/
        /*!
            @brief  set the temperature compensation offset for the device. This is needed to offset errors in NTC measurements.
            @param offset the offset to be added to temperature measurements.
        */
        /**************************************************************************/
void setTempOffset(float offset);

//check if data is available to be read
int8_t available();
uint8_t readData();

int8_t checkError();

void write8(uint8_t reg, uint8_t value);
void write16(uint8_t reg, uint16_t value);
uint8_t read8(uint8_t reg);

uint8_t read(uint8_t reg/*, uint8_t *buf, uint8_t num*/);
uint16_t read16(uint8_t reg);
void read_alg_result(uint8_t reg);
void write(uint8_t reg,/* uint8_t *buf,*/ uint8_t num);
void write_boot();
void _i2c_init();

/*=========================================================================
	REGISTER BITFIELDS
    -----------------------------------------------------------------------*/
		// The status register
//        struct status {
//
//            /* 0: no error
//            *  1: error has occurred
//            */
//            uint8_t ERROR;
//
//            // reserved : 2
//
//            /* 0: no samples are ready
//            *  1: samples are ready
//            */
//            uint8_t DATA_READY;
//            uint8_t APP_VALID;
//
//			// reserved : 2
//
//            /* 0: boot mode, new firmware can be loaded
//            *  1: application mode, can take measurements
//            */
//            uint8_t FW_MODE;

void set_status(uint8_t data);

        //measurement and conditions register
//        struct meas_mode {
//        	// reserved : 2
//
//        	/* 0: interrupt mode operates normally
//            *  1: Interrupt mode (if enabled) only asserts the nINT signal (driven low) if the new
//				ALG_RESULT_DATA crosses one of the thresholds set in the THRESHOLDS register
//				by more than the hysteresis value (also in the THRESHOLDS register)
//            */
//        	uint8_t INT_THRESH;
//
//        	/* 0: int disabled
//            *  1: The nINT signal is asserted (driven low) when a new sample is ready in
//				ALG_RESULT_DATA. The nINT signal will stop being driven low when
//				ALG_RESULT_DATA is read on the I²C interface.
//            */
//        	uint8_t INT_DATARDY;
//
//        	uint8_t DRIVE_MODE;

uint8_t get(void);

//        struct error_id {
//        	/* The CCS811 received an I²C write request addressed to this station but with
//			invalid register address ID */
//        	uint8_t WRITE_REG_INVALID;
//
//        	/* The CCS811 received an I²C read request to a mailbox ID that is invalid */
//        	uint8_t READ_REG_INVALID;
//
//        	/* The CCS811 received an I²C request to write an unsupported mode to
//			MEAS_MODE */
//        	uint8_t MEASMODE_INVALID;
//
//        	/* The sensor resistance measurement has reached or exceeded the maximum
//			range */
//        	uint8_t MAX_RESISTANCE;
//
//        	/* The Heater current in the CCS811 is not in range */
//        	uint8_t HEATER_FAULT;
//
//        	/*  The Heater voltage is not being applied correctly */
//        	uint8_t HEATER_SUPPLY;

void set_error_id(uint8_t data);
/*=========================================================================*/

#endif

#endif /* CCS811_H_ */
