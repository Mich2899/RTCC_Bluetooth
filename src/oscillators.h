/*
 * oscillators.h
 * This file contains function prototype for oscillator initialization.Check .c file for function definition.
 *  Created on: Sep 9, 2021
 *      Author: mich1576
 */

#ifndef SRC_OSCILLATORS_H_
#define SRC_OSCILLATORS_H_

#include "em_cmu.h"
#include "app.h"
#define PRESCALAR_VALUE_LFXO    4                      //Prescalar value for LFXO
#define PRESCALAR_VALUE_ULFRCO  1                      //Prescalar value for ULFRCO

#define FREQUENCY_LFXO          32768                  //Frequency of LFXO clock
#define FREQUENCY_ULFRCO        1000                   //Frequency of ULFRCO clock

#define ACTUAL_CLK_FREQ_LFXO        (FREQUENCY_LFXO/PRESCALAR_VALUE_LFXO)         //clock frequency after pre-scaling for LFXO
#define ACTUAL_CLK_FREQ_ULFRCO      (FREQUENCY_ULFRCO/PRESCALAR_VALUE_ULFRCO)     //clock frequency after pre-scaling for ULFRCO

#define VALUE_TO_LOAD_LFXO_COMP0    (ACTUAL_CLK_FREQ_LFXO *LETIMER_PERIOD_MS)/1000  //value to load in comp0 when LFXO is used
#define VALUE_TO_LOAD_LFXO_COMP1    (ACTUAL_CLK_FREQ_LFXO *(LETIMER_PERIOD_MS-LETIMER_ON_TIME_MS))/1000 //value to load in comp1 when LFXO is used

#define VALUE_TO_LOAD_ULFRCO_COMP0  (ACTUAL_CLK_FREQ_ULFRCO *LETIMER_PERIOD_MS)/1000       //value to load in comp0 when ULFRCO is used
#define VALUE_TO_LOAD_ULFRCO_COMP1  (ACTUAL_CLK_FREQ_ULFRCO *(LETIMER_PERIOD_MS-LETIMER_ON_TIME_MS))/1000 //value to load in comp1 when ULFRCO is used

//function prototypes
void init_oscillators();

#endif /* SRC_OSCILLATORS_H_ */
