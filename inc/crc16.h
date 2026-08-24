#ifndef CRC16_H_
#define CRC16_H_

#include "main.h"

uint16_t modbus_crc16(const uint8_t *data, uint16_t rx_length);


#endif /* CRC16_H_ */