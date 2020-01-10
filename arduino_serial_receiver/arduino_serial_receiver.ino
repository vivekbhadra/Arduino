#include <SoftwareSerial.h>

SoftwareSerial mySerial(6, 7);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(6, INPUT);
  pinMode(7, OUTPUT);
  mySerial.begin(9600);
}

void loop() {

  mySerial.flush();
  /* unblock the master so that data flow starts */
  /* write synch byte on the serial for the master to start transmission */
  mySerial.write(0xde);
  mySerial.write(0xad);
  mySerial.write(0xbe);
  mySerial.write(0xef);
  
  // put your main code here, to run repeatedly:
  Serial.println("Waiting for payload bytes\n");
  while(!mySerial.available())
          ;
  /* just wait */
  Serial.println("Reading Payload bytes\n");
  while(mySerial.available())
  {
    int val = mySerial.read();
    Serial.println(val);
  }
  Serial.println("Flushing SW Serail\n");
  mySerial.flush();
  Serial.println("Writing the stop byte\n");
  //mySerial.write(0x30);
}
