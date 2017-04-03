/* Name: main.cpp
	ADC test
 */

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include "Arduino.h"


int main(void)
{
	DDRA |= 1 << DDB0; // set port B bit 5 for output
	/* Begin Serial */
	Serial.begin(115200);
	
	
	
	for(;;)
	{
		
		
		/* Print Value */
		Serial.println();
		
		_delay_ms(1);
	}
	return 0;
}

