
#include <avr/io.h>
#include <stdint.h>

uint16_t T1, T2, T3;
uint16_t duty_cycle;

int main(void) {

    DDRL &= 0xFE; // Set PL0 as input (ICP4)
    TCCR4A = 0x00; // Set Timer 4 to normal mode
    TCCR4B = 0x43; // Capture on rising edge, prescaler = 64
    TIFR4 = 0x2F;  // Clear all flags related to Timer 4
    TCNT4 = 0; // Count up from 0

    // Wait for 1st rising edge
    while(!(TIFR4 & 0x20));
    T1 = ICR4;
    TIFR4 = 0x20; // Clear input capture flag

    // ICES4=0 to capture falling edge
    TCCR4B &= ~0x40;
    TIFR4 = 0x20; 

    // Wait for falling edge
    while(!(TIFR4 & 0x20));
    T2 = ICR4;
    TIFR4 = 0x20; 

    // ICES4=1 to capture rising edge
    TCCR4B |= 0x40;
    TIFR4 = 0x20; 

    // Wait for 2nd rising edge
    while(!(TIFR4 & 0x20));
    T3 = ICR4;
    TCCR4B = 0x00; // Stop Timer 4
    TIFR4 = 0x20; 

    // Calculate duty cycle
    duty_cycle = ((uint16_t)(T2 - T1) / (uint16_t)(T3 - T1)) * 100;

    while(1);
}




