/*
 * hw.c
 *
 *  Created on: 2018. 4. 14.
 *      Author: baram
 */



#include "hw.h"





void hwInit(void)
{
  bspInit();

  cmdifInit();
  ledInit();
  uartInit();
}

void delay(uint32_t time_ms)
{
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
    HAL_Delay(time_ms);
  }
  else
  {
    osDelay(time_ms);
  }
}

uint32_t millis(void)
{
  return HAL_GetTick();
}
