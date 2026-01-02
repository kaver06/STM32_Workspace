
#ifndef UART_H_
#define UART_H_

#include "stm32f446xx.h"
#include <stdint.h>
void init_uart3_tx(void);
void init_uart3_tx_interrupt(void);
#define SR_TXE	  			(1<<7)
#define CR_TXEIE 		(1U<<7)

#endif /* UART_H_ */
