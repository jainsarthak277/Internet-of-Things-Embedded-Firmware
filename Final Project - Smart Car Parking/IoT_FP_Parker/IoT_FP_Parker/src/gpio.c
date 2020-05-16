//***********************************************************************************
// Include files
//***********************************************************************************
#include "src/gpio.h"

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
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
//	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, LED0_default);
//
//	GPIO_PinModeSet(LED0_port, 6, gpioModeInputPull, 1);
//
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
//	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED1_default);

	GPIO_PinModeSet(gpioPortF, 6U, gpioModeInputPull, true);
	GPIO_PinModeSet(gpioPortF, 7U, gpioModeInputPull, true);

	GPIO_PinModeSet(I2C_SCL_PORT, I2C_SCL_PIN, gpioModeWiredAnd, true);
	GPIO_PinModeSet(I2C_SDA_PORT, I2C_SDA_PIN, gpioModeWiredAnd, true);
}
