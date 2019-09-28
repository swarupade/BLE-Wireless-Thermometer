/***********************************************************************************************//**
 * \file   main.c
 * \brief  Silicon Labs Empty Example Project
 *
 * Server Side Code for BLE ***************************************************************************************************
 * <b> (C) Copyright 2016 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

/* Board headers */
#include "init_mcu.h"
#include "init_board.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "infrastructure.h"

/* GATT database */
#include "gatt_db.h"

/* App headers */
#include "init_app.h"

/* Libraries containing default Gecko configuration values */
#include "em_system.h"
#include "em_emu.h"
#include "em_cmu.h"

/* Device initialization header */
#include "hal-config.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

// Gecko configuration parameters (see gecko_configuration.h)
static const gecko_configuration_t config = {
  .config_flags = 0,
  .sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap),
  .bluetooth.sleep_clock_accuracy = 100, // ppm
  .gattdb = &bg_gattdb_data,
  .ota.flags = 0,
  .ota.device_name_len = 3,
  .ota.device_name_ptr = "OTA",

  #if (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
  .pa.config_enable = 1, // Enable high power PA
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
};


/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/





//***********************************************************************************
// Include files
//***********************************************************************************

#include "src/main.h"
#include "src/gpio.h"
#include "src/cmu.h"
#include "src/lt.h"
#include "src/I2C.h"
#include "temp.h"
#include "lpm.h"
#include "ustimer.h"
//#include "ustimer.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************

int connection;
int result;
//***********************************************************************************
// function prototypes
//***********************************************************************************


//***********************************************************************************
// functions
//***********************************************************************************


//***********************************************************************************
// main
//***********************************************************************************

/**
 * @brief  Main function
 */
int main(void)
{


  // Initialize device
  initMcu();
  // Initialize board
  initBoard();



  // Initialize clocks
  cmu_init();

  // Initialize stack
  gecko_init(&config);

  initletimer();

  USTIMER_Init() ;





	  while (1)
	       {


	         /* Event pointer for handling events */
	         struct gecko_cmd_packet* evt;

      	     /* Check for stack event. */
	         evt = gecko_wait_event();

	          /* Handle events */
	         switch (BGLIB_MSG_ID(evt->header))
	              {
	                 /* This boot event is generated when the system boots up after reset.
	                  * Do not call any stack commands before receiving the boot event.
	                  * Here the system is set to start advertising immediately after boot procedure. */
	                   case gecko_evt_system_boot_id:
	                       /* Set advertising parameters. 100ms advertisement interval.
	                        * The first two parameters are minimum and maximum advertising interval, both in
	                        * units of (milliseconds * 0.625). */
	                       gecko_cmd_le_gap_set_adv_parameters(400, 400, 7);

	                         /* Start general advertising and enable connections. */
	                       gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
	                       break;

	                   case gecko_evt_le_connection_opened_id:

	                	   connection = evt->data.evt_le_connection_opened.connection;
	                	   gecko_cmd_le_connection_set_parameters(connection, 0x003C, 0x003C, 0x0003, 0x1F4 );
	                	   result =  evt->data.rsp_le_connection_disable_slave_latency.result;
	                	   break;




	                    /* This event is generated when a connected client has either
	                     * 1) changed a Characteristic Client Configuration, meaning that they have enabled
	                        * or disabled Notifications or Indications, or
	                        * 2) sent a confirmation upon a successful reception of the indication. */
	                    case gecko_evt_gatt_server_characteristic_status_id:
	                      /* Check that the characteristic in question is temperature - its ID is defined
	                       * in gatt.xml as "temperature_measurement". Also check that status_flags = 1, meaning that
	                       * the characteristic client configuration was changed (notifications or indications
	                       * enabled or disabled). */
	                        if ((evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_temperature_measurement_3)
	                        && (evt->data.evt_gatt_server_characteristic_status.status_flags == 0x01))
	                        {
//	                        	gecko_cmd_le_connection_get_rssi(connection);
	                        }
//	                        gecko_cmd_le_connection_get_rssi(connection);
	                        break;

	                    case gecko_evt_le_connection_closed_id:


	                         /* Restart advertising after client has disconnected */
	                    	 gecko_cmd_system_set_tx_power(0);
	                          gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);

	                          break;

	                    case gecko_evt_system_external_signal_id:


	                    	innitgpio();
	                    	init_i2c();
	                    	tempcalc();s
	                    	disable();

	                    	gecko_cmd_le_connection_get_rssi(connection);



	                    	break;

	                    case gecko_evt_le_connection_rssi_id:

	                    	  	  if (evt->data.evt_le_connection_rssi.rssi > -35 )
	                    	  	      {
	                    	  		gecko_cmd_system_set_tx_power(-260);	//setting TX power to min (-26 dbm) when rssi more than -35 dB

	                    	  	      }
	                    	  	  else if (evt->data.evt_le_connection_rssi.rssi > -45 && evt->data.evt_le_connection_rssi.rssi <-35)
	                    	  	      {
	                    	  		gecko_cmd_system_set_tx_power(-200);

	                    	  	      }
	                    	  	  else if(evt->data.evt_le_connection_rssi.rssi > -55 && evt->data.evt_le_connection_rssi.rssi <-45)
	                    	  	      {

	                    	  		gecko_cmd_system_set_tx_power(-150);
	                    	  	      }
	                    	  	  else if (evt->data.evt_le_connection_rssi.rssi >-65 && evt->data.evt_le_connection_rssi.rssi< -55)
	                    	  	      {
	                    	  		gecko_cmd_system_set_tx_power(-50);
	                    	  	      }
	                    	  	  else if (evt->data.evt_le_connection_rssi.rssi >-75 && evt->data.evt_le_connection_rssi.rssi <-65)
	                    	  	      {

	                    	  		gecko_cmd_system_set_tx_power(0);

	                    	  	      }
	                    	  	  else if (evt->data.evt_le_connection_rssi.rssi >-85 && evt->data.evt_le_connection_rssi.rssi < -75)
	                    	  	      {

	                    	  		gecko_cmd_system_set_tx_power(50);

	                    	  	      }
	                    	  	  else
	                    	  	      {
	                    	  		gecko_cmd_system_set_tx_power(80); //setting TX power to max (8 dbm) when rssi less than -85 dB
	                    	  	      }

	                    	  break;


	                    default:

	                    	break;



	              }

	       }

}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
