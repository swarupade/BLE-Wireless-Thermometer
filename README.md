# BLE-Wireless-Thermometer
## Project Goal : To take the temperature measured by the Si7021 included on the Blue Gecko Board and communicate it via BLE to the BLE Client. 

### Includes Code for BLE Server Side as well as BLE Client. 

#### BLE Server :
----------------------------------------------------------------------------------------------------------------------------
#### BLE Services Used For the Server:

1. Health Thermometer 
   Characteristics:
    a. Temperature Measurement
    b. Temperature Type
2. Tx Power
  Characteristics:
    a. Tx Power Level
    
##### To maximize energy savings, the Bluetooth application should change its advertising, connection interval, and slave interval to
what is appropriate to the application.
a. The Advertising min and max is 250mS
b. Connection Interval minimum and maximum is 75mS
c. Slave latency to enable it to be off the “air” is up to 300mS

##### To further maximize energy savings, the application should automatically adjust its transmit power based on the proximity of the master / client, the phone. 
Use the following settings:
a. If rssi > -35db, set tx_power to BGM121 TX Min i.e. -26db
b. If -35db > rssi > -45db, tx_power is -20db
c. If -45db > rssi > -55db, tx_power is -15db
d. If -55db > rssi > -65 db, tx_power is -5db
e. If -65db > rssi > -75db, tx_power is 0db
f. If -75db > rssi > -85db, tx_power is 5db
g. If rssi < -85db, tx_power is BGM121 TX Max i.e. 8db
h. Upon reset or a Bluetooth connection disconnect, TX Power is set to 0db

##### LETIMER0 is set to the following conditions at startup / reset.
a. Period = 1.75 seconds 
b. No period, so no need to have the LETIMER0 to interrupt twice per period 
c. During the LETIMER0 period interrupt, it will request, receive, and process the temperature reading from the Si7021

#### Si7021 I2C temp sensor (Used I2C for interfacing the sensor):
a. It should be running at the lowest energy possible while enabled and while taking temperature measurements
b. Temperature measurements should be 14-bit and calculated in degrees C
c. The default temperature should be 15C
d. If the temperature read from the Si7021 is below the set temperature, LED1 is latched on until cleared by pressing the joy stick button down
e. The temperature set point should increase by 5 degrees C every time the joy stick is pressed to the right
f. The temperature set point should decrease by 5 degrees C every time the joy stick is pressed to the left
g. If LED1 is turned off by pressing the joy stick button down(centre) and the temperature is still below the current set point, LED1 should turn on at the next temperature measurement
h. The temperature set point is not reset when the joy stick button is pressed down. The only function pressing the joy stick down should be to turn off LED1.
