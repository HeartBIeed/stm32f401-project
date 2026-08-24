#ifndef MODBUS_H_
#define MODBUS_H_

#include "main.h"

#define MODBUS_MAX_FRAME_SIZE 256

typedef enum { // состояния
    MODBUS_RX_IDLE,
    MODBUS_RX_RECEIVING,
    MODBUS_RX_FRAME_READY
} modbus_rx_state;

typedef struct {
    uint8_t address;
    uint8_t rx_frame[MODBUS_MAX_FRAME_SIZE];
    uint16_t rx_length;
    uint32_t last_byte_time;
    uint8_t frame_timeout;
    modbus_rx_state rx_state;

    uint8_t tx_frame[MODBUS_MAX_FRAME_SIZE];
    uint16_t tx_length;

} modbus_rtu;

void modbus_init(modbus_rtu *modbus, uint8_t address);
void modbus_poll(modbus_rtu *modbus);
int modbus_frame_check(modbus_rtu *modbus);
void modbus_frame_reset(modbus_rtu *modbus);
int modbus_handle_read_holding(modbus_rtu *modbus, const uint16_t *registers, uint16_t registers_count);









#endif /* MODBUS_H_ */
