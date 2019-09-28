
#include "I2C.h"
#include "gpio.h"
#include "sleep.h"
#include "ustimer.h"



void init_i2c(void)

{
	//blocksleepmode(EM2);

	const I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

	I2C0->ROUTEPEN |= I2C_ROUTEPEN_SDAPEN|I2C_ROUTEPEN_SCLPEN;
  	I2C0->ROUTELOC0|= ((I2C0->ROUTELOC0 & (~_I2C_ROUTELOC0_SCLLOC_MASK))|I2C_ROUTELOC0_SCLLOC_LOC14);
  	I2C0->ROUTELOC0|= ((I2C0->ROUTELOC0 & (~_I2C_ROUTELOC0_SDALOC_MASK))|I2C_ROUTELOC0_SDALOC_LOC16);

  	I2C_Init(I2C0, &i2cInit);

  	if (I2C0->STATE & I2C_STATE_BUSY)
  	  {
  		I2C0->CMD = I2C_CMD_ABORT;

  	  }
  	I2C_Enable(I2C0, true);

  	GPIO_PinModeSet(gpioPortD,15,gpioModePushPull, 1);
  	GPIO_PinOutSet(gpioPortD,15);
  	for(int i=0; i<150000; i++);
  	USTIMER_Delay(100);
    //TIMER0->CMD = TIMER_CMD_START;





}


uint16_t  i2c_driver1()

{

	     uint16_t value1;

	     I2C0->TXDATA = ((I2C_DEVICE_ADDR << 1 )|(0x00));

         I2C0->CMD = I2C_CMD_START;


	  	 while ((I2C0->IF & I2C_IF_ACK) ==  0);

	  	 I2C0->IFC = I2C_IFC_ACK;

	  	 I2C0->TXDATA = 0xE3 ;

	  	 while ((I2C0->IF & I2C_IF_ACK) ==  0);

	  	 I2C0->IFC = I2C_IFC_ACK;

	  	 I2C0->CMD = I2C_CMD_START;

	  	 I2C0->TXDATA = ((I2C_DEVICE_ADDR <<1)|0x01);

	  	 while ((I2C0->IF & I2C_IF_ACK) ==  0);
	  	 I2C0->IFC = I2C_IFC_ACK;

	  	// I2C_Enable(I2C0,I2C_CTRL_AUTOACK);
         while ((I2C0->IF  & I2C_IF_RXDATAV)==0);
         value1 = I2C0->RXDATA;
         value1= value1 << 8;

	     I2C0->CMD= I2C_CMD_ACK;




	    // while ((I2C0->IF & I2C_IF_ACK) ==  0);
	     while ((I2C0->IF  & I2C_IF_RXDATAV)==0);
	     value1 |= I2C0->RXDATA;

//	     I2C0->CMD= I2C_CMD_ACK;
	     //I2C0->IFC = I2C_IFC_ACK;

         I2C0->CMD = I2C_CMD_NACK;

	  	 I2C0->CMD = I2C_CMD_STOP;

	  	 return (value1);


}
