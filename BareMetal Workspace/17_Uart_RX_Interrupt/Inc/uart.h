#ifndef UART_H_
#define UART_H_

#include "stm32f446xx.h"
#include <stdint.h>
void init_uart3_rxtx(void);
char uart3_read(void);
void init_uart3_interrupt_rxtx(void);
#define RXNE  			(1<<5)

#endif /* UART_H_ */
