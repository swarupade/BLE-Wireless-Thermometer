/***********************************************************************************************//**
 * \file   main.c
 * \brief  Silicon Labs Empty Example Project
 *
 * ***************************************************************************************************
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
#include "stdint.h"
#include "stdio.h"
#include "src/main.h"
#include "src/gpio.h"
#include "src/cmu.h"
#include "src/lt.h"
#include "src/I2C.h"
#include "temp.h"
#include "lpm.h"
#include "ustimer.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************

int connection;
//int result;
struct gecko_msg_system_get_bt_address_rsp_t* btaddr;
struct gecko_msg_system_get_bt_address_rsp_t* serveraddr;
char bt_address[64];
char serv_address[64];
char key[32];
char clientaddress[25];
float temp_in_c;
char t[15];

typedef enum {
  disconnected =0,
  discoveringServices,
  discoveringCharacteristics,
  enablingIndication,
  data
} connectionState;

typedef struct
   {
     uint8_t  connectionHandle;
     connectionState connectionstate;
     bd_addr serverBTAddress;
     uint32_t thermServiceHandle;
     uint8array thermServiceUUID;

} connectionproperties;

connectionproperties servers[255] = {0};

uint8array tempemeasureCharacteristic_UUID;
uint16_t tempmeasureCharacteristicHandle = 0;

#define UINT32_TO_FLT(data)    (((float)((int32_t)( data) & 0x00FFFFFFU)) * (float)pow(10, ((int32_t)(data) >> 24)))

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

  uint8_t Index;
  uint8_t tempArr[5];
  // Initialize device
  initMcu();
  // Initialize board
  initBoard();
 // Initialize clocks
  cmu_init();

  // Initialize stack
  gecko_init(&config);
 //initialize LCD
  LCD_init("BLE Client");
 //Initialize LETIMER
  initletimer();
  //Initialize GPIO
  innitgpio();
  //Initialize USTIMER
  USTIMER_Init() ;
  //storing the UUID for temperature measurement characteristic
  tempemeasureCharacteristic_UUID.len = 2;
  tempemeasureCharacteristic_UUID.data[0] = 0x1C;
  tempemeasureCharacteristic_UUID.data[1] = 0x2A;



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
	                	    //security setting
	                        gecko_cmd_sm_configure(0x0f,sm_io_capability_displayyesno);
	                        gecko_cmd_sm_delete_bondings();
	                        gecko_cmd_sm_set_bondable_mode(1);
                            //Displaying the BLE Client's address
	                        btaddr = gecko_cmd_system_get_bt_address();
	                        bd_addr *btaddress = &btaddr->address;
	                        sprintf(bt_address, "%x:%x:%x:%x:%x:%x",
	                       				  btaddress->addr[5],
	                       				  btaddress->addr[4],
	                       				  btaddress->addr[3],
	                       	     		  btaddress->addr[2],
	                       				  btaddress->addr[1],
	                       				  btaddress->addr[0]
	                       			  );
                                  //displaying the bluetooth address on the LCD
	                           	  LCD_write("BT ADDR", LCD_ROW_BTADDR1);

	                       		  LCD_write(bt_address, LCD_ROW_BTADDR2);
	                       	//storing the ble address of the specific server the client will connect to.
	                       	bd_addr serveraddress = {.addr = {0xf0,0x30,0xef,0x57,0x0b,0x00}};
	                       	LCD_write("Connecting...", LCD_ROW_CONNECTION);
	                       	//Connecting to the specific server
	                        gecko_cmd_le_gap_connect(serveraddress, 0 , 1);
	                       break;

	                   case gecko_evt_le_connection_opened_id:

	                	    //getting the connection handle
	                	    connection = evt->data.evt_le_connection_opened.connection;
	                	    Index = connection;
	                	    //Changing the connection state of the client and storing the
	                	    //connection handle and the server address in a structure
	                	    servers[Index].connectionstate = discoveringServices;
	                	    servers[Index].connectionHandle = evt->data.evt_le_connection_opened.connection;
	                	    servers[Index].serverBTAddress = evt->data.evt_le_connection_opened.address;
	                	    //command to increase the security for the connection formed
	                	    gecko_cmd_sm_increase_security(connection);



	                	    LCD_write("Connected", LCD_ROW_CONNECTION);

	                	    //Discover the thermometer service if the devices have already bonded
	                	    if(evt->data.evt_le_connection_opened.bonding != 0xFF)
	                	    {
	                	                //variable to store the uuid for health thermometer service
	                	    	        uint8array thermUUID;
	                	    			thermUUID.data[0] = 0x09;
	                	    			thermUUID.data[1] = 0x18;
	                	    			thermUUID.len = 2 ;
	                	    			//command to search for primary service with the help of a specific uuid
	                	    			gecko_cmd_gatt_discover_primary_services_by_uuid(connection,thermUUID.len,
	                	    					thermUUID.data);
	                	    			LCD_write("disc services" ,LCD_ROW_ACTION);
	                	    }
	                	   break;

	                   case gecko_evt_sm_bonding_failed_id:
                              //in case the bonding fails between the two devices
	                       	  LCD_write("Bonding Failed", LCD_ROW_CONNECTION);
	                       	  LCD_write("", LCD_ROW_PASSKEY);
	                   		  LCD_write("", LCD_ROW_ACTION);

	                       	  break;

	                         case gecko_evt_sm_bonded_id:

	                       	  LCD_write("Bonded", LCD_ROW_CONNECTION);
	                       	  LCD_write("", LCD_ROW_PASSKEY);
	                       	  //LCD_write("", LCD_ROW_ACTION);
	                       	  //searching for the service for the first time after the service has been discovered.
	                       	  uint8array thermUUID;
	                       	  thermUUID.data[0] = 0x09;
	                       	  thermUUID.data[1] = 0x18;
	                       	  thermUUID.len = 2 ;
	                       	  gecko_cmd_gatt_discover_primary_services_by_uuid(connection,thermUUID.len, thermUUID.data);
	                          LCD_write("discovering" ,LCD_ROW_ACTION);
	                       	  break;



	                   case gecko_evt_sm_confirm_passkey_id:


	                	   sprintf(key,"%lu",evt->data.evt_sm_passkey_display.passkey);
                           /*Displaying the passkey required for bonding on the server's LCD .
                            * Waiting for PB0 to get pressed as confirmation that the same passkey is displayed
                            * on the server and the client so bonding can take place
                            */
	                	   while (GPIO_PinInGet(Button_Port, Button_pin )!=0)
	                	   {
	                		   LCD_write(key, LCD_ROW_PASSKEY);
	                		   LCD_write("Press PB0", LCD_ROW_ACTION);
	                	   }
	                	   //Bonding Confirmed
	                	   gecko_cmd_sm_passkey_confirm(evt->data.evt_sm_confirm_bonding.connection, 1 );
	                	   LCD_write("", LCD_ROW_PASSKEY);
	                	   LCD_write("", LCD_ROW_ACTION);


                           break;




	                    case gecko_evt_gatt_service_id:
	                    	/*event is triggered when the service is discovered and the service handle and uuid
	                    	 * is stored
	                         */
                            Index= evt->data.evt_gatt_service.connection;
	                    	servers[Index].thermServiceHandle = evt->data.evt_gatt_service.service;
	                    	servers[Index].thermServiceUUID = evt->data.evt_gatt_service.uuid;
	                    	LCD_write("discovered", LCD_ROW_ACTION);



	                    	break;

	                    case gecko_evt_gatt_procedure_completed_id:
	                    	/*event is triggered whenever any gatt command is executed*/
	                    	 Index=evt->data.evt_gatt_procedure_completed.connection;
	                    	 /*depending on the connection state individual cases will be executed*/
	                    	 switch(servers[Index].connectionstate)
	                    	      {
	                    	          case discoveringServices:
                                            /*case executed after service is discovered, connection state is changed
                                             * and command to discover characteristics is executed
                                             */
	                    	        	  if(evt->data.evt_gatt_procedure_completed.result == 0)
	                    	        	       {
	                    	        		     servers[Index].connectionstate= discoveringCharacteristics;
	                    	        		     gecko_cmd_gatt_discover_characteristics_by_uuid(servers[Index].connectionHandle,
	                    	        		    		 servers[Index].thermServiceHandle,
														 tempemeasureCharacteristic_UUID.len,tempemeasureCharacteristic_UUID.data);
	                    	        		     LCD_write("discovered", LCD_ROW_ACTION);
	                    	        	       }

	                    	        	  break;

	                    	          case discoveringCharacteristics:
                                           /* case executed after the characteristics is discovered, connection state is changed
                                            * and command to enable indication being sent from the remote gatt server is executed
                                            *
                                            */

	                    	        	  if(evt->data.evt_gatt_procedure_completed.result == 0)
	                    	        	 	    {
	                    	        	 	      servers[Index].connectionstate= enablingIndication;
	                    	        	 	      gecko_cmd_gatt_set_characteristic_notification(servers[Index].connectionHandle,
	                    	        	 	    		  tempmeasureCharacteristicHandle,gatt_indication);
	                    	        	 	      LCD_write("characteristic", LCD_ROW_ACTION);

	                    	        	 	    }

	                    	        	  break;

	                    	          case enablingIndication:
                                            /* connection state is changed once the enabling of the indication
                                             * is done.
                                             */
	                    	        	  if(evt->data.evt_gatt_procedure_completed.result == 0)
	                    	        	  	      {
	                    	        	  	        servers[Index].connectionstate= data;

	                    	        	  	        LCD_write("indication", LCD_ROW_ACTION);

	                    	        	  	       }

	                    	        	  break;




	                    	          default:
									  break;

	                    	 }

	                    	break;

	                    	case gecko_evt_gatt_characteristic_id:
	                    		 /*event is called after the desired characteristic is called
	                    		  * value of the characteristic handle is saved.
	                    		  */

	                    		 tempmeasureCharacteristicHandle=evt->data.evt_gatt_characteristic.characteristic;

	                    		 break;

	                    	case gecko_evt_gatt_characteristic_value_id:
	                    		 //event triggered after gatt_characteristic_id

	                    		 if(evt->data.evt_gatt_characteristic_value.characteristic==tempmeasureCharacteristicHandle)
	                    		     {
                                         if(evt->data.evt_gatt_characteristic_value.att_opcode==0x1d)
                                         {
                                        	 //sending acknowledgement after receiving and indication
                                        	 gecko_cmd_gatt_send_characteristic_confirmation(servers[Index].connectionHandle);
                                         }
	                    		     }
                                 /*converting the temperature data from bitstream into a float value and displaying it
                                  * on the LCD.
                                  */
	                    		 memcpy(tempArr,&evt->data.evt_gatt_characteristic_value.value.data[0],5);
	                    		 temp_in_c = UINT32_TO_FLT(*(uint32_t*)&tempArr[1]);
	                    		 sprintf(t, "%4.2f",temp_in_c);
	                    		 LCD_write(t, LCD_ROW_TEMPVALUE);

	                    		break;


	                    case gecko_evt_le_connection_closed_id:

                            //setting the transmission power to zero on disconnecting.
	                         gecko_cmd_system_set_tx_power(0);
	                    	 LCD_write("Disconnected", LCD_ROW_CONNECTION);
	                    	 break;

	                   // case gecko_evt_system_external_signal_id:



	                    default:

	                    	break;



	              }

	       }

}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
