/*
 * irq.c
 * This file contains function prototype defining the functionality in interrupt handler
 *  Created on: Sep 9, 2021
 *      Author: mich1576
 */

/*********************************************************INCLUDES****************************************************************/
#include "irq.h"

/****************************************************FUNCTION DEFINITION*********************************************************/
//Check for any interrupts, turn the LED on and off accordingly and reset the interrupt flags.
void LETIMER0_IRQHandler (void) {

    uint32_t flags;
        // determine source of IRQ
        flags = LETIMER_IntGetEnabled(LETIMER0);
        // clear source of IRQ set in step 3
        LETIMER_IntClear(LETIMER0, flags);

        if(flags & LETIMER_IF_UF){
            //call scheduler to read temperature data from SI7021
            schedulerSetReadTempEvent();
        }

} // LETIMER0_IRQHandler()

