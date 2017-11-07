/*
  Digital Pot Control using software SPI

  This example controls an Analog Devices AD5206 digital potentiometer.
  The AD5206 has 6 potentiometer channels. Each channel's pins are labeled
  A - connect this to voltage
  W - this is the pot's wiper, which changes when you set it
  B - connect this to ground.

 The AD5206 is SPI-compatible,and to command it, you send two bytes,
 one with the channel number (0 - 5) and one with the resistance value for the
 channel (0 - 255).

 The circuit:
  * All A pins  of AD5206 connected to +5V
  * All B pins of AD5206 connected to ground
  * An LED and a 220-ohm resisor in series connected from each W pin to ground
  * CS -  to digital pin 3
  * SDI - to digital pin 0 (MOSI pin)
  * CLK - to digital pin 2 (SCK pin)

 Created 10 Aug 2010
 by Tom Igoe

 Modified by MCUdude to support ATtiny13
*/


// Include the TinySPI library:
#include <TinySPI.h>


// Set pin 10 as the slave select for the digital pot:
const uint8_t slaveSelectPin = 3;


void setup() 
{  
  pinMode(slaveSelectPin, OUTPUT); // Set the slaveSelectPin as an output
  SPI.begin(); // Initialize SPI
}


void loop() 
{
  for (uint8_t channel = 0; channel < 6; channel++)  // Go through the six channels of the digital pot
  {
    // Change the resistance on this channel from min to max
    for (uint8_t level = 0; level < 255; level++) 
    {
      digitalPotWrite(channel, level);
      delay(10);
    }
    
    delay(1000); // Wait a second at the top
    
    // Change the resistance on this channel from max to min
    for (uint8_t level = 0; level < 255; level++) 
    {
      digitalPotWrite(channel, 255 - level);
      delay(10);
    }
  }
}

void digitalPotWrite(int address, int value) 
{  
  digitalWrite(slaveSelectPin, LOW);  // Pull the SS pin low to select the chip
  SPI.transfer(address);              // Send in the address via SPI
  SPI.transfer(value);                // Send the value via SPI
  digitalWrite(slaveSelectPin, HIGH); // Pull the SS pin high to de-select the chip
}