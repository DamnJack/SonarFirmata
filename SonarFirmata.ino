/*
 * Firmata is a generic protocol for communicating with microcontrollers
 * from software on a host computer. It is intended to work with
 * any host computer software package.
 *
 * To download a host software package, please clink on the following link
 * to open the download page in your default browser.
 *
 * http://firmata.org/wiki/Download
 */

/* Supports as many analog inputs and analog PWM outputs as possible.
 *
 * This example code is in the public domain.
 */
#include <Firmata.h>
#define BUFSIZE 64

byte analogPin = 0;
byte i = 0;
unsigned long value; 
unsigned long buffer[BUFSIZE] = {0};

void analogWriteCallback(byte pin, int value)
{
    if (IS_PIN_PWM(pin)) {
        pinMode(PIN_TO_DIGITAL(pin), OUTPUT);
        analogWrite(PIN_TO_PWM(pin), value);
    }
}

void setup()
{
    Firmata.setFirmwareVersion(0, 1);
    Firmata.attach(ANALOG_MESSAGE, analogWriteCallback);
    Firmata.begin(57600);
}

void loop()
{
    while(Firmata.available()) {
        delay(50);
        Firmata.processInput();
    }
    // do one analogRead per loop, so if PC is sending a lot of
    // analog write messages, we will only delay 1 analogRead
    buffer[i] = analogRead(analogPin);
    i = ++i & 0x07;
    value = avg(buffer);
    
    Firmata.sendAnalog(analogPin, value); 
//    Firmata.sendAnalog(analogPin, analogRead(analogPin)); 
    analogPin = analogPin + 1;
    if (analogPin >= TOTAL_ANALOG_PINS) analogPin = 0;
    
}

unsigned long avg(unsigned long *buf)
{
 byte i;
 unsigned long sum = 0;
 
 for(i = 0; i < BUFSIZE; i++)
   sum += buf[i];
   
 return (unsigned long)(((float) sum)/BUFSIZE + 0.5);
}

