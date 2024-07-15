#include <avr/io.h>
#include <stdint.h>

int main(void) {
    
    DDRL |= 0x08; // Set PTL3 for output
    
    TCCR5A |= 0x22; // Fast PWM mode, clear OC5B compare match
    TCCR5B |= 0x1D; // Fast PWM mode, prescaler is 1024
    

    ICR5 = 1561; // TOP = (16000000/1024*10) -1 = 1561.5
    
    OCR5A = 312; // Duty cycle = 1561*0.2 = 312.2

    while (1);
}

