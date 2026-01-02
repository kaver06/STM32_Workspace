/*
 * lcd.h
 *
 *  Created on: Aug 11, 2025
 *      Author: kaver
 */

#ifndef LCD_H_
#define LCD_H_

void LCD_Data(char *data);
void LCD_Command(unsigned char cmd);
void Delay(unsigned int num);
void LCD_Init(void);
void lcd_nibble_write(char data,uint32_t control);
void LCD_String(char *str);


#endif /* LCD_H_ */
