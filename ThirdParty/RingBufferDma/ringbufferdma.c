
#include "ringbufferdma.h"

#ifdef HAL_DMA_MODULE_ENABLED

#include "utils.h"

void RingBufferDmaU8_initUSARTRx(RingBufferDmaU8* ring, UART_HandleTypeDef* husart, uint8_t* buffer, uint16_t size) {
  ring->storage = buffer;
  ring->size = size;
  ring->end = ring->storage + ring->size;
  ring->read = ring->storage;
  ring->write = ring->storage;
  ring->available = 0;
  ring->dmaHandle = husart;
  HAL_UART_Receive_DMA(ring->dmaHandle, (uint8_t *)ring->write, ring->size);
}

uint16_t RingBufferDmaU8_available(RingBufferDmaU8* ring) {
  return ring->available;
}

//bool RingBufferDmaU8_readLine(RingBufferDmaU8* ring, char* line, uint16_t size) {
//  volatile uint8_t* in = ring->tailPtr;
//  uint8_t* out = (uint8_t*)line;
//  uint8_t total = min(size, RingBufferDmaU8_available(ring));
//  for (uint16_t i = 0; i < total; i++) {
//    uint8_t c = *in++;
//    *out = c;
//    if (in >= ring->buffer + ring->size) {
//      in -= ring->size;
//    }
//    if (c == '\n' || c == '\r') {
//      *out = 0x00;
//      ring->tailPtr = in;
//      return true;
//    }
//    out++;
//  }
//  return false;
//}

uint8_t RingBufferDmaU8_readByte(RingBufferDmaU8* ring) {
  if (ring->available == 0) {
    return 0;
  }
  uint8_t ret = *ring->read++;
  ring->available--;
  if (ring->read >= ring->end) {
    ring->read = ring->storage;
  }
  return ret;
}

uint16_t RingBufferDmaU8_read(RingBufferDmaU8* ring, uint8_t* buffer, uint16_t size) {
	uint8_t total = min(size, RingBufferDmaU8_available(ring));
  // TODO can be optimized
  for (uint16_t i = 0; i < total; i++) {
    buffer[i] = RingBufferDmaU8_readByte(ring);
  }
	return total;
}


void RingBufferDmaU8_writeByte(RingBufferDmaU8* ring) {
  if (ring->available >= ring->size) {
    RingBufferDmaU8_readByte(ring);
  }

  ring->write++;
  ring->available++;
  if (ring->write >= ring->end) {
    ring->write = ring->storage;
  }
}

void RingBufferDmaU8_write(RingBufferDmaU8* ring, uint16_t size) {
  ENTER_CRITICAL_CODE;
  uint16_t i;

  // TODO can be optimized
  for (i = 0; i < size; i++) {
    RingBufferDmaU8_writeByte(ring);
  }
	HAL_UART_Receive_DMA(ring->dmaHandle, (uint8_t *)ring->write, ring->size - ring->available);
	EXIT_CRITICAL_CODE;
}

#endif
