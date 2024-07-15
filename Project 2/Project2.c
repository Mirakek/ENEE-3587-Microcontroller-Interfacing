// Project 2

#define F_CPU 16000000L //16MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void setupLCD(void)
{
	DDRF = 0xFF;            //LCD 8bit data
	DDRK = 0x03;            //PTK0= En, PTK1=RS
	
	char2LCD(0x38, 0);      // set 8-bit data, 2-line display, 5x8 font
	char2LCD(0x0F, 0);      // turn on display, cursor, blinking
	char2LCD(0x06, 0);      // move cursor right
	char2LCD(0x01, 0);      // clear screen, move cursor to home
}

char keypad()
{
	char col[4] = {0x70, 0xB0, 0xD0, 0xE0};
	char key[4][4] = {	
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}};

	int r = -1;
	DDRA = 0xF0;
	

	for (int c = 0; c < 4; c++) // enable each column 1 at a time
	{
		PORTA = col[c];
		
		if (PINA != 0x0F)
		{
			_delay_ms(10); //debounce
			switch (PINA & 0x0F)
			{
				case(0b0111): r = 0; break;
				case(0b1011): r = 1; break;
				case(0b1101): r = 2; break;
				case(0b1110): r = 3; break;
				default: break;
			}
			if(r != -1)
			{
				return (key[r][c]);
			}
		}
	}
	return ' ';
}

void char2LCD (unsigned char data, unsigned char IR_DR)
{   PORTK = IR_DR;            // set instr or character
	PORTK = IR_DR|0x01;        // pull E signal to high
	PORTF = data;
	PORTK = IR_DR;
}


void str2LCD (unsigned char *ptr)
{
	while (*ptr)                 //ascii-Z strings (null-terminated)
	{
		char2LCD(*ptr, 2);
		// _delay_ms(500);
		ptr++;
	}
}

// Inputs a char at a specific area of a string to the LED
void input_char(int8_t character, uint8_t *str, int8_t len)
{
	int8_t i;
	for (i = len - 1; i >= 0; i--){
		str[i] = character;
	}
}

// Converts stuff into arrays/strings for LCD output
void output_LCD(uint32_t num, uint8_t *str, int8_t len)
{
	int8_t i;
	for (i = len -1; i >= 0; i--){
		str[i] = (num % 10) + 0x30;
		num /= 10;
	}

}

void add_digit(uint32_t *num, uint32_t *mul, uint8_t key){
	*num += (uint32_t) (key - 0x30) * (*mul);
	*mul /= 10;
}

void delete_digit(uint32_t *num, uint32_t *mul){
	*mul *= 10;
    *num /= *mul;
    *num *= *mul;
}


int32_t pwm_settings(uint32_t *signal_freq){
	int32_t TOP;
	const uint16_t U16_MAX = 65535; // 16 bit limit
	TCCR0A = 0b00100011;
	
	// Extra Credit
	if((F_CPU/ *signal_freq)/1024 < U16_MAX && !(F_CPU%( *signal_freq*1024) > 0)){
		TOP = F_CPU/( *signal_freq*1024);
		TCCR0B = 0b00001101;
	}
	else if((F_CPU/ *signal_freq)/256 < U16_MAX && !(F_CPU%( *signal_freq*256) > 0)){
		TOP = F_CPU/( *signal_freq*256);
		TCCR0B = 0b00001100;
	}
	else if((F_CPU/ *signal_freq)/64 < U16_MAX && !(F_CPU%( *signal_freq*64) > 0)){
		TOP = F_CPU/( *signal_freq*64);
		TCCR0B = 0b00001011;
	}
	else if((F_CPU/ *signal_freq)/8 < U16_MAX && !(F_CPU%( *signal_freq*8) > 0)){
		TOP = F_CPU/( *signal_freq*8);
		TCCR0B = 0b00001010;
	}
	else if(F_CPU/ *signal_freq < U16_MAX && !(F_CPU% *signal_freq > 0)){
		TOP = F_CPU/ *signal_freq;
		TCCR0B = 0b00001001;
	}
	
	// Fast PWM
	else
	{
		if((F_CPU/ *signal_freq)/1024 < U16_MAX)
		{
			TOP = F_CPU/( *signal_freq*1024);
			TCCR0B = 0b00001101;
		}
		else if((F_CPU/ *signal_freq)/256 < U16_MAX)
		{
			TOP = F_CPU/( *signal_freq*256);
			TCCR0B = 0b00001100;
		}
		else if((F_CPU/ *signal_freq)/64 < U16_MAX)
		{
			TOP = F_CPU/(*signal_freq*64);
			TCCR0B = 0b00001011;
		}
		else if((F_CPU/ *signal_freq)/8 < U16_MAX)
		{
			TOP = F_CPU/(*signal_freq*8);
			TCCR0B = 0b00001010;
		}
		else
		{
			TOP = F_CPU/ *signal_freq;
			TCCR0B = 0b00001001;
		}
	}
	return TOP;
}

int main(void){
	DDRG |= 0x20; // configure OC0B pin for PWM output (PG5 is the built-in output pin)

	setupLCD();
	
	// Display Intro
	str2LCD("  Square Wave   ");
	char2LCD(0XC0,0); 
	str2LCD("   Generator    ");
	_delay_ms(1000);
	
	// Display Instructions
	char2LCD(0X01,0);
	str2LCD("    ENTER: *    ");
	char2LCD(0XC0,0);
	str2LCD("    DELETE: #   ");
	_delay_ms(1000);
	
	// Display Max Freq.
	char2LCD(0X01,0);
	str2LCD("     Stop: A    ");
	char2LCD(0XC0,0);
	str2LCD("Max Freq = 10kHz");
	_delay_ms(1000);
	
	// Input Freq.
	char2LCD(0X01,0); 
	str2LCD("Enter Frequency:");
	char2LCD(0XC0,0);
	str2LCD("       Hz       ");
	_delay_ms(1000);
	
	uint8_t *freq_Hz = "       Hz", *duty_percent = "    %    ";
	uint32_t signal_freq, duty_cycle;
	
	// User input logic for freq
	int8_t flag = 0;
	while(flag != 1){
		
		char2LCD(0XC0,0);
		signal_freq = 0;
		uint8_t *string_build = " ";
		int8_t enter_flag = 0;
		uint8_t current_char = 0xC0;
		uint8_t key = ' ';
		uint32_t mul = 100000;
		current_char++;
		
		for(int8_t i = 0; i <= 6 && enter_flag != 1; string_build[0] = " "){
			_delay_ms(100);
			key = ' ';
			while(key == ' '||key == 'B'||key == 'C'||key == 'D'){
				key = keypad();
			}
			
			if(key == '*'){
				enter_flag = 1;
			}
			
			else if(key == '1' && i == 0){
				enter_flag = 1;
				signal_freq = 100000;
			}
			
			else if(key == '#'){
				if(i != 0){
					i--;
					current_char--;	
					char2LCD(current_char-1,0);
					input_char(' ', string_build, 1);
					str2LCD(string_build);
					char2LCD(current_char-1,0);
					delete_digit(&signal_freq, &mul);
			
				}
			}
			else if(key == 'A'){
				char2LCD(0X01,0);
				str2LCD("                ");
				char2LCD(0XC0,0);
				str2LCD("                ");
				exit(0);
			}
			
			else if(key != '0' && i == 0){
				
			}
			
			// Add valid key input to LED
			else if(i != 6){
	
				input_char(key, string_build, 1);
				str2LCD(string_build);
				char2LCD(current_char,0);
				add_digit(&signal_freq, &mul, key);

				current_char++;
				i++;
			}
			
			else{
				// Do nothing in edge cases
			}
			
		}
			
		
		if(signal_freq <= 100000){
			flag = 1;
		}
	}
	
	output_LCD(signal_freq, freq_Hz, 6);
	
	// Max Duty Cycle
	char2LCD(0X01,0);
	str2LCD("Max Duty Cycle =");
	char2LCD(0XC0,0);
	str2LCD("      100 %     ");
	_delay_ms(1000);
	char2LCD(0X01,0);
	
	// Duty Cycle Input Logic
	str2LCD("Enter DutyCycle:");
	char2LCD(0XC0,0);
	str2LCD("    %           ");
	flag = 0;
	
	while(flag != 1){
		char2LCD(0XC0,0);
		uint8_t *string_build = " ";
		int8_t enter_flag = 0;
		uint8_t current_char = 0xC0;
		uint8_t key = ' ';
		uint32_t mul = 100;
		current_char++;
		duty_cycle = 0;
		
		for(int8_t i = 0; i <= 3 && enter_flag != 1; string_build[0] = " "){
			_delay_ms(100);
			key = ' ';
			while(key == ' '||key == 'B'||key == 'C'||key == 'D'){
				key = keypad();
			}
			
			if(key == '*'){
				enter_flag = 1;
			}
			
			else if(key == '1' && i == 0){
				enter_flag = 1;
				duty_cycle = 100;
			}
			
			else if(key == '#'){
				if(i != 0){
					i--;
					current_char--;
					char2LCD(current_char-1,0);
					input_char(' ', string_build, 1);
					str2LCD(string_build);
					char2LCD(current_char-1,0);
					delete_digit(&duty_cycle, &mul);
					
				}
			}
			else if(key == 'A'){
				char2LCD(0X01,0);
				str2LCD("                ");
				char2LCD(0XC0,0);
				str2LCD("                ");
				exit(0);
			}
			
			else if(key != '0' && i == 0){
				
			}
			
			else if(i != 3){
				input_char(key, string_build, 1);
				str2LCD(string_build);
				char2LCD(current_char,0);
				add_digit(&duty_cycle, &mul, key);

				current_char++;
				i++;
			}
			
			else{
				// Do nothing in edge cases
			}
			
		}
		
		if(duty_cycle <= 100){
			flag = 1;
		}
	}
	
	// Display Final Values
	output_LCD(duty_cycle, duty_percent, 3);
	_delay_ms(100);
	char2LCD(0x01,0);
	str2LCD("Freq = ");
	str2LCD(freq_Hz);
	char2LCD(0xC0,0);
	str2LCD("Duty = ");
	str2LCD(duty_percent);
	

	int32_t TOP = pwm_settings(&signal_freq);
	int32_t duty_high = TOP*duty_cycle/100;
	TCNT0 = 0; 
	OCR0A = TOP-1;
	OCR0B = duty_high;
	while(1);
	
}
