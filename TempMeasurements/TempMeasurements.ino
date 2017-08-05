const int analogInPin = A0; // Analog input pin
float tempSensorValue = 0; // Temperature sensor data


void setup() {
  // initialize digital pin 13 as an output
  pinMode(13,OUTPUT);

  // initialize serial communication at 9600 bps
  Serial.begin(9600);

}

void loop() {
  // turn the led on (HIGH is the voltage level)
  digitalWrite(13,HIGH);
  // read ADC value
  tempSensorValue = analogRead(analogInPin);
  //concert temperature data
  tempSensorValue = tempSensorValue * 5000/1023 * 1/10;
  //transmit remperature sensor data, 1 decimal place
  Serial.print("Temp: "); Serial.print(tempSensorValue,1); Serial.println(" [degrees C].");

  delay(500);
  //turn the LED off bt making the voltage LOW
  digitalWrite(13,LOW);

  delay(500);

}
