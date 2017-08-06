#include<Wire.h> // I2C library

const int MPU_6050_addr = 0x68; // I2C address of the MPU-6050
const byte PWR_MGMT_1 = 0x6B;
const byte ACCEL_CONFIG = 0x1C;
const byte ACCERL_XOUT_H = 0x3B;

float MPU_6050_AccX;
float MPU_6050_AccY;
float MPU_6050_AccZ;

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
  
  Serial.begin(9600);

}

void loop() {
  Wire.beginTransmission(MPU_6050_addr);
  Wire.write(ACCERL_XOUT_H);
  Wire.endTransmission();
  Wire.requestFrom(MPU_6050_addr,8); // requesting 8 bytes from the MPU-6050
  MPU_6050_AccX = Wire.read() << 8 | Wire.read(); 
  MPU_6050_AccY = Wire.read() << 8 | Wire.read();
  MPU_6050_AccZ = Wire.read() << 8 | Wire.read();

  MPU_6050_AccX = MPU_6050_AccX/4096;
  MPU_6050_AccY = MPU_6050_AccY/4096;
  MPU_6050_AccZ = MPU_6050_AccZ/4096;

  Serial.print("MPU-6050 Acc X-Axis   "); Serial.print(MPU_6050_AccX,2);Serial.println(" [g] ");
  Serial.print("MPU-6050 Acc Y-Axis   "); Serial.print(MPU_6050_AccY,2);Serial.println(" [g] ");
  Serial.print("MPU-6050 Acc Z-Axis   "); Serial.print(MPU_6050_AccZ,2);Serial.println(" [g] ");
  Serial.println(" ------------ "); 
  
  delay(1000);

}
