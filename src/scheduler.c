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
  myEvents |= evtUF;                   // set event to underflow event
  CORE_EXIT_CRITICAL();               // re-enable interrupts in NVIC
}

//set the myevents global variable to COMP1 event
void scheduler_evtCOMP1 () {
  CORE_DECLARE_IRQ_STATE;
  // set event
  CORE_ENTER_CRITICAL();              // turn off interrupts in NVIC
  myEvents |= evtCOMP1;                // set event to COMP1 event
  CORE_EXIT_CRITICAL();               // re-enable interrupts in NVIC
}

//set the myevents global variable to I2C event
void scheduler_evtI2C () {
  CORE_DECLARE_IRQ_STATE;
  // set event
  CORE_ENTER_CRITICAL();              // turn off interrupts in NVIC
  myEvents |= evtI2C;                  // set event to I2C event
  CORE_EXIT_CRITICAL();               // re-enable interrupts in NVIC
}

// scheduler routine to return 1 event to state machine and clear that event
uint32_t getNextEvent() {
    uint32_t theEvent;

    theEvent = evtNoEvent;

    CORE_DECLARE_IRQ_STATE;
   // clear the event in your data structure, this is a read-modify-write

    theEvent = evtNoEvent;  // DOS, event to return

    // enter critical section
    CORE_ENTER_CRITICAL();              // turn off interrupts in NVIC

    if (myEvents & evtUF) {
        theEvent = evtUF;  // returned event
        myEvents ^= evtUF; // RMW, clear the event from our data structure
    }

    else if (myEvents & evtCOMP1) {
          theEvent = evtCOMP1;  // returned event
          myEvents ^= evtCOMP1; // RMW, clear the event from our data structure
    }

    else if (myEvents & evtI2C) {
            theEvent = evtI2C;  // returned event
            myEvents ^= evtI2C; // RMW, clear the event from our data structure
    }

    // exit critical section
    CORE_EXIT_CRITICAL();               // re-enable interrupts in NVIC

    return (theEvent);
} // getNextEvent()

void temperature_state_machine (uint32_t event) {

     state_t currentState;                                                      //variable to get the current state
     static state_t nextState = Idle;                                           //initializing next state as ideal
     currentState = nextState;                                                  //initialize current state as next state

     switch (currentState) {                                                    //check the value of current state
       case Idle:                                                               //case when the device is in idle state
         nextState = Idle;                                                      //next state should be idle unless an event is encountered
         if (event & evtUF) {                                                   //if an underflow event is encountered
             warmup();                                                          //turn the sensor on
             timerWaitUs_irq(80000);                                            //wait for power up of 7021
             nextState = Warmup ;                                               //set the next state to warmup
         }
         break;


       case Warmup:                                                             //if the device is in warmup state
         nextState = Warmup;                                                    //next state should be warmup unless an event is encountered
         if (event & evtCOMP1) {                                                //if COMP1 event encountered
             LETIMER_IntDisable(LETIMER0, LETIMER_IEN_COMP1);                   //disable COMP1 interrupt
             sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);         //sleep in EM1 while performing EM1 transaction
             I2C_write();                                                       //perform I2C write
             nextState = write;                                                 //set the next state to write
         }
         break;

       case write:                                                              //if the device is in write state
         nextState = write;                                                     //next state should be write until event is encountered
         if (event & evtI2C) {                                                  //if I2C event encountered
             NVIC_DisableIRQ(I2C0_IRQn);                                        //Disable I2C interrupt
             sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);      //go back to EM3 sleep state
             timerWaitUs_irq(11000);                                            //wait at least 10.8ms
             nextState = timerwait;                                             //set next state to timerwait
         }
         break;


       case timerwait:                                                          //if device in timerwait state
         nextState = timerwait;                                                 //next state should be timerwait until an event encountered
         if (event & evtCOMP1) {                                                //if COMP1 event encountered
             LETIMER_IntDisable(LETIMER0, LETIMER_IEN_COMP1);                   //disable COMP1 interrupt
             sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);         //sleep in EM1 while performing EM1 transaction
             I2C_read();                                                        //perform I2C read
             nextState = read;                                                  //set next state to read
         }
         break;


       case read:                                                               //if device in read state
         nextState = read;                                                      //next state should be timerwait until an event encountered
         if (event & evtI2C) {                                                  //if I2C event encountered
             NVIC_DisableIRQ(I2C0_IRQn);                                        //Disable I2C interrupt
             turnoff();                                                         //turn the sensor off
             store();                                                           //convert and store the temperature data for logging
             sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);      //go back to EM3 sleep state
             nextState = Idle;                                                  //set next state to Idle
         }
         break;

       default:
         break;
     } // switch
} // state_machine()
