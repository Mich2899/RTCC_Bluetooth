/*
 * scheduler.h
 * Contains function prototypes for scheduling temperature measurement event. Check .c file for function definition.
 *  Created on: Sep 15, 2021
 *      Author: mich1576
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

#include <stdint.h>
#include "em_core.h"
#include "i2c.h"

//enum to determine which state/event to be in
typedef enum {
evtNoEvent = 0,
evtReadTemperature = 1,
}event;

//function prototypes
/*  function          : schedulerSetReadTempEvent
 *  params            : none
 *  brief             : set the myevents global variable to read temperature event.
 *                      Makes sure that the variable is assigned value in critical section
 *  return-type       : void
 * */
void schedulerSetReadTempEvent ();

/*  function          : getNextEvent
 *  params            : none
 *  brief             : save the current event in a variable return that value and
 *                      set the myEvents variable to noEvent
 *  return-type       : uint32_t
 * */
uint32_t getNextEvent();

#endif /* SRC_SCHEDULER_H_ */
