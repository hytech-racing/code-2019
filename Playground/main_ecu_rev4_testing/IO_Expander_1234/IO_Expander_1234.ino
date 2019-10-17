#include <MCP23S17.h>

#ifdef __PIC32MX__
// chipKIT uses the DSPI library instead of the SPI library as it's better
#include <DSPI.h>
DSPI0 SPI;
#else
// Everytying else uses the SPI library
#include <SPI.h>
#endif

const uint8_t chipSelect = 10;

// Create an object for each chip
// Bank 0 is address 0
// Bank 1 is address 1.
// Increase the addresses by 2 for each BA value.

MCP23S17 Bank1(&SPI, chipSelect, 0);

void setup() {
	Bank1.begin();

	Bank1.pinMode(8, OUTPUT);
  Bank1.pinMode(9, OUTPUT);
  Bank1.pinMode(10, OUTPUT);
  Bank1.pinMode(11, OUTPUT);
  Bank1.pinMode(12, INPUT);
}

void loop() {
  Bank1.digitalWrite(8, 1);
  Bank1.digitalWrite(9, 1);
  Bank1.digitalWrite(10, 1);
  Bank1.digitalWrite(11, 1);
  Bank1.digitalRead(12);
  delay(1000);
  Bank1.digitalWrite(8, 0);
  Bank1.digitalWrite(9, 0);
  Bank1.digitalWrite(10, 0);
  Bank1.digitalWrite(11, 0);
  Bank1.digitalRead(12);
  delay(1000);
}
