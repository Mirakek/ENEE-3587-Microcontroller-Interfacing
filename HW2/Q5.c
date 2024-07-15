// HW1 Q5

#define F_CPU 16000000UL  // 16 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

int main(void)
{
	DDRA = 0xFF;		// PORTA = GND Output
	DDRB = 0xFF;		// PORTB = 7SD Output
	uint8_t LED_1, LED_2, LED_3, LED_4 = 0;
	uint8_t colon = 0x70;  // 0b1000 0000
	uint8_t SSD[] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B};
	
	unsigned char time[] = "12:30";
	
	LED_1 = time[0] - 0x30; // =1
	LED_2 = time[1] - 0x30; // =2
	LED_3 = time[2] - 0x30; // =3
	LED_4 = time[3] - 0x30; // =0
	
	while(1){
		PORTA &= 0b11111110;
		PORTB = SSD[LED_1];
		_delay_ms(10);
		
		PORTA &= 0b11111101;
		PORTB = SSD[LED_2] + colon;
		_delay_ms(10);
		
		PORTA &= 0b11111011;
		PORTB = SSD[LED_3] + colon;
		_delay_ms(10);
		
		PORTA &= 0b11110111;
		PORTB = SSD[LED_4];
		_delay_ms(10);
		
	}
	return 0;
}


