#ifndef MODBUS_REGISTERS_H
#define MODBUS_REGISTERS_H

#include <stdint.h>

#define HOLDING_REGISTERS_COUNT 127

extern uint16_t holding_registers[HOLDING_REGISTERS_COUNT];

void registers_init(void);

#endif // MODBUS_REGISTERS_H