/*
 * timers.h
 * This file contains function prototype for timer initialization and interrupt settings. Check .c file for function definition.
 *  Created on: Sep 9, 2021
 *      Author: mich1576
 */

#ifndef SRC_TIMERS_H_
#define SRC_TIMERS_H_

#include "em_letimer.h"
#include "main.h"
#include "app.h"
#include "oscillators.h"
#include <stdint.h>

//function prototypes
/* function       : initLETIMER0
 * params         : void
 * brief          : initialize LETIMER0, set the compare register values based on the energy mode settings and set the correct settings for interrupt
 * return type    : void
 */
void initLETIMER0 ();

/* function       : timerWaitUs_polled
 * params         : uint32_t us_wait
 * brief          : Takes input in microseconds and provides required amount of delay using polling method
 * return type    : void
 */
void timerWaitUs_polled(uint32_t us_wait);

/* function       : timerWaitUs_interrupt
 * params         : uint32_t us_wait
 * brief          : Takes input in microseconds and provides required amount of delay using interrupt method
 * return type    : void
 */
void timerWaitUs_irq(uint32_t us_wait);
#endif /* SRC_TIMERS_H_ */
