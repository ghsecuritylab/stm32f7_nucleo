/*
 * hw_def.h
 *
 *  Created on: 2018. 4. 14.
 *      Author: baram
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_

#include "def.h"
#include "bsp.h"
#include "cmdif.h"


#ifdef __cplusplus
extern "C" {
#endif


#define _HW_DEF_USE_LED





#define _HW_DEF_CMDIF_LED





#define _HW_DEF_CMDIF_LIST_MAX    16








extern void delay(uint32_t time_ms);
extern uint32_t millis(void);


#ifdef __cplusplus
}
#endif

#endif /* SRC_HW_HW_DEF_H_ */
