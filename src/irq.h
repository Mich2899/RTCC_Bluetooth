/*
 * irq.h
 * This file contains function prototype defining the functionality in interrupt handler.Check .c file for function definition.
 *  Created on: Sep 9, 2021
 *      Author: mich1576
 */

#ifndef SRC_IRQ_H_
#define SRC_IRQ_H_

#include "timers.h"
#include "oscillators.h"
#include "scheduler.h"

//function prorotypes
/* function       : LETIMER0_IRQHandler
 * params         : void
 * brief          : check for any interrupts, turn the LED on and off accordingly and reset the interrupt flags.
 * return type    : void
 */
void LETIMER0_IRQHandler(void);

#endif /* SRC_IRQ_H_ */
