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
uint16_t midt;