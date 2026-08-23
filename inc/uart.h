#ifndef UART_H_
#define UART_H_

#include "main.h"

#define U1RX 10
#define U1TX 9

void UART_init(uint16_t speed);
void usart1_send_byte(uint8_t tx_data);
void usart1_ptr_str(char *str);



#endif /* UART_H_ */
