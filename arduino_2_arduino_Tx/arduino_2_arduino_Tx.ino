#include <SoftwareSerial.h>

SoftwareSerial mySerial(6, 7);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(6, INPUT);
  pinMode(7, OUTPUT);
}

void loop() {
  int temp = 10;
  int accX = 20;
  int accY = 30; 
  int accZ = 40; 
  bool isOpen = 0; 
  bool isTilted = 0;
  bool wait_for_synch = 0;

  Serial.println("Flusing the SW serial\n");
  mySerial.flush();

  Serial.println("Waiting for syncd word\n");
  /* wait for slave to get ready */
  while(!wait_for_synch)
  {
    char synch_word[8] = {0xde, 0xad, 0xbe, 0xef};
    int idx;
    while(!mySerial.available())
      ;
 
    for(idx = 0; idx < 4; ++idx)
      {
        if(synch_word[idx] != mySerial.read()) {
          wait_for_synch = 1;
          break;
        } 
      }
  }

  Serial.println("Flushing SW serial\n");
  mySerial.flush();

  if(mySerial.available())
    Serial.println("Bytes on serial after flush!!!");
  Serial.println("Synch received from slave\n");
  Serial.println("Writing payload bytes\n");
  mySerial.write(temp);
  mySerial.write(accX);
  mySerial.write(accY);
  mySerial.write(accZ);
  mySerial.write(isOpen);
  mySerial.write(isTilted);

  //delay(100);
  Serial.println("Writing for stop bytes from slave\n");
  while(1) 
  {
    while(!mySerial.available())
        ;
    Serial.println(mySerial.read());
    if(mySerial.read() == 0x30)
      break;
  }
}
