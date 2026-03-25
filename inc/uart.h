#ifndef UART_H_
#define UART_H_

#include "main.h"

#define FCPU 84000000u
#define U1RX 10
#define U1TX 9

void UART_init(uint16_t speed);
void usart1_send_byte(uint8_t tx_data);
uint8_t usart1_recieve_byte();
void usart1_rxen_flag();
void usart1_ptr_str(char *str);
void usart1_echo();
int USART_commands();


extern volatile uint8_t uart1_flag;
extern volatile uint8_t usart_data_buffer[32];

#endif /* UART_H_ */
