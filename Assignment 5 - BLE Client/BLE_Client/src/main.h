//***********************************************************************************
// Include files
//***********************************************************************************

#include <stdint.h>
#include <stdbool.h>

//***********************************************************************************
// defined files
//***********************************************************************************

#define TIME_P 3 //in seconds
#define EVENT_1 0x01

//***********************************************************************************
// global variables
//***********************************************************************************

//***********************************************************************************
// function prototypes
//***********************************************************************************
void sleep(void);
void BlockSleep(int);
void UnblockSleep(int);
__uint8_t bonding_var;
__uint8_t tempbonding_var;
float midt;
struct gecko_msg_system_get_bt_address_rsp_t *bt_addr_t;
char bt_addr_string[64];
char lcdtemp[13];
char passkey[10];
uint8_t serv;
uint32_t service_handle;
uint32_t characteristic_handle;
uint32_t t;
uint8_t *tempArrptr;
