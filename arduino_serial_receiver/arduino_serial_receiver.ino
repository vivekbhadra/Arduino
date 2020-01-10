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

  /* unblock the master so that data flow starts */
  /* write synch byte on the serial for the master to start transmission */
  mySerial.write(20);
  // put your main code here, to run repeatedly:
  Serial.println("Waiting for data\n");
  while(!mySerial.available())
          ;
  /* just wait */
  
  while(mySerial.available())
  {
    int val = mySerial.read();
    Serial.println(val);
  }
  mySerial.write(20);
}
