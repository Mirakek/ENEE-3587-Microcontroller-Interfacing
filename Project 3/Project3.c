// Project 3

#define F_CPU 16000000L //16MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
volatile uint16_t tovCnt = 0;	// use volatile for interrupt variables

void setupLCD(void)
{
	DDRF = 0xFF;            //LCD 8bit data
	DDRK = 0x03;            //PTK0= En, PTK1=RS
	
	char2LCD(0x38, 0);      // set 8-bit data, 2-line display, 5x8 font
	char2LCD(0x0F, 0);      // turn on display, cursor, blinking
	char2LCD(0x06, 0);      // move cursor right
	char2LCD(0x01, 0);      // clear screen, move cursor to home
}

void char2LCD (unsigned char data, unsigned char IR_DR)
{   PORTK = IR_DR;            // set instr or character
	PORTK = IR_DR|0x01;        // pull E signal to high
	PORTF = data;
	PORTK = IR_DR;
}


void str2LCD (unsigned char *ptr)
{
	while (*ptr)                 //ascii-Z strings (null-terminated)
	{
		char2LCD(*ptr, 2);
		// _delay_ms(500);
		ptr++;
	}
}

// Inputs a char at a specific area of a string to the LED
void input_char(int8_t character, uint8_t *str, int8_t len)
{
	int8_t i;
	for (i = len - 1; i >= 0; i--){
		str[i] = character;
	}
}

// Converts stuff into arrays/strings for LCD output
void output_LCD(uint32_t num, uint8_t *str, int8_t len)
{
	int8_t i;
	for (i = len -1; i >= 0; i--){
		str[i] = (num % 10) + 0x30;
		num /= 10;
	}

}

ISR (TIMER4_OVF_vect) {
	tovCnt++;
}

int main(void)
{	
	setupLCD();
	
	uint16_t period, duty_cycle, time_start, time_end;
	TCCR4A = 0;
	DDRL &= 0XFE;
	TIFR4 = 0X2F;
	TIMSK4 = 0X01;
	sei();			
	while(1)
	{
		str2LCD("Period: "); 
		str2LCD("      us"); 
		char2LCD(0XC0,0);
		str2LCD("Duty Cycle:    %"); 
		
		// Period Calculation
		TCCR4B = 0X43;	// Rising Edge Flag
		TIFR4 = 0X21;	// clear ICF4 and OVF
		TCNT4 = 0;
		tovCnt = 0;     // Reset overflow counter
		while(!(TIFR4 & 0x20)); // 1st Rising Edge
		uint16_t period_start = tovCnt * 65536 + ICR4;

		TIFR4 = 0x21;	// clear ICF4 and OVF
		tovCnt = 0;     // Reset overflow counter
		while(!(TIFR4 & 0x20)); // 2nd Rising Edge
		uint16_t period_end = tovCnt * 65536 + ICR4;
		uint16_t period = period_end - period_start;

		// Duty Cycle Calculation
		TIFR4 = 0x21;
		TCNT4 = 0;
		tovCnt = 0; // Reset overflow counter
		while(!(TIFR4 & 0x20)); // 1st Rising Edge
		uint16_t time_start = tovCnt * 65536 + ICR4;

		TIFR4 = 0x21; // clear ICF4 and OVF
		tovCnt = 0;   // Reset overflow counter
		TCCR4B = 0x03; // Falling Edge Flag
		while(!(TIFR4 & 0x20)); // Falling Edge
		uint16_t time_end = tovCnt * 65536 + ICR4;
		uint16_t duty_cycle = time_end - time_start;

		// Duty Cycle and Period
		duty_cycle = (duty_cycle * 100) / period;
		period = (period * 64) / 16;
		output_LCD(period,"      us",5);
		output_LCD(duty_cycle,"   %",3);
		_delay_ms(1000);
		char2LCD(0X01,0);
	}
	
}





