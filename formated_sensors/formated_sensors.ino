#include<Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h> 
#include <Servo.h>
#include <SD.h>

// MPU-6050 constants
const int MPU_6050_addr = 0x68;
const byte PWR_MGMT_1 = 0x6B;
const byte ACCEL_CONFIG = 0x1C;
const byte ACCEL_XOUT_H = 0x3B;
const byte GYRO_CONFIG = 0x1B;
long time;

//variables MPU-6050
float MPU_6050_AccX;
float MPU_6050_AccY;
float MPU_6050_AccZ;

boolean ejected = false;
float magnitude;
float pressure = 0;
float previous_height_1 = 0;
float previous_height_2 = 0;
float previous_height_3 = 0;

float MPU_6050_Temp;

float MPU_6050_GyroX;
float MPU_6050_GyroY;
float MPU_6050_GyroZ;

//BMP280
Adafruit_BMP280 bme;
Servo myServo;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_6050_addr);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);    // set to zero (wakes up the MPU-6050)
  Wire.endTransmission();

  Wire.beginTransmission(MPU_6050_addr);
  Wire.write(ACCEL_CONFIG);
  Wire.write(0x10);     // setting the full scale range to plus minus 8g
  Wire.endTransmission();

  Wire.beginTransmission(MPU_6050_addr);
  Wire.write(GYRO_CONFIG);
  Wire.write(0x08);     // setting the full scale range to plus minus 8g
  Wire.endTransmission();

  bme.begin();
  myServo.attach(10);
  myServo.write(0);  // set servo to mid-point
  Serial.begin(9600);

  Serial.println("Time; Magnitude; Temperature; Pressure; Height; Ejected?");

  pressure = bme.readPressure()/100;
}

void loop() {
  time = millis();
  Wire.beginTransmission(MPU_6050_addr);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission();
  
  Wire.requestFrom(MPU_6050_addr,14); // requesting 8 bytes from the MPU-6050
  MPU_6050_AccX = Wire.read() << 8 | Wire.read(); 
  MPU_6050_AccY = Wire.read() << 8 | Wire.read();
  MPU_6050_AccZ = Wire.read() << 8 | Wire.read();

  MPU_6050_Temp = Wire.read() << 8 | Wire.read();
  
  MPU_6050_GyroX = Wire.read() << 8 | Wire.read(); 
  MPU_6050_GyroY = Wire.read() << 8 | Wire.read();
  MPU_6050_GyroZ = Wire.read() << 8 | Wire.read();

  MPU_6050_AccX = MPU_6050_AccX/4096;
  MPU_6050_AccY = MPU_6050_AccY/4096;
  MPU_6050_AccZ = MPU_6050_AccZ/4096;

  MPU_6050_Temp = MPU_6050_Temp/340 + 36.53;

  MPU_6050_GyroX = MPU_6050_GyroX/65.5;
  MPU_6050_GyroY = MPU_6050_GyroY/65.5;
  MPU_6050_GyroZ = MPU_6050_GyroZ/65.5;
// average for testing, comment after
 // Serial.print("average: "); Serial.print(avr_altitude);Serial.println(";");
  Serial.print(time/1000);Serial.print("; ");
  magnitude = sqrt(pow(MPU_6050_AccX,2) + pow(MPU_6050_AccY,2) + pow(MPU_6050_AccZ,2));
  Serial.print(magnitude);Serial.print("; ");
  
  Serial.print(bme.readTemperature());Serial.print("; ");
  Serial.print(bme.readPressure()/100);Serial.print("; ");  
  Serial.print(bme.readAltitude(pressure));Serial.print("; ");

  
  if(bme.readAltitude(pressure) > 10 && ejected == false){
    previous_height_3 = previous_height_2;
    previous_height_2 = previous_height_1;
    previous_height_1 = bme.readAltitude(pressure);
    
    if(previous_height_3 > previous_height_2 && previous_height_2 > previous_height_1){
        myServo.write(180);              // tell servo to go to position in variable 'pos'
        delay(500);                       // waits 500ms for the servo to reach the position
        myServo.write(0);              // tell servo to go to position in variable 'pos'
        ejected = true;
        Serial.println("EJECTED!"); 
       }
       else{
          Serial.println("No"); 
        }     
    }else{
      Serial.println("No"); 
    }
    
  
  
    while(time + 99 > millis()){
      delay(1);
    }
    
}
