//***********************************************************************************
// Include files
//***********************************************************************************
#include "src/cmu.h"
#include "main.h"


// Macro



//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
void cmu_init(void)

  {


				    CMU_OscillatorEnable(cmuOsc_LFXO, true, true);

		            CMU_OscillatorEnable(cmuOsc_ULFRCO , true, true);

		            CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
				    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);
			        CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);
				    CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);

					CMU_ClockEnable(cmuClock_GPIO,true);
					CMU_ClockEnable(cmuClock_LETIMER0,true);
					CMU_ClockEnable(cmuClock_LFA, true);
					CMU_ClockEnable(cmuClock_CORELE, true);
					CMU_ClockEnable(cmuClock_HFPER, true);
					CMU_ClockEnable(cmuClock_I2C0,true );
					CMU_ClockEnable(cmuClock_TIMER0, true);

					// Interface clocks enabled



}

