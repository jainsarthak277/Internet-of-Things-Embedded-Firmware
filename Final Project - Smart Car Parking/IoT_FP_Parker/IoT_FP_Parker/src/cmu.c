//***********************************************************************************
// Include files
//***********************************************************************************
#include "src/cmu.h"

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


	  /* Enable LE domain registers */
	  CMU_ClockEnable(cmuClock_CORELE, true);


	  CMU_ClockEnable(cmuClock_LFA, true);

	  /* Enable LFXO as LFACLK in CMU. This will also start LFXO */
	  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

	  CMU_ClockSelectSet(cmuClock_LFE, cmuSelect_LFXO);

	  /* Enable RTC clock */
	  CMU_ClockEnable(cmuClock_RTCC, true);

	CMU_OscillatorEnable(cmuOsc_LFXO , true, true);// LFXO Oscillator Enable
	CMU_OscillatorEnable(cmuOsc_ULFRCO , true, true);//ULFRCO Oscillator Enable

	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_I2C0, true);
	// Peripheral clocks enabled
	CMU_ClockEnable(cmuClock_GPIO, true);
}

