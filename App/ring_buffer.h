#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include "main.h"

typedef struct {

    uint8_t *data;
    uint16_t size;
    volatile uint16_t head;
    volatile uint16_t tail;

} ring_buffer;

int ring_buffer_init(ring_buffer *buffer, uint8_t *data, uint16_t size);
int ring_buffer_put(ring_buffer *buffer, uint8_t byte);
int ring_buffer_get(ring_buffer *buffer, uint8_t *byte);


#endif /* RING_BUFFER_H_ */