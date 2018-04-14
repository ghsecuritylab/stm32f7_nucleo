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

  ledInit();
}

void delay(uint32_t time_ms)
{
  HAL_Delay(time_ms);
}

uint32_t millis(void)
{
  HAL_GetTick();
}
