/*
 * irq.c
 * This file contains function prototype defining the functionality in interrupt handler
 *  Created on: Sep 9, 2021
 *      Author: mich1576
 */

#include "irq.h"

/* function       : LETIMER0_IRQHandler
 * params         : void
 * brief          : check for any interrupts, turn the LED on and off accordingly and reset the interrupt flags.
 * return type    : void
 */
void LETIMER0_IRQHandler (void) {

    uint32_t flags;
        // determine source of IRQ
        flags = LETIMER_IntGetEnabled(LETIMER0);
        // clear source of IRQ set in step 3
        LETIMER_IntClear(LETIMER0, flags);

        if(flags & LETIMER_IF_UF){
            //everytme there is an underflow interrupt turn the led on
            gpioLed0SetOn();
            //reset the COMP0 and COMP1 registers for continuous operation
#if ((LOWEST_ENERGY_MODE == 0) || (LOWEST_ENERGY_MODE == 1) || (LOWEST_ENERGY_MODE == 2))
              LETIMER_CompareSet(LETIMER0, 0, VALUE_TO_LOAD_LFXO_COMP0);
#elif(LOWEST_ENERGY_MODE == 3)
              LETIMER_CompareSet(LETIMER0, 0, VALUE_TO_LOAD_ULFRCO_COMP0);
#endif
        }

        if(flags & LETIMER_IF_COMP1){
            //everytme there is a COMP1 interrupt turn the led off
            gpioLed0SetOff();
            //reset the COMP0 and COMP1 registers for continuous operation
#if ((LOWEST_ENERGY_MODE == 0) || (LOWEST_ENERGY_MODE == 1) || (LOWEST_ENERGY_MODE == 2))
                LETIMER_CompareSet(LETIMER0, 1, VALUE_TO_LOAD_LFXO_COMP1);
#elif(LOWEST_ENERGY_MODE == 3)
                LETIMER_CompareSet(LETIMER0, 1, VALUE_TO_LOAD_ULFRCO_COMP1);
#endif
        }

} // LETIMER0_IRQHandler()

