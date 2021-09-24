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
//set the myevents global variable to Underflow event.
void scheduler_evtUF () {
  CORE_DECLARE_IRQ_STATE;
  // set event
  CORE_ENTER_CRITICAL();              // turn off interrupts in NVIC
  myEvents = evtUF;                   // set event to underflow event
  CORE_EXIT_CRITICAL();               // re-enable interrupts in NVIC
}

//set the myevents global variable to COMP1 event
void scheduler_evtCOMP1 () {
  CORE_DECLARE_IRQ_STATE;
  // set event
  CORE_ENTER_CRITICAL();              // turn off interrupts in NVIC
  myEvents = evtCOMP1;                // set event to COMP1 event
  CORE_EXIT_CRITICAL();               // re-enable interrupts in NVIC
}

//set the myevents global variable to I2C event
void scheduler_evtI2C () {
  CORE_DECLARE_IRQ_STATE;
  // set event
  CORE_ENTER_CRITICAL();              // turn off interrupts in NVIC
  myEvents = evtI2C;                  // set event to I2C event
  CORE_EXIT_CRITICAL();               // re-enable interrupts in NVIC
}

// scheduler routine to return 1 event to state machine and clear that event
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


void temperature_state_machine (uint32_t event) {

     state_t currentState;
     static state_t nextState = Idle;
     currentState = nextState;

     switch (currentState) {
       case Idle:
         nextState = Idle;
         if (event == evtUF) {
             //sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM3);
             warmup();
             nextState = Warmup ;
         }
         break;


       case Warmup:
         nextState = Warmup;
         if (event == evtCOMP1) {
             LETIMER_IntDisable(LETIMER0, LETIMER_IEN_COMP1);
             //sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM3);
             //sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
             I2C_write();
             nextState = write;
         }
         break;

       case write:
         nextState = write;
         if (event == evtI2C) {
             NVIC_DisableIRQ(I2C0_IRQn);
             //sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
             //sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM3);
             timerWaitUs_irq(10000);
             nextState = timerwait;
         }
         break;


       case timerwait:
         nextState = timerwait;
         if (event == evtCOMP1) {
             LETIMER_IntDisable(LETIMER0, LETIMER_IEN_COMP1);
             //sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM3);
             //sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
             I2C_read();
             nextState = read;
         }
         break;


       case read:
         nextState = read;
         if (event == evtI2C) {
             NVIC_DisableIRQ(I2C0_IRQn);
             store();
             //sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
             //sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM3);
             nextState = Idle;
         }
         break;


       default:
         //sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM3);
         break;
     } // switch
} // state_machine()
