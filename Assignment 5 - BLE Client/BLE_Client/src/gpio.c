//***********************************************************************************
// Include files
//***********************************************************************************
#include "src/gpio.h"
#include "src/i2c0_tempsense.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************


//***********************************************************************************
// functions
//***********************************************************************************
void gpio_init(void){

	// Set LED ports to be standard output drive with default off (cleared)
	//GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, LED0_default);

	GPIO_PinModeSet(LED0_port, 6, gpioModeInputPull, 1);

	//GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED1_default);

	GPIO_PinModeSet(I2C_SCL_port, I2C_SCL_pin, gpioModeWiredAnd, true);
	GPIO_PinModeSet(I2C_SDA_port, I2C_SDA_pin, gpioModeWiredAnd, true);
	GPIO_PinModeSet(gpioPortD, 15, gpioModePushPull, true);
}
