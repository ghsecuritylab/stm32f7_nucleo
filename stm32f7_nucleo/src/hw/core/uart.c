/*
 * uart.c
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */
/*
  _HW_DEF_UART_CH_MAX : 2
  _DEF_UART1          : VCP
  _DEF_UART2          : USART2
*/
/*
  USART2
    - RX : PD6, DMA1, Channel 4, Stream 5
    - TX : PD5, DMA1, Channel 4, Stream 6


  USART8
    - RX : PE0, DMA1, Channel 5, Stream 6
    - TX : PE1, DMA1, Channel 5, Stream 0
*/


#include <stdarg.h>
#include <stdbool.h>
#include "hw.h"



#include "uart.h"



#define UART_RX_BUF_LENGTH    64



//-- Internal Variables
//
uart_t  uart_tbl[UART_MAX_CH];
uint8_t uart_buffer[UART_MAX_CH][UART_RX_BUF_LENGTH] __attribute__((section(".NoneCacheableMem")));


//-- External Variables
//


//-- Internal Functions
//
static bool uartIsEnable(uint8_t channel);
void uartStartRx(uint8_t channel);


//-- External Functions
//


//-- Driver Functions
//




bool uartInit(void)
{
  uint32_t i;


  for(i=0; i<UART_MAX_CH; i++)
  {
    uart_tbl[i].is_open           = false;
    uart_tbl[i].hw.rx_buf.ptr_in  = 0;
    uart_tbl[i].hw.rx_buf.ptr_out = 0;
    uart_tbl[i].hw.rx_buf.p_buf   = NULL;
    uart_tbl[i].hw.vcp_enable     = false;
    uart_tbl[i].hw.dma_enable     = false;
  }


  return true;
}

uint32_t uartOpen(uint8_t channel, uint32_t baud)
{
	uint32_t err_code  = OK;
	uart_t *p_drv_uart = &uart_tbl[channel];


  if (channel >= UART_MAX_CH)
  {
    return 1;
  }



  switch(channel)
  {
    case _DEF_UART1:

      p_drv_uart->baud           = baud;
      p_drv_uart->hw.h_uart_inst = USART3;
      p_drv_uart->hw.dma_enable  = true;
      p_drv_uart->hw.vcp_enable  = false;

      p_drv_uart->hw.rx_buf.ptr_in  = 0;
      p_drv_uart->hw.rx_buf.ptr_out = 0;
      p_drv_uart->hw.rx_buf.p_buf   = (uint8_t *)uart_buffer[0];
      p_drv_uart->hw.rx_buf.length  = UART_RX_BUF_LENGTH;

      p_drv_uart->hw.h_uart.Instance        = p_drv_uart->hw.h_uart_inst;
      p_drv_uart->hw.h_uart.Init.BaudRate   = baud;
      p_drv_uart->hw.h_uart.Init.WordLength = UART_WORDLENGTH_8B;
      p_drv_uart->hw.h_uart.Init.StopBits   = UART_STOPBITS_1;
      p_drv_uart->hw.h_uart.Init.Parity     = UART_PARITY_NONE;
      p_drv_uart->hw.h_uart.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
      p_drv_uart->hw.h_uart.Init.Mode       = UART_MODE_TX_RX;

      HAL_UART_DeInit(&p_drv_uart->hw.h_uart);
      HAL_UART_Init(&p_drv_uart->hw.h_uart);



      p_drv_uart->is_open = true;


      uartStartRx(channel);
      break;
  }





  return err_code;
}

bool uartIsEnable(uint8_t channel)
{
  if (channel >= UART_MAX_CH)
  {
    return false;
  }

  if (uart_tbl[channel].hw.vcp_enable == true)
  {
    /*
    if (vcpIsConnected() == false || uart_tbl[channel].is_open == false)
    {
      return false;
    }
    else
    {
      return true;
    }
    */
    return false;
  }
  else
  {
    return uart_tbl[channel].is_open;
  }
}

void uartWaitForEnable(uint8_t channel, uint32_t timeout)
{
  uint32_t t_time;


  t_time = millis();
  while(1)
  {
    if (uartIsEnable(channel) == true)
    {
      break;
    }
    if ((millis()-t_time) >= timeout)
    {
      break;
    }
  }
}

uint32_t uartAvailable(uint8_t channel)
{
  uint32_t length = 0;
  uart_t *p_drv_uart = &uart_tbl[channel];


  if (p_drv_uart->hw.vcp_enable == true)
  {
    length = 0; //vcpAvailable();
  }
  else
  {
    if (p_drv_uart->hw.dma_enable == true)
    {
      p_drv_uart->hw.rx_buf.ptr_in = p_drv_uart->hw.rx_buf.length - p_drv_uart->hw.hdma_rx.Instance->NDTR;
      length = (   p_drv_uart->hw.rx_buf.length
                 + p_drv_uart->hw.rx_buf.ptr_in
                 - p_drv_uart->hw.rx_buf.ptr_out ) % p_drv_uart->hw.rx_buf.length;

    }
    else
    {
      length = (   p_drv_uart->hw.rx_buf.length
                 + p_drv_uart->hw.rx_buf.ptr_in
                 - p_drv_uart->hw.rx_buf.ptr_out ) % p_drv_uart->hw.rx_buf.length;
    }
  }

  return length;
}

void uartPutch(uint8_t channel, uint8_t ch)
{
  uartWrite(channel, &ch, 1 );
}

uint8_t uartGetch(uint8_t channel)
{
  if (uartIsEnable(channel) == false ) return 0;


  while(1)
  {
    if( uartAvailable(channel) ) break;
  }


  return uartRead(channel);
}

int32_t uartWrite(uint8_t channel, uint8_t *p_data, uint32_t length)
{
  int32_t  ret = 0;
  uart_t *p_drv_uart = &uart_tbl[channel];


  if (uartIsEnable(channel) == false ) return 0;


  if (p_drv_uart->hw.vcp_enable == true)
  {
    ret = 0; //vcpWrite(p_data, length);
  }
  else
  {
    if (HAL_UART_Transmit(&p_drv_uart->hw.h_uart, (uint8_t*)p_data, length, 100) == HAL_OK)
    {
      ret = length;
    }
  }


  return ret;
}

uint8_t uartRead(uint8_t channel)
{
  uint8_t ret = 0;
  uart_t *p_drv_uart = &uart_tbl[channel];
  uint32_t index;
  volatile uint32_t next_index;


  if (p_drv_uart->hw.vcp_enable == true)
  {
    ret = 0; //vcpRead();
  }
  else
  {
    index      = p_drv_uart->hw.rx_buf.ptr_out;
    next_index = p_drv_uart->hw.rx_buf.ptr_out + 1;

    if (next_index == p_drv_uart->hw.rx_buf.length)
    {
      next_index = 0;;
    }

    if (index != p_drv_uart->hw.rx_buf.ptr_in)
    {
      ret = p_drv_uart->hw.rx_buf.p_buf[index];
      p_drv_uart->hw.rx_buf.ptr_out = next_index;
    }
  }
  return ret;
}

int32_t uartPrintf(uint8_t channel, const char *fmt, ...)
{
  int32_t ret = 0;
  va_list arg;
  va_start (arg, fmt);
  int32_t len;
  static char print_buffer[255];


  if (uartIsEnable(channel) == false ) return 0;

  len = vsnprintf(print_buffer, 255, fmt, arg);
  va_end (arg);

  ret = uartWrite(channel, (uint8_t *)print_buffer, len);

  return ret;
}

int32_t uartPrint(uint8_t channel, uint8_t *p_str)
{
  int32_t index = 0;

  if (uartIsEnable(channel) == false ) return 0;

  while(1)
  {
    uartPutch(channel, p_str[index]);

    if (p_str[index] == 0)
    {
      break;
    }

    index++;

    if (index > 255)
    {
      break;
    }
  }


  return index;
}

void uartStartRx(uint8_t channel)
{
  uart_t *p_drv_uart = &uart_tbl[channel];


  if(p_drv_uart->hw.dma_enable == true)
  {
    HAL_UART_Receive_DMA(&p_drv_uart->hw.h_uart, (uint8_t *)p_drv_uart->hw.rx_buf.p_buf, p_drv_uart->hw.rx_buf.length);
  }
  else
  {
    //
  }

}

void uartErrHandler(uint8_t channel)
{
  uart_t *p_drv_uart = &uart_tbl[channel];


  if(p_drv_uart->hw.dma_enable == false)
  {
      uartStartRx(channel);
  }
}

uint8_t uartGetIndex(UART_HandleTypeDef *UartHandle)
{
  uint32_t i;
  uint8_t  ret = 0;

  for(i=0; i<UART_MAX_CH; i++)
  {
    if (UartHandle->Instance == uart_tbl[i].hw.h_uart_inst)
    {
      ret = i;
    }
  }

  return ret;
}


//--------------------------------------- Hardware Functions
//
void USART3_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uart_tbl[_DEF_UART1].hw.h_uart);
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  uartErrHandler(uartGetIndex(UartHandle));
}

// UART1 DMA IRQ
void DMA1_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uart_tbl[_DEF_UART1].hw.h_uart.hdmarx);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct;
  uart_t *p_drv_uart;
  uint8_t channel = uartGetIndex(huart);


  p_drv_uart = &uart_tbl[channel];


  if (huart->Instance == USART3)
  {
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
    RCC_PeriphCLKInitStruct.Usart2ClockSelection = RCC_USART3CLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();

    //-- TX Pin
    GPIO_InitStruct.Pin       = GPIO_PIN_8;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    //-- RX Pin
    GPIO_InitStruct.Pin       = GPIO_PIN_9;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);



    // DMA Setup
    /* Configure the DMA handler for reception process */
    __HAL_RCC_DMA1_CLK_ENABLE();

    p_drv_uart->hw.hdma_rx.Instance                 = DMA1_Stream1;
    p_drv_uart->hw.hdma_rx.Init.Channel             = DMA_CHANNEL_4;
    p_drv_uart->hw.hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    p_drv_uart->hw.hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    p_drv_uart->hw.hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
    p_drv_uart->hw.hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    p_drv_uart->hw.hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    p_drv_uart->hw.hdma_rx.Init.Mode                = DMA_CIRCULAR;
    p_drv_uart->hw.hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;

    HAL_DMA_Init(&p_drv_uart->hw.hdma_rx);

    /* Associate the initialized DMA handle to the the UART handle */
    __HAL_LINKDMA(huart, hdmarx, p_drv_uart->hw.hdma_rx);

    /* NVIC configuration for DMA transfer complete interrupt (USART6_RX) */
    HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);


    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(USART3_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ  (USART3_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{

  if(huart->Instance==USART3)
  {
    __USART3_FORCE_RESET();
    __USART3_RELEASE_RESET();

    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();


    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8);
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_9);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(USART3_IRQn);
    HAL_NVIC_DisableIRQ(DMA1_Stream1_IRQn);
  }
}







