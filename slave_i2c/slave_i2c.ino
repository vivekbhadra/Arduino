// Chapter 7 - Communications
// I2C Slave
// By Cornel Amariei for Packt Publishing

// Include the required Wire library for I2C
#include <Wire.h>

int LED = 13;
int x = 0;
const int SLAVE_ADDRESS = 42;

void setup() {

  // Start the I2C Bus as Slave on address 9
  Wire.begin(SLAVE_ADDRESS); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  
  Serial.begin(9600);
  // Define the LED pin as Output
  pinMode (LED, OUTPUT);
}

void receiveEvent(int bytes) {
  x = Wire.read();    // read one character from the I2C
  Serial.println(x, HEX);
}

void requestEvent()
{
  Serial.println("requestEvent");
  if(x == 0xde) {
    Wire.write(0xad);
  } else if(x = 0xbe) {
    Wire.write(0xef);  
  }
}

void loop() {
}
