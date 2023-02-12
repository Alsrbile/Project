#include <util/delay.h>
#define CLEAR   0x01    // Clear Display
#define LINE2   0xC0    // 2nd Line Move
#define HOME    0x02    // Cursor Home
#define RSHIFT  0x1C    // Display Right Shift
#define LSHIFT  0x18    // Display Left Shift
#define DISPON  0x0c     // Display On
#define DISPOFF 0x08    // Display Off
#define CURSORON   0x0E    // Cursor On
#define CURSORONBLINK   0x0F    // Cursor On and Blink
void LCD_init(void);
void LCD_String(char str[]);
void Busy(void);
void Command(unsigned char);
void Data(unsigned char);

void LCD_init(void){
	DDRC = 0xFF;    // 포트 C 출력 설정
	PORTC &= 0xFB;    //E = 0;
	// 충분한 지연시간을 통한 안정화 과정
	_delay_ms(15);
	Command(0x20);    // D5=1
	_delay_ms(5);
	Command(0x20);    // D5=1
	_delay_us(100);
	Command(0x20);    // D5=1
	// 초기화 과정
	Command(0x28);    // function set
	Command(0x06);    // entry mode set
	Command(0x01);    // all clear
	Command(0x0c);    // display on
}

void Command(unsigned char byte){
	Busy();
	// 인스트럭션 상위 바이트
	PORTC = (byte & 0xF0);    
	PORTC &= 0xFE;        // RS = 0;
	PORTC &= 0xFD;        // RW = 0;
	_delay_us(1);
	PORTC |= 0x04;        // E = 1;
	_delay_us(1);
	PORTC &= 0xFB;        // E = 0;
	// 인스트럭션 하위 바이트
	PORTC = ((byte<<4) & 0xF0);  
	PORTC &= 0xFE;        // RS = 0;
	PORTC &= 0xFD;        // RW = 0;
	_delay_us(1);
	PORTC |= 0x04;        // E = 1;
	_delay_us(1);
	PORTC &= 0xFB;        // E = 0;
}


void LCD_String(char str[]){
	char *pStr=0;
	pStr = str;
	while(*pStr) Data(*pStr++);
}

void Data(unsigned char byte){
	Busy();
	// 데이터 상위 바이트
	PORTC = (byte & 0xF0);    
	PORTC |= 0x01;        //RS = 1;
	PORTC &= 0xFD;        //RW = 0;
	_delay_us(1);
	PORTC |= 0x04;        //E = 1;
	_delay_us(1);
	PORTC &= 0xFB;        //E = 0;
	// 데이터 하위 바이트
	PORTC = ((byte<<4) & 0xF0);     
	PORTC |= 0x01;        //RS = 1;
	PORTC &= 0xFD;        //RW = 0;
	_delay_us(1);
	PORTC |= 0x04;        //E = 1;
	_delay_us(1);
	PORTC &= 0xFB;        //E = 0;
}

void Busy(void)
{
	_delay_ms(2);
}

void LCDOUT(void){
	LCD_init();
	Command(CLEAR);
	Command(HOME);
	

}


