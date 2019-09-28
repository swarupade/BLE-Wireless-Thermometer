/*
 * I2C.h
 *
 *  Created on: Sep 20, 2018
 *      Author: Swarupa De
 */

#ifndef I2C_H_
#define I2C_H_
//#include "src/cmu.h"
//#include "main.h"
#include "em_i2c.h"
//#include "gpio.h"


#define I2C_DEVICE_ADDR 0x40

void init_i2c(void);
uint16_t i2c_driver1();




#endif /* I2C_H_ */



