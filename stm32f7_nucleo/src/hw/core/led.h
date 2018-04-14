/*
 * led.h
 *
 *  Created on: 2018. 4. 14.
 *      Author: baram
 */

#ifndef SRC_HW_CORE_LED_H_
#define SRC_HW_CORE_LED_H_


#include "hw_def.h"

#ifdef _HW_DEF_USE_LED

#ifdef __cplusplus
extern "C" {
#endif



#define LED_MAX_CH    3





void ledInit(void);

void ledOn(uint8_t ch);
void ledOff(uint8_t ch);
void ledToggle(uint8_t ch);


#ifdef __cplusplus
}
#endif

#endif

#endif /* SRC_HW_CORE_LED_H_ */
