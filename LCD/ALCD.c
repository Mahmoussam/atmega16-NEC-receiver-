#include "ALCD.H"
char rowCount;
char lastPOSy;
void lcd_reset()
{
	    lcd_port_dir=0XFF;
        lcd_port = 0xFF;
        _delay_ms(20);
        lcd_port = 0x03+LCD_EN;
        lcd_port = 0x03;
        _delay_ms(10);
        lcd_port = 0x03+LCD_EN;
        lcd_port = 0x03;
        _delay_ms(1);
        lcd_port = 0x03+LCD_EN;
        lcd_port = 0x03;
        _delay_ms(1);
        lcd_port = 0x02+LCD_EN;
        lcd_port = 0x02;
        _delay_ms(1);
}

void lcd_init (unsigned char rows)
{
	rowCount=rows;
	lastPOSy=0;
        lcd_reset();         // Call LCD reset
        lcd_cmd(0x28);       // 4-bit mode - 2 line - 5x7 font. 
        lcd_cmd(0x0c);       // Display no cursor - no blink.
        lcd_cmd(0x06);       // Automatic Increment - No Display shift.
        lcd_cmd(0x80);       // Address DDRAM with 0 offset 80h.
 }
 void lcd_cmd (char cmd)
{
        lcd_port = ((cmd >> 4) & 0x0F)|LCD_EN;
        lcd_port = ((cmd >> 4) & 0x0F);

        lcd_port = (cmd & 0x0F)|LCD_EN;
        lcd_port = (cmd & 0x0F);

        _delay_us(200);
        _delay_us(200);
}

void lcd_data (unsigned char dat)
{
        lcd_port = (((dat >> 4) & 0x0F)|LCD_EN|LCD_RS);
        lcd_port = (((dat >> 4) & 0x0F)|LCD_RS);
       
        lcd_port = ((dat & 0x0F)|LCD_EN|LCD_RS);
        lcd_port = ((dat & 0x0F)|LCD_RS);

        _delay_us(200);
        _delay_us(200);
        }
void lcd_gotoxy(char posX,char posY)
{
    if (posY==0 ) 
        lcd_cmd((1<<7)+0x00+posX);
    else
	{
        lcd_cmd((1<<7)+0x40+posX);
	lastPOSy=1;
	}		
		_delay_us(40);
}

void lcd_clear(void)
{
	lcd_cmd(0x01);
	_delay_ms(2);
	lcd_cmd(0x02);
	_delay_ms(2);
}

void lcd_putchar(unsigned char data)
{
	lcd_data(data);
}

void lcd_puts( char *str)
{
	unsigned char i=0;
	
	while(*(str+i)!='\0'&&i<rowCount)
	{
	lcd_data(str[i]);
	i++;	
	}
	if (i>rowCount-1&&lastPOSy==0)
	{
		i=rowCount;
		lcd_gotoxy(0,1);
	
	while(*(str+i)!='\0')
	{
	lcd_data(str[i]);
	i++;	
	}
	}
	
}

void lcd_putconsts(const char *str)
{
	unsigned char i=0;
	
	while(*(str+i)!='\0'&&i<rowCount)
	{
	lcd_data(str[i]);
	i++;	
	}
	if (i>rowCount-1&&lastPOSy==0)
	{
		i=rowCount;
		lcd_gotoxy(0,1);
	
	while(*(str+i)!='\0')
	{
	lcd_data(str[i]);
	i++;	
	}
	}
}
