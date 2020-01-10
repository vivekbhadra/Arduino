char str[4] = {0};

void setup() {
  Serial.begin(9600);
}

void loop() {
  int val;
  val=analogRead(0);   //connect grayscale sensor to Analog 0
  if (val > 50) 
  {
    Serial.println(val,DEC);//print the value to serial
  }
  delay(100);
}
