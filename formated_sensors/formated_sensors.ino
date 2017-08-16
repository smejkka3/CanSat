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
float altitude = 0;
float reference_point = 0;
float reference_pressure = 0;
//variables MPU-6050
float MPU_6050_AccX;
float MPU_6050_AccY;
float MPU_6050_AccZ;

boolean ejected = false;
boolean started = false;
float magnitude = 0;
float pressure = 0;
float previous_height_1 = 0;
float previous_height_2 = 0;
float previous_height_3 = 0;

float MPU_6050_Temp;

float MPU_6050_GyroX;
float MPU_6050_GyroY;
float MPU_6050_GyroZ;

int i = 0;

const int analogInPin = A0; // Analog input pin
float tempSensorValue = 0; // Temperature sensor data

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
  
}

void loop() {
  // if you recieve l from user initialize and set boolean started to true
  if(Serial.read() == 'l'){
      initalize();
  }
  // if the setup was initialized
  if(started == true){

    set_variables();
    print_datas();

    // PANIC BUTTON!!
    if(Serial.read() == 'f'){
          myServo.write(180);              // tell servo to go to position in variable 'pos'
          delay(500);                       // waits 500ms for the servo to reach the position
          ejected = true;
          Serial.print("MANUALLY EJECTED!"); 
    }
   
    if(altitude > reference_point + 10 && ejected == false ){
      previous_height_3 = previous_height_2;
      previous_height_2 = previous_height_1;
      previous_height_1 = altitude;
      
      if((previous_height_3 > previous_height_2) && (previous_height_2 > previous_height_1)){
          myServo.write(180);              // tell servo to go to position in variable 'pos'
          delay(500);                       // waits 500ms for the servo to reach the position
          ejected = true;
          Serial.println("EJECTED!"); 
         }
         else{
            Serial.println("No"); 
          }     
      }else if(ejected == true){
        Serial.println("EJECTED!"); 
      }else{
        Serial.println("No");  
      }
              
      while(time + 99 > millis()){
        delay(1);
      }
  }
}

void initalize(){
  time = 0;
      Serial.println("Time; Magnitude; Temperature; Reference Altitude; Current Altitude; Pressure; Ejected?");
  /* 
      i = 0;
      while(i < 20){
        reference_point += bme.readAltitude(); 
        i++;   
      }
   
      reference_point /= 20;
  */    
      reference_pressure = bme.readPressure()/100;
      reference_point = bme.readAltitude(reference_pressure);
      started = true;
}

void set_variables(){
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

    magnitude = sqrt(pow(MPU_6050_AccX,2) + pow(MPU_6050_AccY,2) + pow(MPU_6050_AccZ,2));
    pressure = bme.readPressure()/100;
    
    i = 0;
    while(i < 2){
      altitude = bme.readAltitude(reference_pressure); 
      i++;  
    }
    altitude /= 2;
   
    reference_point /= 20;
    
      // read ADC value
    tempSensorValue = analogRead(analogInPin);
    //concert temperature data
    tempSensorValue = tempSensorValue * 5000/1023 * 1/10;
  
  }

  void print_datas(){
    Serial.print(time/1000);Serial.print("; ");   
    Serial.print(magnitude);Serial.print("; ");
    Serial.print(tempSensorValue,1); Serial.print("; ");
    Serial.print(reference_point);Serial.print("; ");
    Serial.print(altitude);Serial.print("; ");
    Serial.print(pressure);Serial.print("; ");  
  }
