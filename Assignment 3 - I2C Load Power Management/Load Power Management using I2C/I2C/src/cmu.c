//***********************************************************************************
// Include files
//***********************************************************************************
#include "src/cmu.h"
#include"src/main.h"
#include "sleep.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
void cmu_init(void){

	CMU_OscillatorEnable(cmuOsc_LFXO , true, true);// LFXO Oscillator Enable
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

	CMU_OscillatorEnable(cmuOsc_ULFRCO , true, true);//ULFRCO Oscillator Enable
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);


	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_I2C0, true);
	CMU_ClockEnable(cmuClock_CORELE, true);
	CMU_ClockEnable(cmuClock_LFA, true);
	CMU_ClockEnable(cmuClock_LETIMER0, true);
	// Peripheral clocks enabled
	CMU_ClockEnable(cmuClock_GPIO, true);
}

