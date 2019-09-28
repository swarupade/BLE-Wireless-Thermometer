/*
 * temp.c
 *
 *  Created on: Sep 21, 2018
 *      Author: Swarupa De
 */
#include "gatt_db.h"
#include "temp.h"
#include "src/gpio.h"
#include "infrastructure.h"
#include "native_gecko.h"
#include "src/main.h"


void tempcalc(void)
{
   uint32_t value = i2c_driver1();
   tempinc = (((175.72* value )/ 65536)- 46.85);
   if (tempinc < MINTEMP)
   GPIO_PinOutSet(gpioPortF, 5);
   uint8_t Temperature_buffer[5]; /*  used to store the temperature measured from the sensor in the Health Thermometer (HTM) format. */
   uint8_t flags = 0x00;   /* HTM flags set as 0 for Celsius, no time stamp and no temperature type. */
   uint32_t temperature; /*variable for storing the value of temperature in the appropriate format.*/
   uint8_t *p = Temperature_buffer; /* Pointer to HTM temperature buffer needed for converting values to bitstream. */

    /* Convert flags to bitstream and append them in the HTM temperature data buffer (Temperature_buffer) */
    UINT8_TO_BITSTREAM(p, flags);
      /* Convert sensor data to correct temperature format */
      temperature = FLT_TO_UINT32(tempinc, 0);
      /* Convert temperature to bitstream and place it in the HTM temperature data buffer (htmTempBuffer) */
      UINT32_TO_BITSTREAM(p, temperature);
      /* Send indication of the temperature in htmTempBuffer to all "listening" clients.
       * This enables the Health Thermometer in the Blue Gecko app to display the temperature.
       *  0xFF as connection ID will send indications to all connections. */
     gecko_cmd_gatt_server_send_characteristic_notification(0xFF, gattdb_temperature_measurement_3, 5, Temperature_buffer);
}
