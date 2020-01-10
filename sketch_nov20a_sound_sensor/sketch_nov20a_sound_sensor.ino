// Item Four—Sound Control Light
int soundPin = 0; // Analog sound sensor is to be attached to analog
int sensorPin = 2;                             // IR sensor to be attached to digital pin NO.2
int ledPin = 13; // Digital Piranha LED-R is to be attached to digital
int ledMotion = 11; // Digital Piranha LED-Y is to be attached to digital
int ledGreen = 9; // Digital Piranha LED-G is to be attached to digital
int buzzPin = 7; // Buzzer 
int sensorState = 0;
int sensorProximity = 4;
int sensorTilt = 6;
int val;  
int lightSensorPin = 3;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(ledMotion, OUTPUT);
  Serial.begin(9600); // You can uncomment this for monitoring
}
void loop(){
  int soundState = analogRead(soundPin); // Read sound sensor’s value
  sensorState = digitalRead(sensorPin);               // Read statistics collected from the sensor
  //Serial.println(soundState); // serial port print the sound sensor’s value
  // if the sound sensor’s value is greater than 10, the light will be on for 10 seconds.
  //Otherwise, the light will be turned off
  if (soundState > 10) {
    digitalWrite(ledPin, HIGH);
    delay(10000);
  }else{
    digitalWrite(ledPin, LOW);
  }

  if (sensorState == HIGH) {      // If it is HIGH, LED light will be turned on
    digitalWrite(ledMotion, HIGH);
    //digitalWrite(buzzPin, HIGH);
  }
  else {                          // If it is LOW, LED light will be turned off
    digitalWrite(ledMotion, LOW);
  }

  //Serial.println(digitalRead(sensorProximity)); // serial port print the sound sensor’s value
  if (digitalRead(sensorProximity) == LOW) {
    //digitalWrite(ledGreen, HIGH);
    Serial.println("Close Proximity: ");
    Serial.println(digitalRead(sensorProximity), BIN); // serial port print the sound sensor’s value
    //digitalWrite(buzzPin, HIGH);
  }
  else {
    Serial.println("No Proximity: ");
    Serial.println(digitalRead(sensorProximity), BIN); // serial port print the sound sensor’s value
    //digitalWrite(ledGreen, LOW);
    //digitalWrite(buzzPin, LOW);
  }

  val = analogRead(lightSensorPin);
  //Serial.println("Light value: ");
  //Serial.println(val);
  if(val > 200.00) {
    //Serial.println("High Light");
    digitalWrite(buzzPin, HIGH);
  } 
  else {
    //Serial.println("Low Light");
    digitalWrite(buzzPin, LOW);
  }
  delay(1000);

  if (digitalRead(sensorTilt) == HIGH) {
    Serial.println("Tilted");
    digitalWrite(ledGreen, HIGH);
  }
  else {
    Serial.println("Not Tilted");
    digitalWrite(ledGreen, LOW);
  }
}
