/* Name: main.cpp
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include "Arduino.h"


#define MPU6050 0x68 // MPU6050-6050 address


int main(void)
{
	/* Begin Serial */
	Serial.begin(115200);
	
	/* Set TWI clock to 100 kHz */
	TWBR = 0x48; // attempt to set twi clock to 100 kHz
	
	/* Send Start */
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWEA);
	while (!(TWCR & _BV(TWINT)));
	if ((TWSR & 0xF8) != TW_START) {
		return -1;
	}
	
	/* Send SLA+W */
	TWDR = (MPU6050 << 1) | TW_WRITE; // SLA+W
	TWCR = (1<<TWINT)|(1<<TWEN); // Start transmission
	while (!(TWCR & _BV(TWINT)));
	if ((TWSR & 0xF8) != TW_MT_SLA_ACK) {
		return -1;
	}
	
	/* Set PWR_MGMT_1 to wake up MPU6050-6050 */
	TWDR = 0x6B; // Send address of PWR_MGMT_1
	TWCR = (1<<TWINT)|(1<<TWEN); // Start transmission
	while (!(TWCR & _BV(TWINT)));
	if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
		return -1;
	}
	TWDR = 0; // Set value of PWR_MGMT_1
	TWCR = (1<<TWINT)|(1<<TWEN); // Start transmission
	while (!(TWCR & _BV(TWINT)));
	if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
		return -1;
	}
	
	/* Send Stop condition */
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); 
	_delay_ms(1); // Allow time for stop to send
	
	for(;;)
	{
		/* Send Start */
		TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWEA);
		while (!(TWCR & _BV(TWINT)));
		if ((TWSR & 0xF8) != TW_START) {
			return -1;
		}
		
		/* Send SLA+W */
		TWDR = (MPU6050 << 1) | TW_WRITE; // SLA+W
		TWCR = (1<<TWINT)|(1<<TWEN); // Start transmission
		while (!(TWCR & _BV(TWINT)));
		if ((TWSR & 0xF8) != TW_MT_SLA_ACK) {
			Serial.println("6");
			_delay_ms(200);
			return -1;
		}
		
		/* Set Address in MPU-6050 to read */
		TWDR = 0x3B; // ACCEL_ZOUT_H, address to read at
		TWCR = (1<<TWINT)|(1<<TWEN); // Start transmission
		while (!(TWCR & _BV(TWINT)));
		if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
			return -1;
		}
		
		/* Send Stop condition */
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); 
		_delay_ms(10);
		
		/* Send Start */
		TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWEA);
		while (!(TWCR & _BV(TWINT)));
		if ((TWSR & 0xF8) != TW_START) {
			return -1;
		}
		
		/* Send SLA+R */
		TWDR = (MPU6050 << 1) | TW_READ; // SLA+R
		TWCR = (1<<TWINT)|(1<<TWEN); // Start read
		while (!(TWCR & _BV(TWINT)));
		if ((TWSR & 0xF8) != TW_MR_SLA_ACK) {
			return -1;
		}
		
		/* Read Register(s) */
		int16_t AcZ = 0;
		TWCR = (1<<TWINT)|(1<<TWEN)| _BV(TWEA); // Start Read, ACK enabled
		while (!(TWCR & _BV(TWINT)));
		if ((TWSR & 0xF8) != TW_MR_DATA_ACK) {
			return -1;
		}
		AcZ = TWDR << 8; // Store high byte
		// read next byte
		// MPU-6050 automatically increments address to read from --> ACCEL_ZOUT_L
		TWCR = (1<<TWINT)|(1<<TWEN); // no TWEA--> send NACK after last byte
		while (!(TWCR & _BV(TWINT)));
		if ((TWSR & 0xF8) != TW_MR_DATA_NACK) {
			return -1;
		}
		AcZ |= TWDR; // Store low byte
		
		/* Send Stop */
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); 
		
		/* Print Value */
		//Serial.print("AcX = "); 
		Serial.println(AcZ/16384.0f - 0.07f);
		//Serial.println("g");
		
		
		_delay_ms(1);
	}
	return 0;
}

