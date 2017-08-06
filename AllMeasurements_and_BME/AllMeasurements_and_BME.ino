#include<Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h> 

// MPU-6050 constants
const int MPU_6050_addr = 0x68;
const byte PWR_MGMT_1 = 0x6B;
const byte ACCEL_CONFIG = 0x1C;
const byte ACCEL_XOUT_H = 0x3B;
const byte GYRO_CONFIG = 0x1B;

//variables MPU-6050
float MPU_6050_AccX;
float MPU_6050_AccY;
float MPU_6050_AccZ;

float MPU_6050_Temp;

float MPU_6050_GyroX;
float MPU_6050_GyroY;
float MPU_6050_GyroZ;

//BMP280
Adafruit_BMP280 bme;

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
  Serial.begin(9600);

}

void loop() {
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

  Serial.print("MPU-6050 Acc X-Axis   "); Serial.print(MPU_6050_AccX,2);Serial.println(" [g] ");
  Serial.print("MPU-6050 Acc Y-Axis   "); Serial.print(MPU_6050_AccY,2);Serial.println(" [g] ");
  Serial.print("MPU-6050 Acc Z-Axis   "); Serial.print(MPU_6050_AccZ,2);Serial.println(" [g] ");
  
  Serial.print("MPU_6050 Temperature   "); Serial.print(MPU_6050_Temp, 1); Serial.println(" [degrees C] "); 

  Serial.print("MPU-6050 Gyro X-Axis   "); Serial.print(MPU_6050_GyroX,2);Serial.println(" [deg/s] ");
  Serial.print("MPU-6050 Gyro Y-Axis   "); Serial.print(MPU_6050_GyroY,2);Serial.println(" [deg/s] ");
  Serial.print("MPU-6050 Gyro Z-Axis   "); Serial.print(MPU_6050_GyroZ,2);Serial.println(" [deg/s] ");
  Serial.println(" ------BME------ "); 
  
  Serial.print("BME280 Temperature       "); Serial.print(bme.readTemperature());Serial.println(" [C] ");
  Serial.print("BME280 Pressure          "); Serial.print(bme.readPressure()/100);Serial.println(" [hPa] ");
  Serial.print("BME280 Approx altitude   "); Serial.print(bme.readAltitude(1013.25));Serial.println(" [m] ");
  Serial.println(" ------------ "); 
  delay(1000);


}
