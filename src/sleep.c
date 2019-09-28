

/*
 * sleep.c
 *
 *  Created on: Sep 12, 2018
 *      Author: Swarupa De
 *
 *      @section License * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b> ******************************************************************************* * * Permission is granted to anyone to use this software for any purpose, * including commercial applications, and to alter it and redistribute it * freely, subject to the following restrictions: * * 1. The origin of this software must not be misrepresented; you must not *    claim that you wrote the original software. * 2. Altered source versions must be plainly marked as such, and must not be *    misrepresented as being the original software. * 3. This notice may not be removed or altered from any source distribution. * * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no * obligation to support this Software. Silicon Labs is providing the * Software "AS IS", with no express or implied warranties of any kind, * including, but not limited to, any implied warranties of merchantability * or fitness for any particular purpose or warranties against infringement * of any proprietary rights of a third party. * * Silicon Labs will not be liable for any consequential, incidental, or * special damages, or any other relief, or for any claim by any third party, * arising from your use of this Software. * ******************************************************************************/


//header files
#include "main.h"
#include "sleep.h"
#include "em_core.h"

int sleep_block_counter[5]= {0,0,0,0,0};

void blocksleepmode(int mode)
{

	CORE_ATOMIC_IRQ_DISABLE();
	sleep_block_counter[mode]++;
	CORE_ATOMIC_IRQ_ENABLE();

}

void unblocksleepmode(int mode)
{
	CORE_ATOMIC_IRQ_DISABLE();
	if(sleep_block_counter[mode] > 0)
       sleep_block_counter[mode]--;
	CORE_ATOMIC_IRQ_ENABLE();
}



void sleep(void)
	     {



	     if (sleep_block_counter[0] > 0)
	     {
	 				  return;
	     }


	     else if (sleep_block_counter[1] > 0)
	     {
	    	 return;
	     }

	     else if (sleep_block_counter[2] > 0)
	     {
	    	 EMU_EnterEM1();
	    	 return;
	     }


	     else if (sleep_block_counter[3] > 0)
	     {
	    	 EMU_EnterEM2(true);
	    	 return;
	     }


	     else
	     {
	    	 EMU_EnterEM3(true);
	    	 return;
	     }




         }


