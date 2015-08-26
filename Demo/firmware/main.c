/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    /* insert your hardware initialization here */
    DDRB |= 1 << DDB5;
    PORTB |= 1 << PORTB5;
    for(;;){
        /* insert your main loop code here */
        uint8_t i;
        for (i = 0; i < 100; i++){
        	_delay_ms(10);
        }
        PORTB ^= 1 << PORTB5;
    }
    return 0;   /* never reached */
}
