// HW1 Q1

#define F_CPU 16000000UL  // 16 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

int main(void)
{
	// Configure Port B pins all as output
	DDRB = 0xFF;
	PORTB = 0xFF;
	// Reverse Drive --> All pins are off at logic 1 --> All LEDs except top one are OFF by default
	while (1)
	{
		// Top LED = 0
		// Next LED = 1
		// Bot. LED = 7
		// Loop from 0 to 7 to light LEDs from top to bottom
		for (uint8_t i = 0; i < 8; i++)
		{
			PORTB = ~i;
			_delay_ms(1000);
		}

		// Loop from 6 to 1 to light LEDs from bottom to top
		for (uint8_t i = 6; i > 0; i--)
		{
			PORTB = ~i;
			_delay_ms(1000);
		}
	}

	return 0;
}

