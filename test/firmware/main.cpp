/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <util/delay.h>
extern "C"{
#include "func.h"
}

int main(void)
{
    /* insert your hardware initialization here */
    DDRB |= 1 << DDB5;
    PORTB = 0;
	int result;
	result = addTwoInts(2, 2);
	uint8_t i = 0;
    for(i = 0;i < result;i++){
    //for(;;){
    	PORTB ^= 1 << PORTB5;
    	uint8_t n;
        for (n = 0; n < 100; n++){
        	_delay_ms(5);
        }
        PORTB = 0;
        for (n = 0; n < 100; n++){
        	_delay_ms(5);
        }
        /* insert your main loop code here */
    }
    return 0;   /* never reached */
}
