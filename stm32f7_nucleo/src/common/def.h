/*
 * def.h
 *
 *  Created on: 2018. 4. 14.
 *      Author: baram
 */

#ifndef SRC_COMMON_DEF_H_
#define SRC_COMMON_DEF_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>



#include "def_err.h"



#define OK                0

#define _DEF_UART1        0
#define _DEF_UART2        1
#define _DEF_UART3        2
#define _DEF_UART4        3
#define _DEF_UART5        4




typedef struct
{
  uint32_t  ptr_in;
  uint32_t  ptr_out;
  uint32_t  length;
  uint8_t  *p_buf;
} ring_buf_t;


typedef uint32_t err_code_t;




#endif /* SRC_COMMON_DEF_H_ */
