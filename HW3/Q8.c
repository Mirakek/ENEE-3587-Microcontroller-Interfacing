// HW 3 - Q8

#define F_CPU 16000000L //16MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

// Use volatile variables when dealing with interrupts and concurrent/critical section code
volatile uint16_t frequency = 0;

// Interrupt for PB0 (PCINT0)
ISR(PCINT0_vect) {
    if (PINB & 0x01) {
        frequency++;
    }
}

// Interrupt for PB0 (PCINT20)
ISR(PCINT2_vect) {
    if (PIND & 0x10) {
        frequency--;
    }
}

int main(void){
	
	// PB0, PD4 = Input
	DDRB &= ~0x01;
    DDRD &= ~0x10;
	
	// Pull-ups
	PORTB |= 0x01;
    PORTD |= 0x10;

    // Enable PCINT 2
    PCICR |= 0x05;
	
	//  Pin change interrupt enable
	PCMSK0 |= 0x01;
    PCMSK2 |= 0x10;
	
    sei();
	_delay_ms(1000);
	cli(); 
	while(1);
	
}
    