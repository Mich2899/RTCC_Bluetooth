/*
 * scheduler.c
 * Schedules event to measure temperature from SI7021.
 *  Created on: Sep 15, 2021
 *      Author: mich1576
 */

/*********************************************************INCLUDES****************************************************************/
#include "scheduler.h"


// Include logging for this file
#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"


/*******************************************************GLOBAL VARIABLES*********************************************************/
uint32_t myEvents = 0;

/****************************************************FUNCTION DEFINITION**********************************************************/
//set the myevents global variable to Underflow event.
void scheduler_evtUF () {
  CORE_DECLARE_IRQ_STATE;
  // set event
  CORE_ENTER_CRITICAL();              // turn off interrupts in NVIC
  // DOS: These need to be read-modify-writes (RMW)
  //myEvents = evtUF;                   // set event to underflow event
  myEvents |= evtUF;                   // set event to underflow event
  CORE_EXIT_CRITICAL();               // re-enable interrupts in NVIC
}

//set the myevents global variable to COMP1 event
void scheduler_evtCOMP1 () {
  CORE_DECLARE_IRQ_STATE;
  // set event
  CORE_ENTER_CRITICAL();              // turn off interrupts in NVIC
  myEvents |= evtCOMP1; //DOS RMW              // set event to COMP1 event
  CORE_EXIT_CRITICAL();               // re-enable interrupts in NVIC
}

//set the myevents global variable to I2C event
void scheduler_evtI2C () {
  CORE_DECLARE_IRQ_STATE;
  // set event
  CORE_ENTER_CRITICAL();              // turn off interrupts in NVIC
  myEvents |= evtI2C;  //DOS RMW                 // set event to I2C event
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

    // DOS
    if (myEvents & evtUF) {
        theEvent = evtUF;  // returned event
        myEvents ^= evtUF; // RMW, clear the event from our data structure
    } else

      if (myEvents & evtCOMP1) {
          theEvent = evtCOMP1;  // returned event
          myEvents ^= evtCOMP1; // RMW, clear the event from our data structure
      } else

        if (myEvents & evtI2C) {
            theEvent = evtI2C;  // returned event
            myEvents ^= evtI2C; // RMW, clear the event from our data structure
        }

    // exit critical section
    CORE_EXIT_CRITICAL();               // re-enable interrupts in NVIC

    return (theEvent);
} // getNextEvent()


void temperature_state_machine (uint32_t event) {

            state_t   currentState;
     static state_t   nextState = Idle;

     currentState = nextState;

     switch (currentState) {
       case Idle:
         nextState = Idle;
         if (event == evtUF) {
             //sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM3);
             warmup();
             timerWaitUs_irq(80000); // DOS wait for power-up of 7021
             nextState = Warmup ;
             LOG_INFO("To Warmup"); // DOS

             // DOS test code
             //timerWaitUs_irq(1000000); // 1 sec
             //gpioToggleLED0(); // DOS tled on
         }
         break;


       case Warmup:
         nextState = Warmup;
         if (event == evtCOMP1) {
             //DOS: moved this to your irq.c LETIMER_IntDisable(LETIMER0, LETIMER_IEN_COMP1);
             //sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM3);

             // DOS test code to test your IRQ driven wait function
//             nextState = Idle;
//             LOG_INFO("To Idle"); // DOS
//             gpioToggleLED0(); // led off

             sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
             I2C_write(); // initiate I2C write of 0xF3 command to the 7021
             nextState = write;
             LOG_INFO("To write"); // DOS

         }
         break;

       case write:
         nextState = write;
         if (event == evtI2C) {
             NVIC_DisableIRQ(I2C0_IRQn);
             sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
             //sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM3);
             //timerWaitUs_irq(10000);
             timerWaitUs_irq(10800); // spec from 7021 data sheet is 10.8 ms
             nextState = timerwait;
             LOG_INFO("To timerwait"); // DOS
         }
         break;


       case timerwait:
         nextState = timerwait;
         if (event == evtCOMP1) {
             //DOS: moved this to your irq.cLETIMER_IntDisable(LETIMER0, LETIMER_IEN_COMP1);
             //sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM3);
             sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
             I2C_read();
             nextState = read;
             LOG_INFO("To read"); // DOS
         }
         break;


       case read:
         nextState = read;
         if (event == evtI2C) {
             NVIC_DisableIRQ(I2C0_IRQn);
             turnoff();
             store(); // logs the temp to the UART/VCOM port
             sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
             //sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM3);
             nextState = Idle;
             LOG_INFO("To Idle"); // DOS
         }
         break;


       default:
         //sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM3);
         break;
     } // switch
} // state_machine()
