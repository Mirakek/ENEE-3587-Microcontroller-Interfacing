#include <avr/io.h>
#include <stdint.h>

uint16_t signal1, signal2;
int16_t phase_difference;

int main(void) {
    
    DDRL &= ~0x03; // Set PTL0 and PTL1 to 0 for input

    TCCR4A = 0x00; // Set Timer 4 to normal mode
    TCCR4B = 0x42; // Capture on rising edge, prescaler = 8

    TCCR5A = 0x00; // Set Timer 5 to normal mode
    TCCR5B = 0x42; // Capture on rising edge, prescaler = 8

    while (1) {
       
        while(!(TIFR4 & 0x20));  // Wait for rising edge for Timer 4
        signal1 = ICR4;
        TIFR4 = 0x20; // Clear flag

        while(!(TIFR5 & 0x20)); // Wait for rising edge for Timer 5
        signal2 = ICR5;
        TIFR5 = 0x20; // Clear flag

        phase_difference = signal1 - signal2; // Calculate phase difference

        if (phase_difference < 0) {
            phase_difference = -phase_difference; // Obtain the absolute value for phase difference
        }
    }
}


