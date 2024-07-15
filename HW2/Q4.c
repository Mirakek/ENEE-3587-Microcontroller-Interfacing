// HW1 Q4

#define F_CPU 16000000UL  // 16 MHz
#include <avr/io.h>       
#include <util/delay.h>  
#include <stdint.h>		 

// Quad DIP = 4 bit --> 0b0000

int main(void)
{
	DDRA = 0;			// PORTA = DIP4 Input
	DDRB = 0xFF;		// PORTB = 7SD Output
	
	// Unsigned char = uint8 in AVR
	uint8_t SSD[] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B};
	uint8_t DIP4_Value;
	
	while(1){
		DIP4_Value = PINA;
		
		if(DIP4_Value < 10){
			PORTB = SSD[DIP4_Value];
		}
			
			PORTB = 0;
		}
		_delay_ms(10);
	
	return 0;
}


