#include "modbus_registers.h"

uint16_t holding_registers[HOLDING_REGISTERS_COUNT] = {0};

void registers_init(void)
{
    holding_registers[10] = 555;
    holding_registers[11] = 777;
    holding_registers[12] = 0;
    holding_registers[13] = 0;
    holding_registers[14] = 0;



}