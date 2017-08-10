#include <Servo.h>

Servo myServo;
int pos = 0;    // variable to store the servo position

void setup() {
  // put your setup code here, to run once:
  myServo.attach(10);
  myServo.write(0);  // set servo to mid-point
}

void loop() {

    myServo.write(180);              // tell servo to go to position in variable 'pos'
    delay(500);                       // waits 15ms for the servo to reach the position

    myServo.write(0);              // tell servo to go to position in variable 'pos'
    delay(500);                       // waits 15ms for the servo to reach the position
}
