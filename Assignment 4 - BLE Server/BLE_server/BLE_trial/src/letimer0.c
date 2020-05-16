/*
 * letimer0.c
 *
 *  Created on: Sep 13, 2018
 *      Author: jains
 */
#include "src/main.h"
#include "src/letimer0.h"
#include "em_core.h"
#include "em_letimer.h"
#include "em_gpio.h"
#include "sleep.h"
#include "src/gpio.h"
#include "src/cmu.h"
#include "math.h"

int freq, j;
float freqlo;

void letimer_setup(void)
{
	BlockSleep(sleepEM4);
	const LETIMER_Init_TypeDef letimerInit =
	{
			.enable = false,
			.debugRun = false,
			.comp0Top = true,
			.bufTop = false,
			.out0Pol = 0,
			.out1Pol = 0,
			.ufoa0 = letimerUFOANone,
			.ufoa1 = letimerUFOANone,
			.repMode = letimerRepeatFree
	};
	LETIMER_Init(LETIMER0, &letimerInit);
	int comp0_value;
	freqlo = 1000;
	comp0_value = freqlo*TIME_P;	//Load COMP0 with product of time period and ULFRCO frequency
	LETIMER_CompareSet(LETIMER0, 0, comp0_value);

	while (LETIMER0->SYNCBUSY);	//Wait for synchronization

	LETIMER_IntClear(LETIMER0, LETIMER_IFC_COMP0);
	LETIMER_IntEnable(LETIMER0, LETIMER_IEN_COMP0);

	NVIC_EnableIRQ(LETIMER0_IRQn);
	LETIMER0->CMD = LETIMER_CMD_START;
}

void LETIMER0_IRQHandler(void)
{
		CORE_ATOMIC_IRQ_DISABLE();
//	int flags;
//	flags = LETIMER_IntGet(LETIMER0);	//Get pending flags in variable
//		if (LETIMER_IF_COMP0 & flags/* & (sleepdone  == 1)*/)	//Check for relevant bit in COMP0 register
//		{
//			schedule_event |= LETIMER_IF_COMP0;	//Assign to schedule_event
//		}
		gecko_external_signal(LETIMER_IF_COMP0);
		CORE_ATOMIC_IRQ_ENABLE();
		LETIMER_IntClear(LETIMER0, LETIMER_IFC_COMP0);
}

