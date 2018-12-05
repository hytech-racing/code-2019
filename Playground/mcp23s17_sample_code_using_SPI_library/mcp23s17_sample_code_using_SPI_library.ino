#include <SPI.h>


const uint8_t ADDR = 0;                   //MCP23S17 address, all 3 ADDR pins are grounded
const uint8_t OPCODE_READ  = 0b01000001;  //MCP23S17 read command 
const uint8_t OPCODE_WRITE  = 0b01000000; //MCP23S17 write command

// the following register adresses are for IOCON.BANK = 0. Refer to the datasheet for addresses when IOCON.BANK = 1.
const uint8_t IODIRA = 0x00; // I/O direction registers; 1 = input, 0 = output
const uint8_t IODIRB = 0x01;
const uint8_t IPOLA = 0x02;  // input polarity registers; 1 = inverted, 0 = not inverted
const uint8_t IPOLB = 0x03;
const uint8_t GPINTENA = 0x04;
const uint8_t GPINTENB = 0x05;
const uint8_t DEFVALA = 0x06;
const uint8_t DEFVALB = 0x07;
const uint8_t INTCONA = 0x08;
const uint8_t INTCONB = 0x09;
const uint8_t IOCON = 0x0A;
const uint8_t GPPUA = 0x0C;  // GPIO pull-up resister registers; 1 = pull-up enabled, 0 = pull-up disabled
const uint8_t GPPUB = 0x0D;
const uint8_t INTFA = 0x0E;
const uint8_t INTFB = 0x0F;
const uint8_t INTCAPA = 0x10;
const uint8_t INTCAPB = 0x11;
const uint8_t GPIOA = 0x12;  // GPIO registers; read this register to read the value from a pin
const uint8_t GPIOB = 0x13;
const uint8_t OLATA = 0x14;  // output latches registers; write to these registers to output from pins
const uint8_t OLATB = 0x15;

const uint8_t CS2 = 10; // pin 10 on Teensy is used as chip select for MCP23S17




// read all the pins from register B
void readExpanderB (uint8_t chipAddress) {
  
  SPI.beginTransaction(SPISettings (SPI_CLOCK_DIV8, MSBFIRST, SPI_MODE0));
  digitalWrite(CS2, LOW);              
  SPI.transfer(OPCODE_READ | chipAddress << 1);           
  SPI.transfer(GPIOB);
  uint8_t reading = SPI.transfer(0);
  Serial.println(reading, BIN);
  digitalWrite(CS2, HIGH);
  SPI.endTransaction();
  
}


// write specified message to register A
void writeExpanderA (uint8_t chipAddress, uint8_t message) {

  SPI.beginTransaction(SPISettings (SPI_CLOCK_DIV8, MSBFIRST, SPI_MODE0));
  digitalWrite(CS2, LOW);
  SPI.transfer(OPCODE_WRITE  | chipAddress << 1);
  SPI.transfer(OLATA);
  SPI.transfer(message);
  digitalWrite(CS2, HIGH);
  SPI.endTransaction();  
  
}


// write a message to specified register (see register addresses above or in the MCP23S17 datasheet)
void writeRegister (uint8_t chipAddress, uint8_t message, uint8_t registerAddress) {

  SPI.beginTransaction(SPISettings (SPI_CLOCK_DIV8, MSBFIRST, SPI_MODE0));
  digitalWrite(CS2, LOW);
  SPI.transfer(OPCODE_WRITE | chipAddress << 1);
  SPI.transfer(registerAddress);
  SPI.transfer(message);             
  digitalWrite(CS2, HIGH);
  SPI.endTransaction();

}


// read specified register and print the reading to serial (see register addresses above or in the MCP23S17 datasheet)
void readRegister (uint8_t chipAddress, uint8_t registerAddress) {

  SPI.beginTransaction(SPISettings (SPI_CLOCK_DIV8, MSBFIRST, SPI_MODE0));
  digitalWrite(CS2, LOW);
  SPI.transfer(OPCODE_READ | chipAddress << 1);
  SPI.transfer(registerAddress);
  uint8_t reading = SPI.transfer(0);
  Serial.println(reading);
  digitalWrite(CS2, HIGH);
  SPI.endTransaction();

}


void setup() 
{
  Serial.begin(9600);

  SPI.begin();

  pinMode(CS2, OUTPUT);     // set the chip select pin on Teensy as output
  digitalWrite(CS2, HIGH);

  // following code writes default values to all registers of the chip except the ones specified with comments; it is neccessary to ensure stable operation of the chip.
  // delay is added to make sure there is no interference between SPI messages
  writeRegister(0, 0x00, IODIRA); // set register A pins as outputs
  delay(10);
  writeRegister(0, 0xFF, IODIRB); 
  delay(10);
  writeRegister(0, 0x00, IPOLA);  
  delay(10); 
  writeRegister(0, 0x00, IPOLB);  
  delay(10);
  writeRegister(0, 0x00, GPINTENA); 
  delay(10);
  writeRegister(0, 0x00, GPINTENB); 
  delay(10);
  writeRegister(0, 0x00, DEFVALA); 
  delay(10);
  writeRegister(0, 0x00, DEFVALB); 
  delay(10);
  writeRegister(0, 0x00, INTCONA); 
  delay(10);
  writeRegister(0, 0x00, INTCONB); 
  delay(10);
  writeRegister(0, 0x00, IOCON);  
  delay(10);
  writeRegister(0, 0x00, GPPUA);  
  delay(10);
  writeRegister(0, 0xFF, GPPUB); // activate pull-ups for all register B pins
  delay(10);
  writeRegister(0, 0x00, INTFA);  
  delay(10);
  writeRegister(0, 0x00, INTFB); 
  delay(10);
  writeRegister(0, 0x00, INTCAPA); 
  delay(10);
  writeRegister(0, 0x00, INTCAPB);  
  delay(10);
  writeRegister(0, 0x00, GPIOA);  
  delay(10);
  writeRegister(0, 0x00, GPIOB); 
  delay(10);
  writeRegister(0, 0x00, OLATA); 
  delay(10);
  writeRegister(0, 0x00, OLATB);  
  delay(10);

}

void loop() {
  
  delay(1); // there needs to be a delay before accessing the same register again. Refer to the datasheet for specific values. Metro library can bbe used in the final code.
  
  readExpanderB(0);
  writeExpanderA(0, 0b10101010);
//  
//  delay(1); 
//  readRegister(0, GPIOB);
//  writeExpanderA(0, 0b10010010);

  
}
