
void i2c_init(void);
void i2c_driver(void);
void i2c_disable(void);
float compare_temp(void);

#define WRITE 0x00
#define READ 0x01
#define I2C_DEVICE_ADD 0x40
#define READ_TEMP 0xE3

#define SENSOR_PORT gpioPortD
#define SENSOR_PIN 15

#define I2C_SCL_port gpioPortC
#define I2C_SCL_pin 10

#define I2C_SDA_port gpioPortC
#define I2C_SDA_pin 11

#define THRESHOLD 15

uint16_t rxdata_MSB;
float temprxdata;
uint8_t rxdata_LSB;
