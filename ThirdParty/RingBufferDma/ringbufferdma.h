#ifndef _RINGBUFFER_DMA_H_
#define _RINGBUFFER_DMA_H_

//#include <platform_config.h>
#include "stm32f1xx_hal.h"
#include "main.h"

#ifdef HAL_DMA_MODULE_ENABLED

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint8_t* storage;
  uint8_t* end;
  uint16_t size;
  volatile uint8_t* read;
  volatile uint8_t* write;
  volatile uint16_t available;
  UART_HandleTypeDef* dmaHandle;
} RingBufferDmaU8;

void RingBufferDmaU8_initUSARTRx(RingBufferDmaU8* ring, UART_HandleTypeDef* husart, uint8_t* buffer, uint16_t size);
//uint16_t RingBufferDmaU8_available(RingBufferDmaU8* ring);
//bool RingBufferDmaU8_readLine(RingBufferDmaU8* ring, char* line, uint16_t size);
uint8_t RingBufferDmaU8_readByte(RingBufferDmaU8* ring);
uint16_t RingBufferDmaU8_read(RingBufferDmaU8* ring, uint8_t* buffer, uint16_t size);
void RingBufferDmaU8_write(RingBufferDmaU8* ring, uint16_t size);

#endif

#endif

