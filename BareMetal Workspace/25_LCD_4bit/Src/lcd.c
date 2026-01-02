#include "stm32f446xx.h"


//RW-GND
//Data	//PC4-PC7
#define RS (1U<<12)		//PB12
#define EN		(1U<<15)		//PB15
#define GPIOBEN (1U<<1)
#define GPIOCEN	(1U<<2)

void lcd_nibble_write(char data,uint32_t control);
void Delay(unsigned int num);
void GPIO_Init(void);
void LCD_Command(unsigned char cmd);

void LCD_Init(void)
{
	GPIO_Init();
	//Init Sequence
	Delay(30);
	lcd_nibble_write(0x30,0);
	Delay(5);
	lcd_nibble_write(0x30,0);
	Delay(1);
	lcd_nibble_write(0x30,0);

	lcd_nibble_write(0x20,0); //set to 4 bit data mode
	Delay(1);
	LCD_Command(0x28);//4 bit mode,2 line, 5x7 font
	LCD_Command(0x06); //move cursor right
	LCD_Command(0x01); //clear screen,move cursor home
	LCD_Command(0x0F); //turn display,blink cursor

}
void GPIO_Init(void)
{
	RCC->AHB1ENR|=GPIOBEN |GPIOCEN;

	GPIOB->MODER |= (1U<<24) | (1U<<30);
	GPIOB->MODER &= ~(1U<<25) | ~(1U<<31);

	GPIOC->MODER |= (1U<<8) |(1U<<10) | (1U<<12) |(1U<<14);
	GPIOC->MODER &= ~(1U<<9) |~(1U<<11) | ~(1U<<13) |~(1U<<15);

	//set EN and RW low
	GPIOB->BSRR = EN<<16;

}
void lcd_nibble_write(char data,uint32_t control)
{
	GPIOC->BSRR =0x00F00000;//clear data bits
	GPIOC->BSRR =data&0xF0;
	if(control & RS)
		GPIOB->BSRR = RS;
	else
		GPIOB->BSRR = RS<<16;
	GPIOB->BSRR=EN;
	Delay(0);
	GPIOB->BSRR=EN<<16;

}
void LCD_Command(unsigned char cmd)
{
	lcd_nibble_write(cmd & 0xF0,0); //upper 4 bits
	lcd_nibble_write(cmd<<4,0); //lower 4 bits
	if(cmd<4)
		Delay(2);
	else
		Delay(1);
}

void LCD_Data(char data)
{
	lcd_nibble_write(data & 0xF0,RS);
	lcd_nibble_write(data<<4,RS);
	Delay(1);
}


void LCD_String(char *str)
{
    while(*str)
    {
        LCD_Data(*str); // send one character
        str++;
    }
}


void Delay(unsigned int num)
{
	int i,j;
	for(i=0;i<num;i++)
		for(j=0;j<1600;j++);
}
