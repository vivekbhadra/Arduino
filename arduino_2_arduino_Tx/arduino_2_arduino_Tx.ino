#include <SoftwareSerial.h>

SoftwareSerial mySerial(6, 7);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(6, INPUT);
  pinMode(7, OUTPUT);
}

void loop() {

  /*
  int val;
  val=analogRead(0);   //connect grayscale sensor to Analog 0
  if (val > 50) 
  {
    val &= 0xFF;
    char var = val;
    Serial.write(var);//print the value to serial
  }
  delay(100);
  */
  int temp = 10;
  int accX = 20;
  int accY = 30; 
  int accZ = 40; 
  bool isOpen = 0; 
  bool isTilted = 0;

  /* wait for slave to get ready */
  mySerial.flush();
  while(!mySerial.available())
     ;
  
  /* just wait */
  Serial.println("Synch received from slave\n");
  mySerial.write(temp);
  mySerial.write(accX);
  mySerial.write(accY);
  mySerial.write(accZ);
  mySerial.write(isOpen);
  mySerial.write(isTilted);

  delay(100);
  
  while(!mySerial.available())
        ;
    /* just wait */
  Serial.println("Data available\n");
  if(mySerial.available()) 
  {
    int val = mySerial.read();
    Serial.println(val);
  }
}
