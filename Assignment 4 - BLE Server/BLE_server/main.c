
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

  //Define variable for keeping track of scheduler
  schedule_event = 0;
 // sleepdone = 0;

  //Initializing variables

  while (1) {	//Scheduler
	 /*
	  if (schedule_event & EVENT_1)
	  {
		  i2c_init();	//Initialize I2C
		  //sleepy();
		  i2c_driver();	//Initialize driver
		  compare_temp();	//Compare measured value of temperature with preset threshold
		  schedule_event &= ~EVENT_1;	//Clear schedule_event
		  i2c_disable();	//Disable I2C
		  UnblockSleep(sleepEM2);
	  }
	      Event pointer for handling events*/
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
	          gecko_cmd_le_gap_set_advertise_timing(0, 400, 400, 0, 0);

	          /* Start general advertising and enable connections. */
	          gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable);
	          break;

	        case gecko_evt_le_connection_opened_id:
	        	        	connection = evt->data.evt_le_connection_opened.connection;
	        	        	gecko_cmd_le_connection_set_parameters(evt->data.evt_le_connection_opened.connection,60,60,3,400);
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
//	            if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == 0x02) {
//	              /* Indications have been turned ON - start the repeating timer. The 1st parameter '32768'
//	               * tells the timer to run for 1 second (32.768 kHz oscillator), the 2nd parameter is
//	               * the timer handle and the 3rd parameter '0' tells the timer to repeat continuously until
//	               * stopped manually.*/
//	              gecko_cmd_hardware_set_soft_timer(32768, 0, 0);
//	            } else if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == 0x00) {
//	              /* Indications have been turned OFF - stop the timer. */
//	              gecko_cmd_hardware_set_soft_timer(0, 0, 0);
//	            }
	       		  gecko_cmd_le_connection_get_rssi(connection);
//	        	if (evt->data.evt_gatt_server_characteristic_status.status_flags == gatt_server_confirmation){
//	        	  gecko_cmd_le_connection_get_rssi(evt->data.evt_gatt_server_characteristic_status.connection);
	       	  }
	          break;

	        /* This event is generated when the software timer has ticked. In this example the temperature
	         * is read after every 1 second and then the indication of that is sent to the listening client. */

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
	          break;

	        case gecko_evt_system_external_signal_id:
	        	 i2c_init();	//Initialize I2C
	        	  uint8_t htmTempBuffer[5]; /* Stores the temperature data in the Health Thermometer (HTM) format. */
	        	  uint8_t flags = 0x00;   /* HTM flags set as 0 for Celsius, no time stamp and no temperature type. */
//	        	  int32_t tempData;     /* Stores the Temperature data read from the RHT sensor. */
	//        	  uint32_t rhData = 0;    /* Dummy needed for storing Relative Humidity data. */
	        	  uint32_t temperature;   /* Stores the temperature data read from the sensor in the correct format */
	        	  uint8_t *p = htmTempBuffer; /* Pointer to HTM temperature buffer needed for converting values to bitstream. */

	        	  /* Convert flags to bitstream and append them in the HTM temperature data buffer (htmTempBuffer) */
	        	    UINT8_TO_BITSTREAM(p, flags);

	        	 i2c_driver();	//Initialize driver
	        	 midt = compare_temp();	//Compare measured value of temperature with preset threshold

	        	  temperature = (uint32_t)FLT_TO_UINT32(midt, 0);
	        	  UINT32_TO_BITSTREAM(p, temperature);
	        	  gecko_cmd_gatt_server_send_characteristic_notification(0xFF, gattdb_temperature_measurement, 5, htmTempBuffer);

	        	 i2c_disable();	//Disable I2C
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
