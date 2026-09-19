#ifndef UART_H_
#define UART_H_

#include "main.h"
#include "ring_buffer.h"

void USART1_init(uint32_t baudrate);
uint8_t USART1_recieve_byte();
void USART1_send_byte(uint8_t tx_data);
void USART1_send_string(char *str);
int USART1_read_byte(uint8_t *byte); 



#endif /* UART_H_ */
