#ifndef MODBUS_H_
#define MODBUS_H_

#include "main.h"

#define MODBUS_MAX_FRAME_SIZE 256
#define HOLDING_REGISTERS_COUNT 127

extern uint16_t holding_registers[HOLDING_REGISTERS_COUNT];

typedef enum { // состояния
    MODBUS_RX_IDLE,
    MODBUS_RX_RECEIVING,
    MODBUS_RX_FRAME_READY
} modbus_rx_state;

typedef enum {
    MODBUS_FC_READ_COILS               = 0x01,  // Read Coils
    MODBUS_FC_READ_DISCRETE_INPUTS     = 0x02,  // Read Discrete Inputs
    MODBUS_FC_READ_HOLDING_REGISTERS   = 0x03,  // Read Holding Registers
    MODBUS_FC_READ_INPUT_REGISTERS     = 0x04,  // Read Input Registers
    MODBUS_FC_WRITE_SINGLE_COIL        = 0x05,  // Write Single Coil
    MODBUS_FC_WRITE_SINGLE_REGISTER    = 0x06,  // Write Single Register
    MODBUS_FC_WRITE_MULTIPLE_COILS     = 0x0F,  // Write Multiple Coils
    MODBUS_FC_WRITE_MULTIPLE_REGISTERS = 0x10,  // Write Multiple Registers
    MODBUS_FC_REPORT_SLAVE_ID          = 0x11,  // Report Slave ID.
} modbus_function;

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

void modbus_init(modbus_rtu *modbus, uint8_t slave_address);
void modbus_poll(modbus_rtu *modbus);
int modbus_frame_check(modbus_rtu *modbus);
int modbus_handle_read_holding(modbus_rtu *modbus, const uint16_t *registers, uint16_t registers_count);
int modbus_handle_write_single_hldreg(modbus_rtu *modbus, const uint16_t *registers, uint16_t registers_count);
int modbus_handle_write_multiple_hldreg(modbus_rtu *modbus, const uint16_t *registers, uint16_t registers_count);
int modbus_handle_report_slave_id(modbus_rtu *modbus);









#endif /* MODBUS_H_ */
