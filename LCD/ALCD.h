#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
//The pins used are same as explained earlier
#define lcd_port PORTD
#define lcd_port_dir DDRD   

//LCD Registers addresses
#define LCD_EN      0x80// 1000 0000
#define LCD_RS      0x20// 0010 0000
void lcd_reset();
void lcd_init (unsigned char rows);
void lcd_cmd (char cmd);
void lcd_data (unsigned char dat);
void lcd_gotoxy(char posX,char posY);
void lcd_clear(void);
void lcd_putchar(unsigned char data);
void lcd_puts( char *str);
void lcd_putconsts(const char *str);
