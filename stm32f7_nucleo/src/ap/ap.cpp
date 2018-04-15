/*
 * ap.cpp
 *
 *  Created on: 2018. 4. 14.
 *      Author: baram
 */




#include "ap.h"



static void threadMain(const void *argument);




void apInit(void)
{
  cmdifBegin(_DEF_UART1, 115200);
}

void apMain(void)
{
  osThreadDef(threadMain, threadMain, osPriorityBelowNormal, 0, 6*1024/4);
  osThreadCreate(osThread(threadMain), NULL);

  osKernelStart ();
}

static void threadMain(const void *argument)
{
  uint32_t pre_time;



  pre_time = millis();
  while(1)
  {
    if (millis()-pre_time >= 500)
    {
      pre_time = millis();
      ledToggle(0);
    }

    cmdifMain();
    osThreadYield();
  }
}
