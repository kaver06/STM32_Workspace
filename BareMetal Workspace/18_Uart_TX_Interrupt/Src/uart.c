
#include "uart.h"

#define GPIODEN 		(1<<3)
#define USART3EN 		(1<<18)
#define SystemClk 		16000000
#define APB1 			SystemClk
#define Uart_Baudrate 	115200
#define TE_EN 			(1<<3)
#define RE_EN 			(1<<2)
#define UE_EN 			(1<<13)

static uint16_t compute_uart_bd(uint32_t Pheriph_clk,uint32_t Baudrate);
static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t Pheriph_clk,uint32_t Baudrate);
void uart3_write(int ch);

int __io_putchar(int ch)
{
	uart3_write(ch);
	return ch;
}

void init_uart3_tx(void)
{
	/*********configure uart gpio_pin***********/
	//enable clock access to gpioD
	RCC->AHB1ENR|=GPIODEN;
	//set PD8 to alternate function mode
	GPIOD->MODER |= (1<<17);
	GPIOD->MODER &=~(1<<16);
	//set PD8 alternate function to type UART_TX (AF7)
	GPIOD->AFR[1]|=(1<<0);
	GPIOD->AFR[1]|=(1<<1);
	GPIOD->AFR[1]|=(1<<2);
	GPIOD->AFR[1]&=~(1<<3);


	/*********configuring Uart module**********/
	//enable clock access to uart3
	RCC->APB1ENR |= USART3EN;
	//configure baudrate
	uart_set_baudrate(USART3,APB1,Uart_Baudrate);
	//configure the transfer direction for both RX and TX using control register
	USART3->CR1 = (TE_EN | RE_EN);
	//enable uart module
	USART3->CR1 |= UE_EN;
}

void uart3_write(int ch)
{
	//ensure the transmit data register is empty
	while(!(USART3->SR & SR_TXE));
	//write to the transmit data register
	USART3->DR = (ch&0xFF);
}


static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t Pheriph_clk,uint32_t Baudrate)
{
	USART3->BRR |=compute_uart_bd(Pheriph_clk,Baudrate);
}

static uint16_t compute_uart_bd(uint32_t Pheriph_clk,uint32_t Baudrate)
{
	return (Pheriph_clk+Baudrate/2)/Baudrate;
}
void init_uart3_tx_interrupt(void)
{
	/*********configure uart gpio_pin***********/
	//enable clock access to gpioD
	RCC->AHB1ENR|=GPIODEN;
	//set PD8 to alternate function mode
	GPIOD->MODER |= (1<<17);
	GPIOD->MODER &=~(1<<16);
	//set PD8 alternate function to type UART_TX (AF7)
	GPIOD->AFR[1]|=(1<<0);
	GPIOD->AFR[1]|=(1<<1);
	GPIOD->AFR[1]|=(1<<2);
	GPIOD->AFR[1]&=~(1<<3);


	/*********configuring Uart module**********/
	//enable clock access to uart3
	RCC->APB1ENR |= USART3EN;
	//configure baudrate
	uart_set_baudrate(USART3,APB1,Uart_Baudrate);
	//configure the transfer direction for both RX and TX using control register
	USART3->CR1 = (TE_EN | RE_EN);
	//enable tx_interrupt
	USART3->CR1 |= CR_TXEIE;
	NVIC_EnableIRQ(USART3_IRQn);
	//enable uart module
	USART3->CR1 |= UE_EN;
}
