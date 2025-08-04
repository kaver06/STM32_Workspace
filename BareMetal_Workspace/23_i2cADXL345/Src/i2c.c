#include "stm32f446xx.h"
#include "i2c.h"

/*
 * PB8 I2C1_ SCL
 * PB9 I2C1_ SCL
 */
#define GPIOBEN 			(1U<<1)
#define I2C1EN 				(1U<<21)
#define CR1_SWRST			(1U<<15)
#define I2C_100HZ 			80
#define I2C_STD_RISE_TIME	17
#define CR1_PE				(1U<<0)
void i2c_init(void)
{
	//enable clock access to GPIOB
	RCC->AHB1ENR |= GPIOBEN;
	//Configure the mode of the port pins to alternate function
	GPIOB->MODER |= (1U<<17);
	GPIOB->MODER &= ~(1U<<16);
	GPIOB->MODER |= (1U<<18);
	GPIOB->MODER &=	 ~(1U<<19);
	//configure the alternate function type
	GPIOB->AFR[1] = (1U<<2);
	GPIOB->AFR[1] |=(1U<<6);
	//set output type to be open drain
	GPIOB->OTYPER |= (1U<<8);
	GPIOB->OTYPER |= (1U<<9);
	//enable the pull ups
	GPIOB->PUPDR |= (1U<<16);
	GPIOB->PUPDR &= ~(1<<17);
	GPIOB->PUPDR |= (1U<<18);
	GPIOB->PUPDR &= ~(1<<19);
	//enable clock access to apb1 bus of I2C1
	RCC->APB1ENR |= I2C1EN;
	//enter the i2c reset mode
	I2C1->CR1 |= CR1_SWRST;
	//get out of reset
	I2C1->CR1 &= ~CR1_SWRST;
	//configure the peripheral clock frequency to 16Mhz
	I2C1->CR2 = (1U<<4);
	//set the i2c mode to standard mode,100KHz clock
	I2C1->CCR |= I2C_100HZ;
	//set the rise time
	I2C1->TRISE = I2C_STD_RISE_TIME;
	//enable i2c module
	I2C1->CR1 |= CR1_PE;
}

/*Create a function to read the byte data with 3 arguments
*address of the slave
*memory address within the slave that we want to read from
*pointer of a variable we want to store the data in
*/

void I2C1_ByteRead(char saddress,char maddress,char* data)
{
//check the status register if the bus is busy
//wait
/* generate a start condition*/
//set the start bit
//wait for the start flag to be set
//transmit slave address+write
//wait until the address flag is set
//clear the address flag (read the status register to a temporary variable)
//send the memory address
//wait until the data register is empty


//generate a restart condition
//wait until the start flag is set
//transmit the slave address+read
//wait until the address flag is set
//disable the ack
//clear the address flag (read the status register to a temporary variable)
//generate stop after data received
//wait until RXNE flag is set
//read the data from data register
}
