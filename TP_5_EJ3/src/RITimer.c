/*
 * RIT.c
 *
 *  Created on: 7 may. 2022
 *      Author: valkur
 */

#include "chip.h"

void RIT_init(void){
Chip_RIT_Init(LPC_RITIMER);
Chip_RIT_Enable(LPC_RITIMER);
NVIC_EnableIRQ(RITIMER_IRQn);

}

void RIT_set(uint32_t ms){
        Chip_RIT_SetTimerInterval(LPC_RITIMER, ms);
}

void RIT_clean(void){
        Chip_RIT_ClearInt(LPC_RITIMER);
}



