/* Name: main.cpp
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include "Arduino.h"


int main(void)
{
	Serial.begin(9600);
	for(;;)
	{
		Serial.write("hello2\n\r");
	}
	return 0;
}

