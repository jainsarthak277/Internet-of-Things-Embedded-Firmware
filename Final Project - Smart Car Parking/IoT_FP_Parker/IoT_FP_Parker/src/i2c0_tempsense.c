#include "src/i2c0_tempsense.h"

void i2c_init(void)
{
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

	for (int k = 0; k < 200000; k++);	//Wait 80 ms for sensor to setup

	I2C_Enable(I2C0, true); //Enable I2C
//	write_boot();

	status_reg = read(CCS811_STATUS);
	if((status_reg & 0b00010000) == 0b00010000)
	{
		write(CCS811_BOOTLOADER_APP_START, 255);
	}
	else printf("No application  firmware present\r\n");
		meas_mode = read(CCS811_MEAS_MODE);
		meas_mode = 0b00010000;
		write(CCS811_MEAS_MODE, meas_mode);
		meas_mode = read(CCS811_MEAS_MODE);
		status_reg = read(CCS811_STATUS);
}

void i2c_disable(void)
{
	I2C0->ROUTEPEN &= ~(I2C_ROUTEPEN_SCLPEN);
	I2C0->ROUTEPEN &= ~(I2C_ROUTEPEN_SDAPEN);

	I2C_Enable(I2C0, false);

	GPIO_PinOutClear(I2C_SDA_port, I2C_SDA_pin);
	GPIO_PinOutClear(I2C_SCL_port, I2C_SCL_pin);
}

void i2c_driver(void)
{
	buffs[0] = read16(CCS811_ALG_RESULT_DATA);
	if(buffs[0] > 800)
	{
		fire_detected = 1;
		printf("\r\nFIRE DETECTED\r\n");
	}
}

