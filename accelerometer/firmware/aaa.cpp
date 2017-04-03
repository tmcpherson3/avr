/* Name: main.cpp
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#define I2CDEV_IMPLEMENTATION I2CDEV_ARDUINO_WIRE

#include <util/delay.h>
#include "Arduino.h"
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <avr/interrupt.h>

// Global Vars
MPU6050 mpu;
Quaternion q;
float ypr[3]; // Yaw, Pitch, Roll
VectorFloat gravity;    // [x, y, z] gravity vector
bool dmpReady = false;
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize = 0;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

volatile bool mpuInterrupt = false;
// Set up interrupt service routine for Digital Pin D2
void dmpDataReady()
{
  mpuInterrupt = true;
}

int main(void)
{
    // Enable Interrupts
    sei();
    
    // Begin Fastwire (I2C driver)
    //Fastwire::setup(400, true);
    Wire.begin();
    TWBR = 24; // 400kHz I2C clock

    Serial.begin(115200);
    Serial.println("Serial Begin");

    Serial.println("Initializing I2C devices...");
    mpu.initialize();

    Serial.println("Testing device connections...");
    Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    
    devStatus = mpu.dmpInitialize();

    // supply your gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);

    // Enable On Chip Sensor Fusion
    mpu.setDMPEnabled(true);

    // enable Arduino Interrupt
    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(0,dmpDataReady,RISING);
    mpuIntStatus = mpu.getIntStatus();
    
    dmpReady = true;

    // getPacketSize for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
    
    // Main Program Loop
    
    while(true)
      {
	// Wait for MPU interrupt or extra packets available
	while(!mpuInterrupt && (fifoCount < packetSize))
	  {
	    // background process goes here
	  }
	// Reset interrupt flag
	mpuInterrupt = false;
	mpuIntStatus = mpu.getIntStatus();

	//Get FIFO Count
	fifoCount = mpu.getFIFOCount();

	//Check For Overflow
	if ((mpuIntStatus & 0x10) || fifoCount == 1024)
	  {
	    mpu.resetFIFO();
	    Serial.println("Fifo overflow!");
	    // Otherwise check for DMP data ready interrupt
	  } else if (mpuIntStatus & 0x02)
	  {
	    // wait for correct available data length (should be short)
	    while(fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

	    // read a packet from FIFO
	    mpu.getFIFOBytes(fifoBuffer,packetSize);

	    // track FIFO count here in case there is > 1 packet available
	    // so more packets can be ready immediately
	    fifoCount -= packetSize;

	    // Get Quaternion
	    mpu.dmpGetQuaternion(&q,fifoBuffer);
	    mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            Serial.print("ypr\t");
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI);
	

	    blinkState = !blinkState;
	    digitalWrite(LED_PIN, blinkState);
	   }
	    
      }
    return 0;
}


