/*
 * led.c
 *
 *  Created on: 2018. 4. 14.
 *      Author: baram
 */




#include "led.h"


#ifdef _HW_DEF_USE_LED

typedef struct
{
  GPIO_TypeDef *port;
  uint16_t      number;
  int           on_sate;
} led_pin_t;



led_pin_t led_pin[LED_MAX_CH] =
    {
       { GPIOB, GPIO_PIN_14, GPIO_PIN_SET},
       { GPIOB, GPIO_PIN_0, GPIO_PIN_SET},
       { GPIOB, GPIO_PIN_7, GPIO_PIN_SET},
    };


#ifdef _HW_DEF_CMDIF_LED
int ledCmdif(int argc, char **argv);
#endif




void ledInit(void)
{
  GPIO_InitTypeDef          GPIO_InitStruct;


  for (uint8_t i=0; i<LED_MAX_CH; i++)
  {
    GPIO_InitStruct.Pin  = led_pin[i].number;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(led_pin[i].port, &GPIO_InitStruct);

    ledOff(i);
  }

#ifdef _HW_DEF_CMDIF_LED
  cmdifAdd("led", ledCmdif);
#endif
}

void ledOn(uint8_t ch)
{
  if (ch < LED_MAX_CH)
  {
    HAL_GPIO_WritePin(led_pin[ch].port, led_pin[ch].number, led_pin[ch].on_sate);
  }
}

void ledOff(uint8_t ch)
{
  if (ch < LED_MAX_CH)
  {
    HAL_GPIO_WritePin(led_pin[ch].port, led_pin[ch].number, !led_pin[ch].on_sate);
  }
}

void ledToggle(uint8_t ch)
{
  if (ch < LED_MAX_CH)
  {
    HAL_GPIO_TogglePin(led_pin[ch].port, led_pin[ch].number);
  }
}




#ifdef _HW_DEF_CMDIF_LED
//-- ledCmdif
//
int ledCmdif(int argc, char **argv)
{
  bool ret = true;
  uint8_t number;


  if (argc == 3)
  {
    number = (uint8_t) strtoul((const char * ) argv[2], (char **)NULL, (int) 0);

    if(strcmp("on", argv[1]) == 0)
    {
      ledOn(number);
    }
    else if(strcmp("off", argv[1])==0)
    {
      ledOff(number);
    }
    else if(strcmp("toggle", argv[1])==0)
    {
      ledToggle(number);
    }
    else if(strcmp("demo", argv[1])==0)
    {
      while(cmdifRxAvailable() == 0)
      {
        ledToggle(number);
        delay(200);
        cmdifPrintf("led toggle %d\n", number);
      }
    }
    else
    {
      ret = false;
    }
  }
  else
  {
    ret = false;
  }

  if (ret == false)
  {
    cmdifPrintf( "led on/off/toggle/demo number ...\n");
  }

  return 0;
}
#endif

#endif
