
#include "uart.h"

#define GPIODEN (1<<3)
#define USART3EN (1<<18)

#define SystemClk 16000000
#define APB1 SystemClk
#define Uart_Baudrate 115200
#define TE_EN (1<<3)
#define RE_EN (1<<2)
#define UE_EN (1<<13)
#define TXE	  (1<<7)
#define RXNE  (1<<5)
static uint16_t compute_uart_bd(uint32_t Pheriph_clk,uint32_t Baudrate);
static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t Pheriph_clk,uint32_t Baudrate);
void uart3_write(int ch);



int __io_putchar(int ch)
{
	uart3_write(ch);
	return ch;
}

void init_uart3_rxtx(void)
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
	//	//set PD9 to alternate function mode
	GPIOD->MODER |= (1<<19);
	GPIOD->MODER &=~(1<<18);
	//set PD9 to alternate function to type UART_RX (AF7)
	GPIOD->AFR[1]|=(1<<4);
	GPIOD->AFR[1]|=(1<<5);
	GPIOD->AFR[1]|=(1<<6);
	GPIOD->AFR[1]&=~(1<<7);

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
	while(!(USART3->SR & TXE));
	//write to the transmit data register
	USART3->DR = (ch&0xFF);
}
//function to recieve the charactor from uart
char uart3_read(void)
{
	//ensure the receive  data register is not empty
	while(!(USART3->SR & RXNE));
	//receive data
	return USART3->DR;
}
static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t Pheriph_clk,uint32_t Baudrate)
{
	USART3->BRR |=compute_uart_bd(Pheriph_clk,Baudrate);
}

static uint16_t compute_uart_bd(uint32_t Pheriph_clk,uint32_t Baudrate)
{
	return (Pheriph_clk+Baudrate/2)/Baudrate;
}
