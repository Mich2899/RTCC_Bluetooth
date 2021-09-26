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

/* function     : letimerMilliseconds
 * params       : void
 * brief        : Calculates milliseconds since boot
 * return_type  : uint32_t
 * */
uint32_t letimerMilliseconds();

#endif /* SRC_IRQ_H_ */
