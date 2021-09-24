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
#include "src/gpio.h" // DOS

//function prorotypes
/* function       : LETIMER0_IRQHandler
 * params         : void
 * brief          : check for any timer interrupts, set the scheduler event accordingly
 * return type    : void
 */
// DOS these should be private functions, dangerous to allow other .c files to know about the funciton name
// void LETIMER0_IRQHandler(void);

/* function       : I2C0_IRQHandler
 * params         : void
 * brief          : check for any interrupts, set the scheduler event based on the
 *                  previous state.
 * return type    : void
 */
//void I2C0_IRQHandler(void);

uint32_t letimerMilliseconds();

#endif /* SRC_IRQ_H_ */
