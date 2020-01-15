// Chapter 7 - Communications
// I2C Master
// By Cornel Amariei for Packt Publishing

// Include the required Wire library for I2C
#include <Wire.h>

const int SLAVE_ADDRESS = 42;
int x = 250;
int y = 100;
int z = 200;

void setup() {
  Serial.begin(9600);
  // Start the I2C Bus as Master
  Wire.begin(); 

  while(1) 
  {
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(0xde);
    Wire.endTransmission();
    
    if(Wire.requestFrom(SLAVE_ADDRESS, 1) == 0)
    {
      Serial.println("Invalid response from slave\r\n");
    } else { 
      Serial.println("Slave responded\r\n");
  
      if(Wire.available()) {
        byte res = Wire.read();
        if(res == 0xad) {
          Serial.println("Good to go\r\n");
          break;
        }
      }
    }
  }
}

void loop() {
  Wire.beginTransmission(SLAVE_ADDRESS); // transmit to device #9
  Wire.write(x);              // sends x 
  Wire.endTransmission();    // stop transmitting
 
//  x++; // Increment x
//  if (x > 255) x = 250; // `reset x once it gets 6
  Wire.beginTransmission(SLAVE_ADDRESS); // transmit to device #9
  Wire.write(y);              // sends x 
  Wire.endTransmission();    // stop transmitting

  Wire.beginTransmission(SLAVE_ADDRESS); // transmit to device #9
  Wire.write(z);              // sends x 
  Wire.endTransmission();    // stop transmitting

  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(0xbe);
  Wire.endTransmission();

  while(1)
  {
    if(Wire.requestFrom(SLAVE_ADDRESS, 1) == 0)
    {
      Serial.println("Invalid Stop ack\r\n");
    } else {
      if(Wire.available())
      {
        byte res = Wire.read();
        if(res == 0xef) {
          break;
        }
      }
    }
  }
  delay(500);
}
