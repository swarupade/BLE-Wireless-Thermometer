/*
 * sleep.h
 *
 *  Created on: Sep 13, 2018
 *      Author: Swarupa De
 */

#include "main.h"
#include "em_cmu.h"
#include "em_emu.h"

#ifndef SLEEP_H_
#define SLEEP_H_

void blocksleep(int energymode);

void unblocksleep(int energymode);

void sleep(void);



#endif /* SLEEP_H_ */

//global variable

//function prototype

