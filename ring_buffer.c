#include "ring_buffer.h"


int ring_buffer_init(ring_buffer *buffer, uint8_t *data, uint16_t size){
 
    if ((buffer == NULL) || (data == NULL) || (size < 2U)) {
        return 0;
    }

    buffer->data = data;
    buffer->size = size; 
    buffer->head = 0U;
    buffer->tail = 0U;

    return 1;    
}

/*
head == tail                  → буфер пуст
next(head) == tail            → буфер полон
*/

int ring_buffer_put(ring_buffer *buffer, uint8_t byte){

    uint16_t next;

    next = (buffer->head + 1U) % buffer->size;
 
    if (next == buffer->tail) {

        return 0; // Буфер заполнен
    }

    buffer->data[buffer->head] = byte;
    buffer->head = next;

    return 1;
}


int ring_buffer_get(ring_buffer *buffer, uint8_t *byte){

    if (buffer->head == buffer->tail) {
        return 0; // Буфер пуст
    }

    *byte = buffer->data[buffer->tail];

    buffer->tail = (buffer->tail + 1U) % buffer->size;

    return 1;
}

/*

//создание буфера

static ring_buffer_t uart1_rx_buffer;
static uint8_t uart1_rx_data[256];

ring_buffer_init(
    &uart1_rx_buffer, //адрес структуры настройки буфера
    uart1_rx_data, // массив
    sizeof(uart1_rx_data) // его размер
);

ring_buffer_t{
    указатель на массив
    размер  массива
    head
    tail
}

*/