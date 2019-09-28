/*
 * lpm.c
 *
 *  Created on: Sep 22, 2018
 *      Author: Swarupa De
 *
 */
#include "lpm.h"
#include "src/gpio.h"


void disable (void)
{
	I2C_Enable(I2C0, false);
	GPIO_PinModeSet(gpioPortD,15,gpioModeDisabled, 0);
	GPIO_PinModeSet(gpioPortC, 11,gpioModeDisabled, 0);
	GPIO_PinModeSet(gpioPortC, 10,gpioModeDisabled, 0);

}

