/*
 * lt.h
 *
 *  Created on: Sep 11, 2018
 *      Author: Swarupa De
 */
//include files

#include "src/main.h"
#include "src/cmu.h"
#include "em_letimer.h"
//#include "efr32bg13p_letimer.h"

//#ifndef LT_H_
//#define LT_H_

void initletimer(void);
//void  callletimer(void);
void LETIMER0_IRQHandler(void);



uint16_t schedule_event;
uint16_t event1;
//
//

//
//#endif /* LT_H_ */

//function prototype




