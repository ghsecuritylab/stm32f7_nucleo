/*
 * ap.cpp
 *
 *  Created on: 2018. 4. 14.
 *      Author: baram
 */




#include "ap.h"





void apInit(void)
{
  cmdifBegin(_DEF_UART1, 115200);
}

void apMain(void)
{
  uint32_t pre_time;

  pre_time = millis();
  while(1)
  {
    if (millis()-pre_time >= 500)
    {
      ledToggle(0);
    }

    cmdifMain();
  }
}
