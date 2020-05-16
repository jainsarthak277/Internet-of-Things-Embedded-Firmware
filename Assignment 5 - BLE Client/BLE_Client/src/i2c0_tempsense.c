
#include "em_i2c.h"
#include "sleep.h"
#include "src/main.h"
#include "src/gpio.h"
#include "src/i2c0_tempsense.h"

void i2c_init(void)
{
	BlockSleep(sleepEM2);	//Enter EM1

	I2C0->ROUTEPEN |= I2C_ROUTEPEN_SCLPEN;
	I2C0->ROUTEPEN |= I2C_ROUTEPEN_SDAPEN;

	I2C0->ROUTELOC0 |= (I2C0->ROUTELOC0 & (~_I2C_ROUTELOC0_SCLLOC_MASK)) | I2C_ROUTELOC0_SCLLOC_LOC14;
	I2C0->ROUTELOC0 |= (I2C0->ROUTELOC0 & (~_I2C_ROUTELOC0_SDALOC_MASK)) | I2C_ROUTELOC0_SDALOC_LOC16;

	const I2C_Init_TypeDef i2c_initial =	//I2C initialization
	{
			.clhr = i2cClockHLRStandard,
			.enable = false,
			.freq = I2C_FREQ_STANDARD_MAX,
			.master = true,
			.refFreq = 0
	};
	I2C_Init(I2C0, &i2c_initial);

	for (int i = 0;i < 9;i++)	//Loop for synchronization
		{
			GPIO_PinOutClear(I2C_SCL_port, I2C_SCL_pin);
			GPIO_PinOutSet(I2C_SCL_port, I2C_SCL_pin);
		}

	if (I2C0->STATE & I2C_STATE_BUSY)	//If I2C line busy, abort
		I2C0->CMD = I2C_CMD_ABORT;

	GPIO_PinOutSet(SENSOR_PORT, SENSOR_PIN);	//Sensor port and pin enable
	for (int k = 0; k < 200000; k++);	//Wait 80 ms for sensor to setup

	I2C_Enable(I2C0, true); //Enable I2C
}

void i2c_disable(void)
{
	I2C0->ROUTEPEN &= ~(I2C_ROUTEPEN_SCLPEN);
	I2C0->ROUTEPEN &= ~(I2C_ROUTEPEN_SDAPEN);

	I2C_Enable(I2C0, false);

	GPIO_PinOutClear(I2C_SDA_port, I2C_SDA_pin);
	GPIO_PinOutClear(I2C_SCL_port, I2C_SCL_pin);

	//GPIO_PinOutClear(SENSOR_PORT, SENSOR_PIN);	//Disable Sensor
}

void i2c_driver(void)
{
	I2C0->TXDATA = (I2C_DEVICE_ADD << 1) | WRITE;	//Command to write to sensor
	I2C0->CMD = I2C_CMD_START;

	while ((I2C0->IF & I2C_IF_ACK) == 0);	//Wait for Ack
	I2C0->IFC = I2C_IFC_ACK;

	I2C0->TXDATA = READ_TEMP;	//READ_TEMP = 0xE3, command for reading temperature
	while ((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;

	I2C0->CMD = I2C_CMD_START;
	I2C0->TXDATA = (I2C_DEVICE_ADD << 1) | READ;	//command to read from sensor

	while ((I2C0->IF & I2C_IF_ACK) == 0);
	while ((I2C0->IF & I2C_IF_RXDATAV) == 0);	//Check if sensor is still transmitting
	rxdata_MSB = I2C0->RXDATA;	//MS 8 bits of temperature reading
	I2C0->IFC = I2C_IFC_ACK;
	I2C0->CMD = I2C_CMD_ACK;

	rxdata_MSB = rxdata_MSB << 8;

	while ((I2C0->IF & I2C_IF_RXDATAV) == 0);

	rxdata_LSB = I2C0->RXDATA;	//LS 8 bits of temperature reading
	rxdata_MSB = rxdata_MSB | rxdata_LSB;	//Combined

	I2C0->CMD = I2C_CMD_NACK;
	I2C0->CMD = I2C_CMD_STOP;

}

float compare_temp(void)
{
	temprxdata = 175.72*rxdata_MSB/65536 - 46.85;	//Converting register 16 bit value to degree C
	if (rxdata_MSB < THRESHOLD)	//Threshold value = 15 degrees Celsius
		{
		GPIO_PinOutSet(LED1_port, LED1_pin);
		}
	return temprxdata;
}
