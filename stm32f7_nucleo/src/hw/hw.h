/*
 * hw.h
 *
 *  Created on: 2018. 4. 14.
 *      Author: baram
 */

#ifndef SRC_HW_HW_H_
#define SRC_HW_HW_H_

#include "hw_def.h"


#ifdef __cplusplus
extern "C" {
#endif


#include "cmdif.h"

#include "core/led.h"
#include "core/uart.h"




void hwInit(void);

void delay(uint32_t time_ms);
uint32_t millis(void);


#ifdef __cplusplus
}
#endif

#endif /* SRC_HW_HW_H_ */
