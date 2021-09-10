/*
 * irq.h
 * This file contains function prototype defining the functionality in interrupt handler.Check .c file for function definition.
 *  Created on: Sep 9, 2021
 *      Author: mich1576
 */

#ifndef SRC_IRQ_H_
#define SRC_IRQ_H_
#include "timers.h"
#include "gpio.h"
#include "oscillators.h"

//function prorotypes
void LETIMER0_IRQHandler(void);

#endif /* SRC_IRQ_H_ */
