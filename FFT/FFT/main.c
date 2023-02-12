#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "ffft.h"		/* Defs for using Fixed-point FFT module */
#include "myLCD.h"

#include "note.h"

#define _BV 1<<
#define bit_is_clear(sfr,bit)  (!(_SFR_BYTE(sfr) & _BV(bit)))
#define	SYSCLK		16000000
#define IE0_EICRA (1<<ISC01) //하강인터럽트
#define IE0_EIMSK (1<<INT0)	//PORTD0 이용


#define TIMER0_FAST_PWM TCCR0 |= (1<<WGM00) | (1<<WGM01)
#define TIMER0_COMP_MODE TCCR0 |= 1<<COM01
#define TIMER0_PRESCALER TCCR0 |= 1<<CS02


unsigned int ad_val = 0, tlrks = 0, pcount=0;
unsigned char sec=0;
char pool[16];

int result = 0;
int a=0;


int16_t capture[FFT_N];			/* Wave captureing buffer */
complex_t bfly_buff[FFT_N];		/* FFT buffer */
uint16_t spectrum[FFT_N/2];		/* Spectrum output buffer */
ISR(TIMER0_OVF_vect){ //타이머 인터럽트
	tlrks ++ ;
	TCNT0 = 6;
	if(tlrks >1000){ //1초마다 sec = 1
		sec=1;
		tlrks = 0;
	}
}



ISR(INT0_vect){ //하강 인터럽트 감지
	pcount++;
}



ISR(ADC_vect){
	ad_val = ADC;
}

void capture_wave (int16_t *buffer, uint16_t count)
{
	ADMUX = _BV(REFS0)|_BV(ADLAR)|_BV(MUX2)|_BV(MUX1)|_BV(MUX0);	// channel

	do {
		ADCSRA = _BV(ADEN)|_BV(ADSC)|_BV(ADFR)|_BV(ADIF)|_BV(ADPS2)|_BV(ADPS1)| _BV(ADPS0); //Prescale 128 -> 8us
		while(bit_is_clear(ADCSRA, ADIF)); // 변환완료 대기
		*buffer++ = ADC - 32768;
	} while(--count);

	ADCSRA = 0;
}

int main(){
	unsigned char ffton[]="RUNNING FFT", fftoff[]="STOP    FFT",freql[16]; //LCD출력을 위한 배열
	TIMER0_FAST_PWM;
	TIMER0_COMP_MODE;
	TIMER0_PRESCALER;
	
	DDRA = 0xff; // led
	DDRB = 0xf0; // led
	
	TIMSK |= 1<<TOIE0;
	TCCR0 = 1<<CS02; //prescale 64 -> 4us
	TCNT0 = 6;
	EICRA |= IE0_EICRA;
	EIMSK |= IE0_EIMSK;
	ADMUX = _BV(REFS0)|_BV(ADLAR)|_BV(MUX2)|_BV(MUX1)|_BV(MUX0); //AD기준전압:AVCC
	ADCSRA = 1<< (ADEN)| 1<< (ADSC)|1 <<(ADFR)| 1<< (ADPS2)| 1<< (ADPS1)| 1<< (ADPS0); //prescale 128
	UCSR0B |= 1<< RXEN0 | 1<<TXEN0; // 송신, 수신활성화 0x18
	UCSR0B |= 1<< RXCIE0; // Recive Interrupt enable
	UCSR0C |= 1<< UCSZ00 | 1<< UCSZ01; //데이터 비트수 = 8
	UBRR0L = 25; //Baud Rate 38400
	UCSR1B = 0x18; // 송신, 수신활성화 // 마이컨 통신 핀
	UCSR1C |= 1<< UCSZ10 | 1<< UCSZ11;
	UBRR1L = 3;
	sei();
	
	LCD_init();
	char *cp;
	uint16_t m, n;
	uint16_t s[FFT_N/2][7];
	char k,q=0;
	double specsum = 0,specrate[3];
	unsigned int regi[5];
	unsigned int freq=6,freqa[6],acfreq = 0; //저주파에서의 노이즈 떄문에 freq = 6
	while(1) {
		if(pcount == 0) LCD_display(fftoff,1); //대기상태일때 lcd에 표시
		capture_wave(capture, FFT_N);
		fft_input(capture, bfly_buff);
		fft_execute(bfly_buff);
		fft_output(bfly_buff, spectrum);
		for(n=6;n<FFT_N/2;n++){
			if(spectrum[n]<11) spectrum[n]=0; //10 이하의 값은 노이즈로 판단하여 제거
			if(spectrum[n]<2000 && spectrum[n]>0)
			if(spectrum[freq]<spectrum[n])			freq = n; //피크 주파수 검출
		}
		specsum = spectrum[freq-1]+spectrum[freq]+spectrum[freq+1]; //피크 주파수와 근처 주파수 측정값의 합
		for(int rate = 0; rate<3; rate++){
			specrate[rate] = (spectrum[freq + rate -1]/specsum); //피크 주파수와 근처 주파수의 기여도
		}
		acfreq = ((specrate[0] * (freq -1) * 38)+(specrate[1] * (freq) * 38)+(specrate[2] * (freq +1) * 38)); //정확한 주파수 = 기여도 * 주파수 * 분해능(38) (+38 제거한 부분)
		if(pcount == 1) { //
			LCD_display(ffton,1); //대기 상태
			for (n = 0; n < FFT_N / 2; n++) {	//10의 자릿수로 분해
				regi[0] = ((spectrum[n] / 10000) % 10) + 48;
				regi[1] = ((spectrum[n] / 1000) % 10) + 48;
				regi[2] = ((spectrum[n] / 100) % 10) + 48;
				regi[3] = ((spectrum[n] / 10) % 10) + 48;
				regi[4] = (spectrum[n] % 10) + 48;
				s[n][0] = 32;	//통신창에서 구분하기 쉽게 공백 추가
				s[n][1] = 32;
				if(spectrum[n] < 10000) regi[0] = ' ';
				if(spectrum[n] < 1000) regi[1] = ' ';
				if(spectrum[n] < 100) regi[2] = ' ';
				if(spectrum[n] < 10) regi[3] = ' ';
				if(spectrum[n] < 1) regi[4] = '0';
				s[n][2] = regi[0];
				s[n][3] = regi[1];
				s[n][4] = regi[2];
				s[n][5] = regi[3];
				s[n][6] = regi[4];
				
			}	//for 끝
			
			if(sec == 1){ // !!!! 1초마다 통신
				freqa[0] = 's'; // 시작을 인지시켜주기 위한 코드
				freqa[1] = (acfreq/1000%10) + 0x30;//주파수 통신 // 천자리 + 48? '0'?
				freqa[2] = (acfreq/100%10) + 0x30;	// 백자리
				freqa[3] = (acfreq/10%10) + 0x30;	// 십자리
				freqa[4] = (acfreq%10) + 0x30;		// 일자리
				freqa[5] = 'e'; //끝을 인지시켜주기 위한 코드
				if(acfreq < 1000) freqa[1] = ' ';
				if(acfreq < 100) freqa[2] = ' ';
				if(acfreq < 10) freqa[3] = ' ';
				if(acfreq < 1) freqa[4] = '0';
				for(int l=0;l<4;l++) freql[l] = freqa[l+1];
				freql[4] = 'H';
				freql[5] = 'z';
				for(int l=6;l<16;l++) freql[l] = ' '; //공백 추가
				
				sec=0;	//1초 통신을 위한 초기화
			}// if 끝
			
			q=0;
			
			
			// 아래부터 수정 코드 //
			
			
			a = acfreq;
			result = note(a);
			
			LCD_display(freql,2); //주파수 LCD출력
			
			PORTA=0x00;				// LED 중복으로 안켜지도록 초기화
			PORTB=0x00;				// LED 중복으로 안켜지도록 초기화
			if (result == 1){
				PORTA = 0x01;		//도
				_delay_ms(500);		// 0.5초 대기
			}
			else if (result == 2){
				PORTA = 0x80;		//도#
				_delay_ms(500);
			}
			else if (result == 3){	//레
				PORTA = 0x02;
				_delay_ms(500);
			}
			else if (result == 4){	//레#
				PORTB = 0x10;
				_delay_ms(500);
			}
			else if (result == 5){	//미
				PORTA = 0x04;
				_delay_ms(500);
			}
			else if (result==6){	//파
				PORTA = 0x08;
				_delay_ms(500);
			}
			else if (result==7){	//파#
				PORTB = 0x20;
				_delay_ms(500);
			}	
			else if (result==8){	//솔
				PORTA = 0x10;
				_delay_ms(500);
			}
			else if (result==9){	//솔#
				PORTB = 0x40;
				_delay_ms(500);
			}	
			else if (result==10){	//라
				PORTA = 0x20;
				_delay_ms(500);
			}
			else if (result==11){	//라#
				PORTB = 0x80;
				_delay_ms(500);
			}
			else if (result==12){	//시
				PORTA = 0x40;
				_delay_ms(500);
			}
			else{					
				PORTA = 0x00;
				PORTB = 0x00;
				_delay_ms(500);
			}
			
		}
		
		
		
		if(pcount > 2) {
			pcount=0;
			LCD_clear();//대기상태일때 LCD클리어
		}
		
	}
	
}