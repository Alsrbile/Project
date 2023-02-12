
#include <util/delay.h>
#define CLEAR   0x01    // Clear Display
#define LINE1   0x02    // Line 1
#define LINE2   0xC0    // line 2
#define RSHIFT  0x1C    // Display Right Shift
#define LSHIFT  0x18    // Display Left Shift
#define DISPON  0x0c     // Display On
#define DISPOFF 0x08    // Display Off
void LCD_init(void);    // initialization
void LCD_display(char str[],char line);  //display
void Command(unsigned char);    //set light-background
void Data(unsigned char);     //converse from table of word
void LCD_clear(void) ;
void LCD_init(void)
{
	DDRC = 0xFF;    //Port C Output Settings
	PORTC &= 0xFB;    //E = 0;
	
	// Port C Output Settings
	_delay_ms(15);
	Command(0x20);    // D5=1
	_delay_ms(5);
	Command(0x20);    // D5=1
	_delay_us(100);
	Command(0x20);    // D5=1
	
	// Initialization process
	Command(0x28);    // function set
	Command(0x06);    // entry mode set
	Command(0x01);    // all clear
	Command(0x0c);    // display on
}



void Command(unsigned char byte)
{
	_delay_ms(2);
	
	// Instruction high byte, light in background
	PORTC = (byte & 0xF0);    // data
	PORTC &= 0xFE;        // RS = 0,mean RS=low level D0-D7 instruction code
	PORTC &= 0xFD;        // RW = 0,read from AVR to LCD
	_delay_us(1);
	PORTC |= 0x04;        // E = 1, LCD active
	_delay_us(1);
	PORTC &= 0xFB;        // E = 0, LCD counteract
	// chip enable signal when 1 to 0
	//Instruction low byte
	PORTC = ((byte<<4) & 0xF0);    // data
	PORTC &= 0xFE;        // RS = 0,RS=low level D0-D7 instruction code
	PORTC &= 0xFD;        // RW = 0,read from AVR to LCD
	_delay_us(1);
	PORTC |= 0x04;        // E = 1, LCD active
	_delay_us(1);
	PORTC &= 0xFB;        // E = 0, LCD counteract
}


void LCD_display(char str[],char line) // display "word"
{
	//Command(CLEAR);
	if(line==1) Command(LINE1);
	if(line==2) Command(LINE2);
	char *pStr=0;
	pStr = str;// ham in ngon ma
	while(*pStr) Data(*pStr++);
}
void Data(unsigned char byte)
{
	_delay_ms(2);
	// Data high byte
	PORTC = (byte & 0xF0);    // data
	PORTC |= 0x01;        //RS = 1 mean D0-D7 data
	PORTC &= 0xFD;        //RW = 0;
	_delay_us(1);
	PORTC |= 0x04;        //E = 1 LCD active
	_delay_us(1);
	PORTC &= 0xFB;        //E = 0 LCD counteract
	// Data low byte
	PORTC = ((byte<<4) & 0xF0);      // data
	PORTC |= 0x01;        //RS = 1 mean D0-D7 data
	PORTC &= 0xFD;        //RW = 0 read from AVR to LCD
	_delay_us(1);
	PORTC |= 0x04;        //E = 1 LCD active
	_delay_us(1);
	PORTC &= 0xFB;        //E = 0 LCD counteract
}

void LCD_clear(void) // display "word"
{
	Command(CLEAR);
}
