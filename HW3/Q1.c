// HW 3 - Q1

#define F_CPU 16000000L //16MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// 1 sec delay
// 16 MHz / 1024 = 15.625 KHz = 15.625 cycles = 1 sec
// Period = 1/15.625 cycles = 0.000064 seconds per count
// 15.625 KHz * 0.000064 = 1 second


void delaySeconds(uint8_t seconds) {
	for (uint8_t i = 0; i < seconds; i++) {
		// 1. Configure Timer 1 (16-bit) with a prescaler of 1024
		TCCR1A = 0;			// Normal mode operation
		TCCR1B = 0;
		TCCR1B = 0x05;		// Set prescaler to 1024
		TCNT1 = 0;

		// 2. Calculate the count value for 1 second delay
		// TCNT = bit max - (delay*F_CPU)/prescaler
		TCNT1 = 65536 - 15625;		
		
		// 3. Clear overflow flag and Polling timer 1 for overflow flag set
		TIFR1 = 1;
		while (!(TIFR1 & 1));

		// 4. Clear the overflow flag
		TIFR1 = 1;
	}
}
