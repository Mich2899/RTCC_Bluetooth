/*
 * oscillators.c
 * This file contains function to initialize the oscillators and set appropriate values in CMU unit for respective energy modes
 *  Created on: Sep 9, 2021
 *      Author: mich1576
 */

/*********************************************************INCLUDES****************************************************************/
#include "oscillators.h"

/****************************************************FUNCTION DEFINITION**********************************************************/
//initializes the clock base on the energy mode setting and initializes the clock for use of LETIMER0
void init_oscillators(void){
  //Select the appropriate Low Frequency clock for the LFA clock tree depending on oscillator


/*  CMU_OscillatorEnable(cmuOsc_LFXO, true, true);                                   //select LFXO
    CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_LFXO);                                 //selecting the LFXO as the LFA clock
    CMU_ClockEnable(cmuClock_LFA, true);                                             //enabling the LFA clock
    CMU_ClockDivSet(cmuClock_LETIMER0, PRESCALAR_VALUE_LFXO);                        //providing the prescalar value for LETIMER0 for LFXO as clock input
    CMU_ClockEnable(cmuClock_LETIMER0, true);                                        //enabling the LETIMER0 clock
*/

  CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);                                   //select ULFRCO
  CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_ULFRCO);                                 //selecting the ULFRCO as the LFA clock
  CMU_ClockEnable(cmuClock_LFA, true);                                               //enabling the LFA clock
  CMU_ClockDivSet(cmuClock_LETIMER0, PRESCALAR_VALUE_ULFRCO);                        //providing the prescalar value for LETIMER0 for ULFRCO as clock input
  CMU_ClockEnable(cmuClock_LETIMER0, true);                                          //enabling the LETIMER0 clock

}
