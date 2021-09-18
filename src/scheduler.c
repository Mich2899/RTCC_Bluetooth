/*
 * scheduler.c
 * Schedules event to measure temperature from SI7021.
 *  Created on: Sep 15, 2021
 *      Author: mich1576
 */

/*********************************************************INCLUDES****************************************************************/
#include "scheduler.h"

/*******************************************************GLOBAL VARIABLES*********************************************************/
uint32_t myEvents = 0;

/****************************************************FUNCTION DEFINITION**********************************************************/
//set the myevents global variable to read temperature event.Makes sure that the variable is assigned value in critical section
void schedulerSetReadTempEvent () {
  CORE_DECLARE_IRQ_STATE;
  // set event
  CORE_ENTER_CRITICAL();              // turn off interrupts in NVIC
  myEvents = evtReadTemperature;      //event to read temperature
  CORE_EXIT_CRITICAL();               // re-enable interrupts in NVIC
}

// scheduler routine to return 1 event to main()code and clear that event
uint32_t getNextEvent() {
    uint32_t theEvent;

    theEvent = myEvents;

    CORE_DECLARE_IRQ_STATE;
    // enter critical section
    CORE_ENTER_CRITICAL();              // turn off interrupts in NVIC
    // clear the event in your data structure, this is a read-modify-write
    myEvents = evtNoEvent;              //no event
    // exit critical section
    CORE_EXIT_CRITICAL();               // re-enable interrupts in NVIC

    return (theEvent);
} // getNextEvent()
