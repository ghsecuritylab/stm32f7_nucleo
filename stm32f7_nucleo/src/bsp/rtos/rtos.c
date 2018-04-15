/*
 * rtos.c
 *
 *  Created on: 2018. 4. 15.
 *      Author: baram
 */



#include "hw.h"
#include "rtos.h"



void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
  uartPrintf(_DEF_UART1, "rtos stack over %s\n", pcTaskName);

  while(1)
  {

  }
}


void rtosInit(void)
{

}
