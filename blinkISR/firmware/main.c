/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */
 
#include <avr/io.h>
#include <avr/interrupt.h>


int main(void)
{
    /* insert your hardware initialization here */
    
    // Globally Disable interrupts while setting up
    cli(); 
    
    // Configure PORTB
    DDRB |= 1 << DDB5; // set port B bit 5 for output
    PORTB |= 1 << PORTB5; // initialize bit 5 to 1
    
    // Configure TIMER0 and Output Compare A for interrupts
    TCCR0A = 0b10; // Normal, with clear counter on match
    TCCR0B = 0b101; // Set counter to increment at f_clock/1024 
    TCNT0 = 0; // initialize counter to 0
    OCR0A = 15; // 1041.66 Hz
    TIMSK0 = 0b10; // enable interrupt on output compare A match
    
    // Globally Enable interrupts
    sei(); 
    
    uint8_t counter = 0;
    
    while (1){
    }
    return 0;   /* never reached */
}

ISR(TIMER0_COMPA_vect)
{
	PORTB ^= 1 << PORTB5;
}