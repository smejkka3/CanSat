/*
  Blink

  TUrns on and Led on for one second and than off for one second

*/
int led = 13;


void setup() {
  pinMode(led, OUTPUT);

}

void loop() {
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
  delay(1000);

}
