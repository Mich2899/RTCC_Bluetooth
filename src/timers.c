/*
 * timers.c
 * This file contains function prototype for timer initialization and interrupt settings.
 *  Created on: Sep 9, 2021
 *      Author: mich1576
 */
#include "timers.h"

//Logging for this file
#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"

/* function       : initLETIMER0
 * params         : void
 * brief          : initialize LETIMER0, set the compare register values based on the energy mode settings and set the correct settings for interrupt
 * return type    : void
 */
void initLETIMER0 (void){
  LETIMER_Init_TypeDef timer_instance=
      {
          .enable         = true,                 //Don't start counting when initialization completes

          .debugRun       = true,                 //Counter will not keep running during debug halt

          .comp0Top       = true,                 //Load COMP0 register into CNT when counter underflows

          .bufTop         = false,                //Don't load COMP1 into COMP0 when REP0 reaches 0

          .out0Pol        = 0,                    //Idle value for output 0
          .out1Pol        = 0,                    //Idle value for output 1
          .ufoa0          = letimerUFOANone,      //No output action
          .ufoa1          = letimerUFOANone,      //No output action
          .repMode        = letimerRepeatFree,    //Repeat until stopped

          .topValue   = 0,                        //Use default top Value
        };

  LETIMER_Init(LETIMER0, &timer_instance);

#if ((LOWEST_ENERGY_MODE == 0) || (LOWEST_ENERGY_MODE == 1) || (LOWEST_ENERGY_MODE == 2))
    LETIMER_CompareSet(LETIMER0, 0, VALUE_TO_LOAD_LFXO_COMP0);
    LETIMER_CompareSet(LETIMER0, 1, VALUE_TO_LOAD_LFXO_COMP1);

#elif(LOWEST_ENERGY_MODE == 3)
    LETIMER_CompareSet(LETIMER0, 0, VALUE_TO_LOAD_ULFRCO_COMP0);
    LETIMER_CompareSet(LETIMER0, 1, VALUE_TO_LOAD_ULFRCO_COMP1);
#endif

  LETIMER_IntClear(LETIMER0, LETIMER_IF_UF);          //clear the underflow flag
  LETIMER_IntClear(LETIMER0, LETIMER_IF_COMP1);       //clear the COMP1 interrupt flag
  LETIMER0->IEN = LETIMER_IEN_UF | LETIMER_IEN_COMP1; //enable both the flags in interrupt enable register

//  LETIMER_Enable(LETIMER0,true);                      //Enable LETIMER0
}
