#include "stm32f446xx.h"
#include "i2c.h"

/*
 * PB8 I2C1_ SCL
 * PB9 I2C1_ SDA
 */

#define GPIOBEN 			(1U<<1)
#define I2C1EN 				(1U<<21)
#define CR1_SWRST			(1U<<15)
#define I2C_100HZ 			80
#define I2C_STD_RISE_TIME	17
#define CR1_PE				(1U<<0)

#define SR2_BUSY			(1U<<1)
#define CR1_START			(1U<<8)
#define SR1_SB				(1U<<0)
#define SR1_ADDR			(1U<<1)
#define SR1_TXE				(1U<<7)
#define CR1_ACK				(1U<<10)
#define CR1_STOP			(1U<<9)
#define SR1_RXE				(1U<<6)

#define SR1_BTF				(1U<<2)

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
	GPIOB->AFR[1] &= ~(1U<<0);
	GPIOB->AFR[1] &= ~(1U<<1);
	GPIOB->AFR[1] |= (1U<<2);
	GPIOB->AFR[1] &= ~(1U<<3);

	GPIOB->AFR[1] &= ~(1U<<4);
	GPIOB->AFR[1] &= ~(1U<<5);
	GPIOB->AFR[1] |= (1U<<6);
	GPIOB->AFR[1] &= ~(1U<<7);
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
	//enter the i2c reset mode(software reset)
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

/*
 *Create a function to read the byte data with 3 arguments
 *address of the slave
 *memory address within the slave that we want to read from
 *pointer of a variable we want to store the data in
*/

void I2C1_ByteRead(char saddress,char maddress,char* data)
{
	volatile int temp;
//check if the bus is busy
	while(I2C1->SR2 & SR2_BUSY);
/* generate a start condition*/
//set the start bit
	I2C1->CR1 |= CR1_START;
//wait for the start flag to be set
	while(!(I2C1->SR1 & SR1_SB));
//transmit slave address+write
	I2C1->DR = saddress<<1;
//wait until the address flag is set
	while(!(I2C1->SR1 & SR1_ADDR));
//clear the address flag (read the status register to a temporary variable)
	temp=I2C1->SR2;
//send the memory address
	I2C1->DR = maddress;
//wait until the data register is empty
	while(!(I2C1->SR1 & SR1_TXE));

//generate a restart condition
	I2C1->CR1 |= CR1_START;
//wait until the start flag is set
	while(!(I2C1->SR1 & SR1_SB));
//transmit the slave address+read
	I2C1->DR = saddress<< 1 | 1;
//wait until the address flag is set
	while(!(I2C1->SR1 & SR1_ADDR));
//disable the ack
	I2C1->CR1 &=~CR1_ACK;
//clear the address flag (read the status register to a temporary variable)
	temp=I2C1->SR2;
//generate stop after data received
	I2C1->CR1 |= CR1_STOP;
//wait until RXNE flag is set
	while(!(I2C1->SR1 & SR1_RXE));
//read the data from data register
	*data++ = I2C1->DR;
}

/*
  I2C Burst read function
  slave address
  master address
  number of bytes to be read
  pointer to the storage of the data
*/
void I2C1_Burst_Read(char saddress,char maddress,int n,char *data)
{
	volatile int temp;
//check if the bus is busy
	while(I2C1->SR2 & SR2_BUSY);
/* generate a start condition*/
//set the start bit
	I2C1->CR1 |= CR1_START;
//wait for the start flag to be set
	while(!(I2C1->SR1 & SR1_SB));
//transmit slave address + write
	I2C1->DR = saddress<<1;
//wait until the address flag is set
	while(!(I2C1->SR1 & SR1_ADDR));
//clear the address flag (read the status register to a temporary variable)
	temp=I2C1->SR2;
//wait until the transmitter is empty
	while(!(I2C1->SR1 & SR1_TXE));
//send the memory address
	I2C1->DR = maddress;
//wait until the data register is empty
	while(!(I2C1->SR1 & SR1_TXE));

//generate a restart condition
	I2C1->CR1 |= CR1_START;
//wait until the start flag is set
	while(!(I2C1->SR1 & SR1_SB));
//transmit the slave address+read
	I2C1->DR = saddress<<1;
//wait until the address flag is set
	while(!(I2C1->SR1 & SR1_ADDR));
//clear the address flag (read the status register to a temporary variable)
	temp=I2C1->SR2;
//Enable ack
	I2C1->CR1 |=CR1_ACK;

/*
*If n is greater than 0
*check if it's one byte left, disable ack
	Then generate a stop condition
	wait until the receive buffer(RXNe) is not empty
	Read data from DR
*If not, wait until RXNE flag is set
	Read data from DR
	decrement n
*/
	while(n>0U)
	{
		if(n==1)
		{
			I2C1->CR1 &=~CR1_ACK;
			I2C1->CR1 |= CR1_STOP;
			while(!(I2C1->SR1 & SR1_RXE));
			*data++ = I2C1->DR;
			break;
		}
		else
		{
			while(!(I2C1->SR1 & SR1_RXE));
			*data++ = I2C1->DR;
			n--;
		}
	}
}
/*
  I2C Burst write function
  slave address
  master address
  number of bytes to be read
  pointer to the storage of the data
*/
void I2C1_Burst_Write(char saddress,char maddress,int n,char *data)
{
	volatile int temp;
//check if the bus is busy
	while(I2C1->SR2 & SR2_BUSY);
/* generate a start condition*/
//set the start bit
	I2C1->CR1 |= CR1_START;
//wait for the start flag to be set
	while(!(I2C1->SR1 & SR1_SB));
//transmit slave address + write
	I2C1->DR = saddress<<1;
//wait until the address flag is set
	while(!(I2C1->SR1 & SR1_ADDR));
//clear the address flag (read the status register to a temporary variable)
	temp=I2C1->SR2;
//wait until the transmitter is empty
	while(!(I2C1->SR1 & SR1_TXE));
//send the memory address
	I2C1->DR = maddress;
/*
inside the loop
  *wait until the data register is empty
  *transmit memory address
*/
	for(int i=0;i<n;i++)
	{
		while(!(I2C1->SR1 & SR1_TXE));
		*data++ = I2C1->DR;
	}
//wait until transfer finished
	while(!(I2C1->SR1 & SR1_BTF));
//generate stop condition
	I2C1->CR1 |= CR1_STOP;
}





