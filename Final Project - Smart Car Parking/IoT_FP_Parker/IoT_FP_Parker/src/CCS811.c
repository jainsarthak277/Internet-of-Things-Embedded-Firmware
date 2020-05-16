/*
 * CCS811.c
 *
 *  Created on: Nov 29, 2018
 *      Author: jains
 */
#include "CCS811.h"

/**************************************************************************/
/*!
    @brief  Setups the I2C interface and hardware and checks for communication.
    @param  addr Optional I2C address the sensor can be found on. Default is 0x5A
    @returns True if device is set up, false on any failure
*/
/**************************************************************************/
int8_t begin()
{
	_i2caddr = CCS811_ADDRESS;

	_i2c_init();

//	SWReset();

	//check that the HW id is correct
	if(read8(CCS811_HW_ID) != CCS811_HW_ID_CODE)
		return 0;

	//try to start the app
//	write(CCS811_BOOTLOADER_APP_START, NULL, 0);
//	delay(100);

	//make sure there are no errors and we have entered application mode
	if(checkError()) return 0;
	if(!FW_MODE) return 0;

	disableInterrupt();

	//default to read every second
	setDriveMode(CCS811_DRIVE_MODE_1SEC);

	return 1;
}

/**************************************************************************/
/*!
    @brief  sample rate of the sensor.
    @param  mode one of CCS811_DRIVE_MODE_IDLE, CCS811_DRIVE_MODE_1SEC, CCS811_DRIVE_MODE_10SEC, CCS811_DRIVE_MODE_60SEC, CCS811_DRIVE_MODE_250MS.
*/
void setDriveMode(uint8_t mode)
{
	DRIVE_MODE = mode;
	write8(CCS811_MEAS_MODE, get());
}

/**************************************************************************/
/*!
    @brief  enable the data ready interrupt pin on the device.
*/
/**************************************************************************/
void enableInterrupt()
{
	INT_DATARDY = 1;
	write8(CCS811_MEAS_MODE, get());
}

/**************************************************************************/
/*!
    @brief  disable the data ready interrupt pin on the device
*/
/**************************************************************************/
void disableInterrupt()
{
	INT_DATARDY = 0;
	write8(CCS811_MEAS_MODE, get());
}

/**************************************************************************/
/*!
    @brief  checks if data is available to be read.
    @returns True if data is ready, false otherwise.
*/
/**************************************************************************/
int8_t available()
{
	set_status(read8(CCS811_STATUS));
	if(!DATA_READY)
		return 0;
	else return 1;
}

/**************************************************************************/
/*!
    @brief  read and store the sensor data. This data can be accessed with getTVOC() and geteCO2()
    @returns 0 if no error, error code otherwise.
*/
/**************************************************************************/
uint8_t readData()
{
	if(!available())
		return 0;
	else{
		uint8_t buf[8];
//		read(CCS811_ALG_RESULT_DATA, buf, 8);

		_eCO2 = ((uint16_t)buf[0] << 8) | ((uint16_t)buf[1]);
		_TVOC = ((uint16_t)buf[2] << 8) | ((uint16_t)buf[3]);

		if(ERROR)
			return buf[5];

		else return 0;
	}
}

/**************************************************************************/
/*!
    @brief  set the humidity and temperature compensation for the sensor.
    @param humidity the humidity data as a percentage. For 55% humidity, pass in integer 55.
    @param temperature the temperature in degrees C as a decimal number. For 25.5 degrees C, pass in 25.5
*/
/**************************************************************************/
//void setEnvironmentalData(uint8_t humidity, double temperature)
//{
//	/* Humidity is stored as an unsigned 16 bits in 1/512%RH. The
//	default value is 50% = 0x64, 0x00. As an example 48.5%
//	humidity would be 0x61, 0x00.*/
//
//	/* Temperature is stored as an unsigned 16 bits integer in 1/512
//	degrees; there is an offset: 0 maps to -25°C. The default value is
//	25°C = 0x64, 0x00. As an example 23.5% temperature would be
//	0x61, 0x00.
//	The internal algorithm uses these values (or default values if
//	not set by the application) to compensate for changes in
//	relative humidity and ambient temperature.*/
//
//	uint8_t hum_perc = humidity << 1;
//
//	float fractional = modf(temperature, &temperature);
//	uint16_t temp_high = (((uint16_t)temperature + 25) << 9);
//	uint16_t temp_low = ((uint16_t)(fractional / 0.001953125) & 0x1FF);
//
//	uint16_t temp_conv = (temp_high | temp_low);
//
//	uint8_t buf[] = {hum_perc, 0x00,
//		(uint8_t)((temp_conv >> 8) & 0xFF), (uint8_t)(temp_conv & 0xFF)};
//
////	write(CCS811_ENV_DATA, buf, 4);
//
//}

/**************************************************************************/
/*!
    @brief  calculate the temperature using the onboard NTC resistor.
    @returns temperature as a double.
*/
/**************************************************************************/
//double calculateTemperature()
//{
//	uint8_t buf[4];
////	read(CCS811_NTC, buf, 4);
//
//	uint32_t vref = ((uint32_t)buf[0] << 8) | buf[1];
//	uint32_t vntc = ((uint32_t)buf[2] << 8) | buf[3];
//
//	//from ams ccs811 app note
//	uint32_t rntc = vntc * CCS811_REF_RESISTOR / vref;
//
//	double ntc_temp;
//	ntc_temp = log((double)rntc / CCS811_REF_RESISTOR); // 1
//	ntc_temp /= 3380; // 2
//	ntc_temp += 1.0 / (25 + 273.15); // 3
//	ntc_temp = 1.0 / ntc_temp; // 4
//	ntc_temp -= 273.15; // 5
//	return ntc_temp - _tempOffset;
//
//}

void set_status(uint8_t data)
{
	ERROR = data & 0x01;
	DATA_READY = (data >> 3) & 0x01;
	APP_VALID = (data >> 4) & 0x01;
	FW_MODE = (data >> 7) & 0x01;
}

void setTempOffset(float offset)
{
	_tempOffset = offset;
}

uint8_t get()
{
	return (INT_THRESH << 2) | (INT_DATARDY << 3) | (DRIVE_MODE << 4);
}

void set_error_id(uint8_t data)
{
    WRITE_REG_INVALID = data & 0x01;
    READ_REG_INVALID = (data & 0x02) >> 1;
    MEASMODE_INVALID = (data & 0x04) >> 2;
    MAX_RESISTANCE = (data & 0x08) >> 3;
    HEATER_FAULT = (data & 0x10) >> 4;
    HEATER_SUPPLY = (data & 0x20) >> 5;
}
/**************************************************************************/
/*!
    @brief  set interrupt thresholds
    @param low_med the level below which an interrupt will be triggered.
    @param med_high the level above which the interrupt will ge triggered.
    @param hysteresis optional histeresis level. Defaults to 50
*/
/**************************************************************************/
void setThresholds(uint16_t low_med, uint16_t med_high, uint8_t hysteresis)
{
	hysteresis = 50;
	uint8_t buf[] = {(uint8_t)((low_med >> 8) & 0xF), (uint8_t)(low_med & 0xF),
	(uint8_t)((med_high >> 8) & 0xF), (uint8_t)(med_high & 0xF), hysteresis};

//	write(CCS811_THRESHOLDS, buf, 5);
}

/**************************************************************************/
/*!
    @brief  trigger a software reset of the device
*/
/**************************************************************************/
//void SWReset()
//{
//	//reset sequence from the datasheet
//	uint8_t seq[] = {0x11, 0xE5, 0x72, 0x8A};
////	write(CCS811_SW_RESET, seq, 4);
//}

/**************************************************************************/
/*!
    @brief   read the status register and store any errors.
    @returns the error bits from the status register of the device.
*/
/**************************************************************************/
int8_t checkError()
{
	set_status(read8(CCS811_STATUS));
	return ERROR;
}

/**************************************************************************/
/*!
    @brief  write one byte of data to the specified register
    @param  reg the register to write to
    @param  value the value to write
*/
/**************************************************************************/
void write8(uint8_t reg, uint8_t value)
{
	write(reg, 1);
}

/**************************************************************************/
/*!
    @brief  read one byte of data from the specified register
    @param  reg the register to read
    @returns one byte of register data
*/
/**************************************************************************/
uint8_t read8(uint8_t reg)
{
	uint8_t ret;
	ret = read(reg/*, &ret, 1*/);

	return ret;
}

void _i2c_init()
{
	begin();
	#ifdef ESP8266
	setClockStretchLimit(500);
	#endif
}

uint8_t read(uint8_t reg/*, uint8_t *buf, uint8_t num*/)
{

	uint8_t num;
//	uint8_t pos = 0;
//
//	//on arduino we need to read in 32 byte chunks
//	while(pos < num){
//
//		uint8_t read_now = min((uint8_t)32, (uint8_t)(num - pos));
//		beginTransmission((uint8_t)_i2caddr);
//		write((uint8_t)reg + pos);
//		endTransmission();
//		requestFrom((uint8_t)_i2caddr, read_now);
//
//		for(int i=0; i<read_now; i++){
//			buf[pos] = read();
//			pos++;
//		}
//	}
	I2C0->CMD = I2C_CMD_START;
	I2C0->TXDATA = (CCS811_ADDRESS << 1) | WRITE;	//Command to write to sensor
	while ((I2C0->IF & I2C_IF_ACK) == 0);	//Wait for Ack
	I2C0->IFC = I2C_IFC_ACK;

	I2C0->TXDATA = reg;	//command for reading from register
	while ((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;

	I2C0->CMD = I2C_CMD_START;
	I2C0->TXDATA = (CCS811_ADDRESS << 1) | READ;	//command to read from sensor
	while ((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;
	while ((I2C0->IF & I2C_IF_RXDATAV) == 0);

	num = I2C0->RXDATA;
	I2C0->CMD = I2C_CMD_NACK;
	I2C0->CMD = I2C_CMD_STOP;

	return num;
}

void write(uint8_t reg,/* uint8_t *buf,*/ uint8_t num)
{
	I2C0->CMD = I2C_CMD_START;
	I2C0->TXDATA = (CCS811_ADDRESS << 1) | WRITE;	//Command to write to sensor
	while ((I2C0->IF & I2C_IF_ACK) == 0);	//Wait for Ack
	I2C0->IFC = I2C_IFC_ACK;

	I2C0->TXDATA = reg;
	while ((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;

	//data
	if(num == 255)
	{
	I2C0->CMD = I2C_CMD_STOP;
	}
	else
	{
	I2C0->TXDATA = num;
	while ((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;
	I2C0->CMD = I2C_CMD_STOP;
	}
	//	beginTransmission((uint8_t)_i2caddr);
//	write((uint8_t)reg);
//	write((uint8_t *)buf, num);
//	endTransmission();
}

uint16_t read16(uint8_t reg/*, uint8_t *buf, uint8_t num*/)
{

	uint16_t num16;
	uint8_t num8;
	I2C0->CMD = I2C_CMD_START;
	I2C0->TXDATA = (CCS811_ADDRESS << 1) | WRITE;	//Command to write to sensor

	while ((I2C0->IF & I2C_IF_ACK) == 0);	//Wait for Ack
	I2C0->IFC = I2C_IFC_ACK;

	I2C0->TXDATA = reg;	//command for reading from register
	while ((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;

	I2C0->CMD = I2C_CMD_STOP;
	I2C0->CMD = I2C_CMD_START;
	I2C0->TXDATA = (CCS811_ADDRESS << 1) | READ;	//command to read from sensor

	while ((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;
	while ((I2C0->IF & I2C_IF_RXDATAV) == 0);

//	UDELAY_Delay(100000);

	num16 = I2C0->RXDATA;
//	I2C0->IFC = I2C_IFC_ACK;
	I2C0->CMD = I2C_CMD_ACK;

	num16 = num16 << 8;

	while ((I2C0->IF & I2C_IF_RXDATAV) == 0);

//	UDELAY_Delay(10000);

	num8 = I2C0->RXDATA;

	num16 = num16 | num8;

	I2C0->CMD = I2C_CMD_NACK;
	I2C0->CMD = I2C_CMD_STOP;

	return num16;
}

void write_boot()
{
	I2C0->CMD = I2C_CMD_START;
	I2C0->TXDATA = (CCS811_ADDRESS << 1) | WRITE;	//Command to write to sensor
	while ((I2C0->IF & I2C_IF_ACK) == 0);	//Wait for Ack
	I2C0->IFC = I2C_IFC_ACK;

	I2C0->TXDATA = 0xFF;
	while ((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;

	//data
	I2C0->TXDATA = 0x11;
	while ((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;

	I2C0->TXDATA = 0xE5;
	while ((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;

	I2C0->TXDATA = 0x72;
	while ((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;

	I2C0->TXDATA = 0x8A;
	while ((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;

	I2C0->CMD = I2C_CMD_STOP;
}

//void read_alg_result(uint8_t reg)
//{
//	I2C0->CMD = I2C_CMD_START;
//	I2C0->TXDATA = (CCS811_ADDRESS << 1) | WRITE;	//Command to write to sensor
//
////	I2C0->CMD = I2C_CMD_START;
//
//	while ((I2C0->IF & I2C_IF_ACK) == 0);	//Wait for Ack
//	I2C0->IFC = I2C_IFC_ACK;
//
//	I2C0->TXDATA = reg;	//command for reading from register
//	while ((I2C0->IF & I2C_IF_ACK) == 0);
//	I2C0->IFC = I2C_IFC_ACK;
//
//	I2C0->CMD = I2C_CMD_STOP;
//
//	I2C0->CMD = I2C_CMD_START;
//	I2C0->TXDATA = (CCS811_ADDRESS << 1) | READ;	//command to read from sensor
//
//	while ((I2C0->IF & I2C_IF_ACK) == 0);
//	while ((I2C0->IF & I2C_IF_RXDATAV) == 0);
//
//	for(int i=0; i<4; i++)
//	{
////		I2C0->CMD = I2C_CMD_START;
////		I2C0->TXDATA = (CCS811_ADDRESS << 1) | READ;	//command to read from sensor
//
////		I2C0->CMD = I2C_CMD_START;
//
////		while ((I2C0->IF & I2C_IF_ACK) == 0);
////		while ((I2C0->IF & I2C_IF_RXDATAV) == 0);
//
//		buffs[i] = I2C0->RXDATA;
//		I2C0->IFC = I2C_IFC_ACK;
//		I2C0->CMD = I2C_CMD_ACK;
//
//		buffs[i] = buffs[i] << 8;
//
//		while ((I2C0->IF & I2C_IF_RXDATAV) == 0);
//
////		for(int k =0; k < 10000; k++);
//
//		buff_8[i] = I2C0->RXDATA;
//
//		buffs[i] = buffs[i] | buff_8[i];
//
//		I2C0->IFC = I2C_IFC_ACK;
//		I2C0->CMD = I2C_CMD_ACK;
//
////		I2C0->CMD = I2C_CMD_NACK;
////		I2C0->CMD = I2C_CMD_STOP;
//	}
//	I2C0->CMD = I2C_CMD_NACK;
//	I2C0->CMD = I2C_CMD_STOP;
//
//}
