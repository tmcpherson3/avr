// AccelDriver.cpp

#define MPU6050 0x68 // MPU6050-6050 address
#define ACCEL_SENS 16384.0f // Accelerometer Sensitivity LSB/g

#include "AccelDriver.h"
#include <avr/io.h>
#include <util/twi.h>
#include <stdint.h>
#include <util/delay.h>
#include "Arduino.h"

AccelDriver::AccelDriver()
{
  initialize();
}

AccelDriver::~AccelDriver()
{
}

void AccelDriver::initialize()
{
    /* Set TWI clock to 100 kHz */
    TWBR = 0x48; // attempt to set twi clock to 100 kHz
	
    /* Send Start */
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWEA);
    while (!(TWCR & _BV(TWINT)));
    if ((TWSR & 0xF8) != TW_START) {
	    error();
    }

    /* Send SLA+W */
    TWDR = (MPU6050 << 1) | TW_WRITE; // SLA+W
    TWCR = (1<<TWINT)|(1<<TWEN); // Start transmission
    while (!(TWCR & _BV(TWINT)));
    if ((TWSR & 0xF8) != TW_MT_SLA_ACK) {
	    error();
    }

    /* Set PWR_MGMT_1 to wake up MPU6050-6050 */
    TWDR = 0x6B; // Send address of PWR_MGMT_1
    TWCR = (1<<TWINT)|(1<<TWEN); // Start transmission
    while (!(TWCR & _BV(TWINT)));
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
	    error();
    }
    TWDR = 0; // Set value of PWR_MGMT_1
    TWCR = (1<<TWINT)|(1<<TWEN); // Start transmission
    while (!(TWCR & _BV(TWINT)));
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
	    error();
    }

    /* Send Stop condition */
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); 
    _delay_ms(1); // Allow time for stop to send
}

void AccelDriver::error()
{
     // Do Nothing for now
  Serial.println("error");
}

AccelData AccelDriver::readData()
{
  /* Read all data from accelerometer:
     Acceleration X,Y,Z
     Temperature 
     Gyroscope X,Y,Z
     
     These reside in registers 0x3B through 0x48. Each value has
     a high and low register to make a 16 bit value.
  */
  
     /* Send Start */
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWEA);
    while (!(TWCR & _BV(TWINT)));
    if ((TWSR & 0xF8) != TW_START) {
	    error();
    }

    /* Send SLA+W */
    TWDR = (MPU6050 << 1) | TW_WRITE; // SLA+W
    TWCR = (1<<TWINT)|(1<<TWEN); // Start transmission
    while (!(TWCR & _BV(TWINT)));
    if ((TWSR & 0xF8) != TW_MT_SLA_ACK) {
	    error();
    }

    /* Write Address in MPU-6050 to read */
    TWDR = 0x3B; // ACCEL_XOUT_H, address to start read at
    TWCR = (1<<TWINT)|(1<<TWEN); // Start transmission
    while (!(TWCR & _BV(TWINT)));
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
	    error();
    }

    /* Send Stop condition */
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); 
    _delay_ms(1);

     /* Send Start */
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWEA);
    while (!(TWCR & _BV(TWINT)));
    if ((TWSR & 0xF8) != TW_START) {
	    error();
    }

    /* Send SLA+R */
    TWDR = (MPU6050 << 1) | TW_READ; // SLA+R
    TWCR = (1<<TWINT)|(1<<TWEN); // Start read
    while (!(TWCR & _BV(TWINT)));
    if ((TWSR & 0xF8) != TW_MR_SLA_ACK) {
	    error();
    }

    /* Read Register(s) */
    AccelData data;

    int16_t tempData = 0;
    tempData = readNextByte() << 8;   // ACCEL_XOUT_H
    tempData |= readNextByte();       // ACCEL_XOUT_L
    data.Ax = static_cast<double>(tempData) / ACCEL_SENS;
    
    tempData = readNextByte() << 8;   // ACCEL_YOUT_H
    tempData |= readNextByte();       // ACCEL_YOUT_L
    data.Ay = tempData / ACCEL_SENS;
    
    tempData = readNextByte() << 8;   // ACCEL_ZOUT_H
    tempData |= readNextByte();       // ACCEL_ZOUT_L
    data.Az = tempData / ACCEL_SENS;
    
    tempData = readNextByte() << 8; // TEMP_OUT_H
    tempData |= readNextByte();     // TEMP_OUT_L
    data.Temp = tempData; // ACCEL_SENS;
    
    tempData = readNextByte() << 8;   // GYRO_XOUT_H
    tempData |= readNextByte();       // GYRO_XOUT_L
    data.Gx = tempData;// / ACCEL_SENS;
    
    tempData = readNextByte() << 8;   // GYRO_YOUT_H
    tempData |= readNextByte();       // GYRO_YOUT_L
    data.Gy = tempData;/// ACCEL_SENS;
    
    tempData = readNextByte() << 8;   // GYRO_ZOUT_H
    // GYRO_ZOUT_L, need to send NACK after last byte read
    TWCR = (1<<TWINT)|(1<<TWEN); // no TWEA--> send NACK after last byte
    while (!(TWCR & _BV(TWINT)));
    if ((TWSR & 0xF8) != TW_MR_DATA_NACK) {
	    error();
    }
    tempData |= TWDR; // GYRO_ZOUT_L
    data.Gz = tempData;// / ACCEL_SENS;

    /* Send Stop */
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
    _delay_ms(1);

    return data;
}


int16_t AccelDriver::readNextByte()
{
    // read next byte from MPU 6050. Must have already sent SLA+R to use this.
    TWCR = (1<<TWINT)|(1<<TWEN)| _BV(TWEA);
    while (!(TWCR & _BV(TWINT)));
    if ((TWSR & 0xF8) != TW_MR_DATA_ACK) {
	    error();
    }
    int16_t byte = TWDR; // Store byte
    return byte;
}

