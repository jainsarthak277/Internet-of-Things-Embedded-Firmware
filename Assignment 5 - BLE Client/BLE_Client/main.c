
/***********************************************************************************************//**
 * \file   main.c
 * \brief  Silicon Labs Empty Example Project
 *
 * This example demonstrates the bare minimum needed for a Blue Gecko C application
 * that allows Over-the-Air Device Firmware Upgrading (OTA DFU). The application
 * starts advertising after boot and restarts advertising after a connection is closed.
 ***************************************************************************************************
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
#include "infrastructure.h"
#include "lcd_driver.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"

/* Libraries containing default Gecko configuration values */
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

// Flag for indicating DFU Reset must be performed
uint8_t boot_to_dfu = 0;
//***********************************************************************************
// Include files
//***********************************************************************************

#include "src/main.h"
#include "src/gpio.h"
#include "src/cmu.h"
#include "src/letimer0.h"
#include "src/i2c0_tempsense.h"
#define UINT32_TO_FLT(data)       \
	(((float)((int32_t)(data) & 0x00FFFFFFU)) * (float)pow(10, ((int32_t)(data) >> 24)))
#define FLAGS 0x0F
#define SCAN_INTERVAL 16
#define SCAN_WINDOW 16
#define PHYS 5
#define SCAN_TYPE 1
#define CONFIRM_TYPE 1
#define UUID_LEN 2
int connection;

//***********************************************************************************
// defined files
//***********************************************************************************

//***********************************************************************************
// global variables
//***********************************************************************************

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

  // Initialize GPIO
  gpio_init();

  // Initialize stack
  gecko_init(&config);

  // Initialize clocks
  cmu_init();

  //Initialize LETIMER
  letimer_setup();

  LCD_init("BLE Client");
  bd_addr server_addr = {.addr = {0xb6,0x80,0xf1,0x57,0x0b,0x00}};	//Jajoo board
//  bd_addr server_addr = {.addr = {0xea,0x2f,0xef,0x57,0x0b,0x00}};	//Ayush board

  //Define variable for keeping track of scheduler
  schedule_event = 0;
  const uint8_t thermometer_service[2] = {0x09, 0x18};
  const uint8_t thermometer_characteristic[2] = {0x1c, 0x2a};
  serv = 0;
  uint8_t tempArr[5] = {0,0,0,0,0};

  //Initializing variables

  while (1) {
//	      Event pointer for handling events
	      struct gecko_cmd_packet* evt;

	      /* Check for stack event. */
	      evt = gecko_wait_event();

	      /* Handle events */
	      switch (BGLIB_MSG_ID(evt->header)) {
	        /* This boot event is generated when the system boots up after reset.
	         * Do not call any stack commands before receiving the boot event.
	         * Here the system is set to start advertising immediately after boot procedure. */
	        case gecko_evt_system_boot_id:
	          /* Set advertising parameters. 100ms advertisement interval.
	           * The first two parameters are minimum and maximum advertising interval, both in
	           * units of (milliseconds * 1.6). */
	          gecko_cmd_sm_delete_bondings();
	          bonding_var = 0;
	          gecko_cmd_sm_configure(FLAGS,sm_io_capability_displayyesno);
	          gecko_cmd_sm_set_bondable_mode(1);
	          gecko_cmd_le_gap_set_discovery_timing(PHYS, SCAN_INTERVAL, SCAN_WINDOW);
	          gecko_cmd_le_gap_set_discovery_type(PHYS, SCAN_TYPE);

	          bt_addr_t = gecko_cmd_system_get_bt_address();
	          bd_addr *addr_f = &bt_addr_t->address;
	          sprintf(bt_addr_string, "%x:%x:%x:%x:%x:%x", addr_f->addr[5], addr_f->addr[4], addr_f->addr[3], addr_f->addr[2], addr_f->addr[1], addr_f->addr[0]);
	          LCD_write("BT ADDR", LCD_ROW_BTADDR1);
	          LCD_write(bt_addr_string, LCD_ROW_BTADDR2);

	          /*Connect to target server with address provided*/
	          gecko_cmd_le_gap_connect(server_addr,le_gap_address_type_public,0x01);
	          break;

	        case gecko_evt_le_connection_opened_id:
	        	        	connection = evt->data.evt_le_connection_opened.connection;
	        	        	gecko_cmd_le_connection_set_parameters(evt->data.evt_le_connection_opened.connection,60,60,3,400);
	        	        	/*Only connected if bonding_var variable equals 0*/
	        	        	if(bonding_var == 0)
	        	        		LCD_write("CONNECTED", LCD_ROW_CONNECTION);
	        	        	else if (bonding_var == 1)
	        	        		LCD_write("CONNECTED, BONDED", LCD_ROW_CONNECTION);
//	        	        	LCD_write("Server- X:X:X:X:80:b6", LCD_ROW_CLIENTADDR);		//Jajoo board
	        	        	LCD_write("SERVER- X:X:X:X:2f:ea", LCD_ROW_CLIENTADDR);		//Ayush board
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
	       	  if ((evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_temperature_measurement)
	              && (evt->data.evt_gatt_server_characteristic_status.status_flags == 0x01)) {
	            /*if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == 0x02) {
	            	 Indications have been turned ON - start the repeating timer. The 1st parameter '32768'
	               * tells the timer to run for 1 second (32.768 kHz oscillator), the 2nd parameter is
	               * the timer handle and the 3rd parameter '0' tells the timer to repeat continuously until
	               * stopped manually.*/
	       		  gecko_cmd_le_connection_get_rssi(connection);
	       	  }
	          break;

	        case gecko_evt_sm_confirm_bonding_id:
	        	LCD_write("PRESS PB0 TO PAIR", LCD_ROW_ACTION);
	        	/*Wait for button press to pair with server*/
	        	while(GPIO_PinInGet(gpioPortF, 6) != 0);
	        	LCD_write("", LCD_ROW_ACTION);
	        	/*Enter passkey id event if confirmed bonding*/
	        	gecko_cmd_sm_bonding_confirm(evt->data.evt_sm_confirm_bonding.connection, CONFIRM_TYPE);
	        	break;

	        case gecko_evt_sm_confirm_passkey_id:
	        	sprintf(passkey, "KEY: %d", evt->data.evt_sm_passkey_display.passkey);
	        	LCD_write(passkey, LCD_ROW_PASSKEY);
	        	LCD_write("PRESS PB0 TO PROCEED", LCD_ROW_ACTION);
	        	/*Wait for button press to bond with server*/
	        	while(GPIO_PinInGet(gpioPortF, 6) != 0);
	        	LCD_write("", LCD_ROW_PASSKEY);
	        	LCD_write("", LCD_ROW_ACTION);
	        	gecko_cmd_sm_passkey_confirm(evt->data.evt_sm_confirm_bonding.connection, CONFIRM_TYPE);
	        	break;

	        case gecko_evt_sm_bonded_id:
	        	bonding_var = 1;
	        	LCD_write("CONNECTED, BONDED", LCD_ROW_CONNECTION);
	        	/*discover primary services with UUID provided*/
	        	gecko_cmd_gatt_discover_primary_services_by_uuid(evt->data.evt_sm_bonded.connection, UUID_LEN,
	        													(const uint8_t*)thermometer_service);
	        	break;

	        case gecko_evt_sm_bonding_failed_id:
	        	LCD_write("BONDING FAILED", LCD_ROW_CONNECTION);
	        	/*Close connection if bonding failed*/
	        	gecko_cmd_le_connection_close(evt->data.evt_sm_bonding_failed.connection);
	        	break;

	        case gecko_evt_gatt_service_id:
//	        	LCD_write("SERVICED", LCD_ROW_PASSKEY);
	        	serv = 1;
	        	service_handle = evt->data.evt_gatt_service.service;
	        	break;

	        case gecko_evt_gatt_characteristic_id:
//	        	LCD_write("CHARACTERIZED", LCD_ROW_PASSKEY);
	        	serv = 2;
	        	characteristic_handle = evt->data.evt_gatt_characteristic.characteristic;
	        	break;

	        case gecko_evt_gatt_procedure_completed_id:
	        	if(serv ==1)
	        	{
	        		serv = 0;
//	        		LCD_write("PROCESSED", LCD_ROW_ACTION);
	        		/*discover characteristics with UUID provided*/
	        		gecko_cmd_gatt_discover_characteristics_by_uuid(evt->data.evt_gatt_procedure_completed.connection,
	        														service_handle, UUID_LEN,
	        			        									(const uint8_t*)thermometer_characteristic);
	        	}
	        	if(serv ==2)
	        	{
	        		serv = 0;
//	        		LCD_write("PROCESSED AGAIN", LCD_ROW_ACTION);
	        		/*Set characteristic notification if characteristics discovered*/
	                gecko_cmd_gatt_set_characteristic_notification(evt->data.evt_gatt_procedure_completed.connection,
	                												characteristic_handle, 2);
	        	}
	        	break;

	        case gecko_evt_gatt_characteristic_value_id:
	        	/*Send confirmation after receiving data*/
	       		gecko_cmd_gatt_send_characteristic_confirmation(evt->data.evt_gatt_characteristic_value.connection);
	        	tempArrptr = &evt->data.evt_gatt_characteristic_value.value.data[0];
	        	memcpy(tempArr, tempArrptr, 5);
	        	t = *((uint32_t *) &tempArr[1]);
	        	midt = (float)UINT32_TO_FLT(t);
	        	sprintf(lcdtemp, "TEMPERATURE: %.2f C", midt);
	        	LCD_write(lcdtemp, LCD_ROW_TEMPVALUE);
	        	if (midt < THRESHOLD)	//Threshold value = 15 degrees Celsius
	        		{
	        			GPIO_PinOutSet(LED1_port, LED1_pin);
	        		}
	        	break;

	        case gecko_evt_le_connection_closed_id:
	          /* Check if need to boot to dfu mode */
	          if (boot_to_dfu) {
	            /* Enter to DFU OTA mode */
	            gecko_cmd_system_reset(2);
	          } else {
	            /* Stop timer in case client disconnected before indications were turned off */
	            gecko_cmd_hardware_set_soft_timer(0, 0, 0);
	            /* Restart advertising after client has disconnected */
	            gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable);
	          }
	          gecko_cmd_system_set_tx_power(0);
	          LCD_write("DISCONNECTED", LCD_ROW_CONNECTION);
	          LCD_write("", LCD_ROW_PASSKEY);
	          LCD_write("", LCD_ROW_TEMPVALUE);
	          gecko_cmd_le_gap_connect(server_addr,le_gap_address_type_public,0x01);
	          break;

	        case gecko_evt_le_connection_rssi_id:
	        	gecko_cmd_system_halt(1);
	        	if (evt->data.evt_le_connection_rssi.rssi > -35)
	        		gecko_cmd_system_set_tx_power(-260);
	        	else if (evt->data.evt_le_connection_rssi.rssi > -45 && evt->data.evt_le_connection_rssi.rssi < -35)
	        		gecko_cmd_system_set_tx_power(-200);
	        		 else if (evt->data.evt_le_connection_rssi.rssi > -55 && evt->data.evt_le_connection_rssi.rssi < -45)
	        		        		gecko_cmd_system_set_tx_power(-150);
	        		 	  else if (evt->data.evt_le_connection_rssi.rssi > -65 && evt->data.evt_le_connection_rssi.rssi < -55)
	        		        		gecko_cmd_system_set_tx_power(-50);
	        		 	  	   else if (evt->data.evt_le_connection_rssi.rssi > -75 && evt->data.evt_le_connection_rssi.rssi < -65)
	        		        		gecko_cmd_system_set_tx_power(0);
	        		 	  	   	    else if (evt->data.evt_le_connection_rssi.rssi > -85 && evt->data.evt_le_connection_rssi.rssi < -75)
	        		        		gecko_cmd_system_set_tx_power(50);
	        		 	  	   	    	 else gecko_cmd_system_set_tx_power(80);
	        	gecko_cmd_system_halt(0);
	        	break;

	        /* Events related to OTA upgrading
	           ----------------------------------------------------------------------------- */

	        /* Checks if the user-type OTA Control Characteristic was written.
	         * If written, boots the device into Device Firmware Upgrade (DFU) mode. */
	        case gecko_evt_gatt_server_user_write_request_id:
	          if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control) {
	            /* Set flag to enter to OTA mode */
	            boot_to_dfu = 1;
	            /* Send response to Write Request */
	            gecko_cmd_gatt_server_send_user_write_response(
	              evt->data.evt_gatt_server_user_write_request.connection,
	              gattdb_ota_control,
	              bg_err_success);

	            /* Close connection to enter to DFU OTA mode */
	            gecko_cmd_le_connection_close(evt->data.evt_gatt_server_user_write_request.connection);
	          }
	          break;

	        default:
	          break;
	      }
  }
}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
