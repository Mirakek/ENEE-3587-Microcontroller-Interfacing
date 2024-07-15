// HW 3 - Q7

#define F_CPU 16000000L //16MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

// Use volatile variables when dealing with interrupts and concurrent/critical section code
volatile uint16_t rising_edge_time = 0;
volatile uint16_t falling_edge_time = 0;
volatile uint16_t period_time = 0;
volatile uint16_t high_time = 0;
volatile uint8_t duty_cycle = 0;

// Pin 0 interrupt
ISR(INT0_vect) {
	if (PIND & 0x01) {
		
	// Rising edge detected
	TCNT1 = 0;
	rising_edge_time = TCNT1;
		
	} else {
	// Falling edge detected
	falling_edge_time = TCNT1;
	high_time = falling_edge_time - rising_edge_time;
	period_time = falling_edge_time;
		
	// Calculate Duty Cycle
	if (period_time > 0) {
		duty_cycle = (high_time * 100) / period_time;
	}
}

int main(void){
	
	// PD0 = Input
	DDRD &= ~0x01;
	
	// Pull-up PD0
	PORTD |= 0x01;
	
	// External interrupt on any edge
	EICRA |= 0x01;
	
	// Enable external interrupt INT0 by setting bit 0
	EIMSK |= 0x01;

	// Configure Timer1, Normal mode, no prescaler
	TCCR1A = 0x00;
	TCCR1B = 0x01;
	sei();
	
	
	_delay_ms(1000);
	cli(); 
	while(1);
	
}
    