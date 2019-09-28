/*
 * lt.c
 *
 *  Created on: Sep 11, 2018
 *      Author: Swarupa De
 *
 *
 */

#include "em_emu.h"
#include "em_core.h"
#include "em_cmu.h"
#include "native_gecko.h"


#include "src/main.h"
#include "src/lt.h"
#include "src/gpio.h"
#include "src/sleep.h"
#include "efr32bg13p632f512gm48.h"
#include "I2C.h"

long int freq;




void initletimer(void)

{
	  //blocksleepmode(EM4);


	   CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

	       /* if (EMX <=3)
	  		    {
	  	      		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
	  	      		freq = 32768; // Frequency of the clock supplied to the timer will vary according to the sleep modes

	  	      	 }
	  		    else
	  		    */

	  		    	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);
	  		    	freq = 1000;



	   LETIMER_Init_TypeDef letimerInit=
	 	   	{
	 	   		.enable = false,         /**< Start counting when initialization comple enable*/
	 	   		.debugRun= false,       /**< Counter shall keep running during debug halt. */

	 	   		.comp0Top= true,       /**< Load COMP0 register into CNT when counter underflows. */
	 	   		.bufTop = false,         /**< Load COMP1 into COMP0 when REP0 reaches 0. */
	 	   		.out0Pol= 0,       /**< Idle value for output 0. */
	 	   		.out1Pol=0,        /**< Idle value for output 1. */
	 	   		.ufoa0 = letimerUFOANone,          /**< Underflow output 0 action. */
	 	   		.ufoa1 = letimerUFOANone,          /**< Underflow output 1 action. */
	 	   		.repMode =letimerRepeatFree,        /**< Repeat mode. */
	 	   	};

	 LETIMER_Init(LETIMER0, &letimerInit );

	 LETIMER_Enable(LETIMER0, true);




	 long int maxtick = freq * period;

	 int i = 0;

	   while ( largestcount <= maxtick )
          {

            (freq) = (freq) >> 1;
		     i++;
		    maxtick = freq * period;
		  }

	  CMU->LFAPRESC0 =i;






      int  COMPVAL1=  (period)*(freq);
      //uint32_t COMPVAL2 = (period - timeon)*(freq);

      LETIMER_CompareSet(LETIMER0, 0, COMPVAL1);

      while (LETIMER0->SYNCBUSY);


      LETIMER_IntClear(LETIMER0, LETIMER_IFC_UF);
      LETIMER_IntClear(LETIMER0, LETIMER_IFC_COMP0);



      //LETIMER_IntEnable(LETIMER0, LETIMER_IEN_COMP0);
      LETIMER_IntEnable(LETIMER0, LETIMER_IEN_UF);

      //LETIMER_CompareSet(LETIMER0, 1, COMPVAL2);








   	  NVIC_EnableIRQ(LETIMER0_IRQn);

      LETIMER0->CMD = LETIMER_CMD_START;




}


void LETIMER0_IRQHandler(void)

      {
	        CORE_ATOMIC_IRQ_DISABLE();
	        int flags = LETIMER_IntGet(LETIMER0);
	        LETIMER_IntClear(LETIMER0, flags);

	        if (LETIMER_IF_UF & flags )
	           {

	        	gecko_external_signal(LETIMER_IF_UF);


	           }

          LETIMER_IntClear(LETIMER0, LETIMER_IFC_UF);


          CORE_ATOMIC_IRQ_ENABLE();





      }





