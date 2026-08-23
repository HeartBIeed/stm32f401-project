#ifndef UART_H_
#define UART_H_

#include "main.h"
#include "ring_buffer.h"

void USART1_init(uint16_t baudrate);
uint8_t USART1_recieve_byte();
void USART1_send_string(char *str);



#endif /* UART_H_ */
