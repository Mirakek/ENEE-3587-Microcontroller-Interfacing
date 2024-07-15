// HW1 Q2

#define F_CPU 16000000UL  // 16 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// PT0 (Default) = 0b1111
// PT1 (Button A)   = 0b1101
// PT2 (Button B)   = 0b1011
// PT3 (Button C)   = 0b0111

int main(void)
{
	DDRB = 0;				// Input
	PORTB = 0x0F;			// Pullup Setup (everything is off at logic 1)
	uint8_t Button_A = 0;
	uint8_t Button_B = 0;
	uint8_t Button_C = 0;
	
	while (1)
	{
		switch(PINB){
			case 0b1101:
			Button_A = 1;
			break;
			
			case 0b1011:
			Button_B = 1;
			break;
			
			case 0b0111:
			Button_C = 1;
			break;
			
			// Reset Button values if no button pressed
			default:
			Button_A = 0;
			Button_B = 0;
			Button_C = 0;
			break;
		}
		_delay_ms(50);

	}
	return 0;
}


