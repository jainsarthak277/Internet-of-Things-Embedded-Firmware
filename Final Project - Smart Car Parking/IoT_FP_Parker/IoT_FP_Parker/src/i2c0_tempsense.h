#include "em_i2c.h"
#include "src/gpio.h"
#include "src/CCS811.h"

void i2c_init(void);
void i2c_driver(void);
void i2c_disable(void);
void compare_temp(void);

//#define MAX30105
//#define SI7021
#define CCS811

#ifndef CCS811
#define WRITE 0x00
#define READ 0x01
#endif
#ifdef SI7021
#define I2C_DEVICE_ADD 0x40
#define READ_TEMP 0xE3
#endif
#ifdef MAX30105
#define FIFO_WR_PTR 0x04
#define FIFO_RD_PTR 0x06
#define FIFO_DATA_PTR 0x07
#define I2C_DEVICE_ADD 0x57
#define WRITE_ADDR 0xAE
#define READ_ADDR 0xAF
#endif
#ifdef CCS811
uint8_t meas_mode;
uint8_t status_reg;
uint16_t reading_eCO2;
uint8_t* ptr;
uint16_t number16;
uint8_t number8;
uint8_t fire_detected;
#endif


#ifdef SI7021
#define SENSOR_PORT gpioPortD
#define SENSOR_PIN 15
#endif
#ifdef MAX30105
#define SENSOR_INT_PORT gpioPortD
#define SENSOR_INT_PIN 10
#endif

#define I2C_SCL_port gpioPortC
#define I2C_SCL_pin 10

#define I2C_SDA_port gpioPortC
#define I2C_SDA_pin 11

#define THRESHOLD 15

uint16_t rxdata_MSB;
uint16_t rxdata;
uint8_t rxdata_LSB;
