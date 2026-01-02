#include "stm32f446xx.h"

#define Data 	(0x00001FE0UL)	//PC0-PC7
#define RS		(1U<<12) 		//PB12
#define RW		(1U<<13)		//PB13
#define EN		(1U<<15)		//PB15
#define GPIOBEN (1U<<1)
#define GPIOCEN	(1U<<2)

void Delay(unsigned int num);
void GPIO_Init(void);
void LCD_Command(unsigned char cmd);

void LCD_Init(void)
{
	GPIO_Init();
	//Init Sequence
	Delay(30);
	LCD_Command(0x30);
	Delay(10);
	LCD_Command(0x30);
	Delay(1);
	LCD_Command(0x30);

	LCD_Command(0x38); //set to 8 bit data mode,2-line,5x7 font
	LCD_Command(0x06); //move cursor right
	LCD_Command(0x01); //clear screen,move cursor home
	LCD_Command(0x0F); //turn display,blink cursor

}
void GPIO_Init(void)
{
	RCC->AHB1ENR|=GPIOBEN |GPIOCEN;

	GPIOB->MODER |= (1U<<24) |(1U<<26) | (1U<<30);
	GPIOB->MODER &= ~(1U<<25) | ~(1U<<27) | ~(1U<<31);

	GPIOC->MODER |= (1U<<0) | (1U<<2) |(1U<<4) | (1U<<6) |(1U<<8) |(1U<<10) | (1U<<12) |(1U<<14);
	GPIOC->MODER &= ~(1U<<1) | ~(1U<<3) |~(1U<<5) | ~(1U<<7) |~(1U<<9) |~(1U<<11) | ~(1U<<13) |~(1U<<15);

	//set EN and RW low
	GPIOB->BSRR = (EN|RW)<<16;

}

void LCD_Command(unsigned char cmd)
{
	//RS=0 R/W=0
	GPIOB->BSRR = (RS|RW)<<16;
	GPIOC->ODR=cmd;
	GPIOB->BSRR=EN;
	Delay(0);
	GPIOB->BSRR=EN<<16;

}

void LCD_Data(char *data)
{
	GPIOB->BSRR=RS; 	//RS=1
	GPIOB->BSRR=RW<<16;	//RW=0
	GPIOC->ODR=(uint32_t)data;
	GPIOB->BSRR=EN;
	Delay(0);
	GPIOB->BSRR=EN<<16;
}

void Delay(unsigned int num)
{
	int i,j;
	for(i=0;i<num;i++)
		for(j=0;j<1600;j++);
}
