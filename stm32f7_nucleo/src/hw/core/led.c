/*
 * led.c
 *
 *  Created on: 2018. 4. 14.
 *      Author: baram
 */




#include "led.h"


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
