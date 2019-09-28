/*
 * gpio.c
 *
 *  Created on: Sep 13, 2018
 *      Author: Swarupa De
 */

#include"src/gpio.h"


void innitgpio(void)
{


	GPIO_PinModeSet(gpioPortC, 11,gpioModeWiredAnd, 1);
	GPIO_PinModeSet(gpioPortC, 10,gpioModeWiredAnd, 1);

	//GPIO_PinModeSet(gpioPortF, 4,gpioModePushPull, 1);
	//GPIO_PinModeSet(gpioPortF, 4,gpioModePushPull, 1);



	for (int i=0;i<9;i++) //reset the I2C slave
	{
		GPIO_PinOutClear(gpioPortC, 11);
	    GPIO_PinOutSet(gpioPortC, 11);
	}



	GPIO_PinModeSet(gpioPortF, 4, gpioModePushPull, false);
	GPIO_DriveStrengthSet(gpioPortF, gpioDriveStrengthWeakAlternateWeak);



}


