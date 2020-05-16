/*
 * letimer0.h
 *
 *  Created on: Sep 13, 2018
 *      Author: jains
 */

#ifndef LETIMER0_H_
#define LETIMER0_H_

#include "em_letimer.h"
#include "em_emu.h"

void letimer_setup(void);
void LETIMER0_IRQHandler(void);

uint16_t schedule_event;

#endif /* LETIMER0_H_ */
