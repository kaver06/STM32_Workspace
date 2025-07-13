#include "stm32f4xx.h"
#include<stdint.h>

#define GPIOAEN		(1U<<0)
#define UART2EN		(1U<<17)

#define CR1_TE (1U<<3)
#define CR1_UE (1U<<13)
#define SR_TXE (1U<<7)

#define SYS_FREQ 16000000
#define APB1 SYS_FREQ

#define Uart_BaudRate 115200

static uint16_t compute_uart_bd(uint32_t PeriphClk,uint32_t BaudRate);
static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t PeriphClk,uint32_t BaudRate);

void uart2_write(int ch);
void init_uart2_tx(void);


#define Uart_BaudRate 115200


int main(void)
{
	init_uart2_tx();
	while(1)
	{
		uart2_write('Y');
	}
}

void init_uart2_tx(void)
{
	/*********configure uart gpio_pin***********/
	//enable clock access to gpioA
	RCC->AHB1ENR|=GPIOAEN;
	//set PA2 to alternate function mode
	GPIOA->MODER|= (1U<<5);
	GPIOA->MODER&= ~(1U<<4);
	//set PA2 alternate function to type UART_TX (AF7)
	GPIOA->AFR[0]&=~(1U<<11);
	GPIOA->AFR[0]|=(1U<<10);
	GPIOA->AFR[0]|=(1U<<9);
	GPIOA->AFR[0]|=(1U<<8);

	/*********configuring Uart module**********/
	//enable clock access to uart2
	RCC->APB1ENR|=(1U<<17);
	//configure baudrate
	uart_set_baudrate(USART2,APB1,Uart_BaudRate);
	//configure the transfer direction
	USART2->CR1 = CR1_TE;

	//enable uart module
	USART2->CR1 |= CR1_UE;
}

void uart2_write(int ch)
{
	//ensure the transmit data register is empty
	while(!(USART2->SR & SR_TXE));
	//write to the transmit data register
	USART2->DR = (ch&0xFF);

}

static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t PeriphClk,uint32_t BaudRate)
{
	USART2->BRR = compute_uart_bd(PeriphClk,BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk,uint32_t BaudRate)
{
	return (PeriphClk+BaudRate/2)/BaudRate;
}
