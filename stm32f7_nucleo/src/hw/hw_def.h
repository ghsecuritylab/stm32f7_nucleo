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





#define IP_ADDR0   (uint8_t) 172
#define IP_ADDR1   (uint8_t) 30
#define IP_ADDR2   (uint8_t) 1
#define IP_ADDR3   (uint8_t) 10

/*NETMASK*/
#define NETMASK_ADDR0   (uint8_t) 255
#define NETMASK_ADDR1   (uint8_t) 255
#define NETMASK_ADDR2   (uint8_t) 255
#define NETMASK_ADDR3   (uint8_t) 0

/*Gateway Address*/
#define GW_ADDR0   (uint8_t) 222
#define GW_ADDR1   (uint8_t) 100
#define GW_ADDR2   (uint8_t) 116
#define GW_ADDR3   (uint8_t) 254





extern void delay(uint32_t time_ms);
extern uint32_t millis(void);


#ifdef __cplusplus
}
#endif

#endif /* SRC_HW_HW_DEF_H_ */
