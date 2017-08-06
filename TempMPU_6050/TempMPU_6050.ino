#include<Wire.h> // I2C library

const int MPU_6050_addr = 0x68; // I2C address of the MPU-6050
const byte PWR_MGMT_1 = 0x6B; // PWR_MGMT_1 register address of the MPU-6050
const byte TEMP_OUT_H = 0x41; // TEMP_OUT_H register address of the MPU-6050

byte TEMP_OUT_H_data;
byte TEMP_OUT_L_data;
float MPU_6050_Temp;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_6050_addr);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);    // set to zero (wakes up the MPU-6050)
  Wire.endTransmission();
  Serial.begin(9600);

}

void loop() {
  Wire.beginTransmission(MPU_6050_addr);
  Wire.write(TEMP_OUT_H);
  Wire.endTransmission();
  Wire.requestFrom(MPU_6050_addr,2); // requesting 2 byte from the MPU-6050
  TEMP_OUT_H_data = Wire.read();
  TEMP_OUT_L_data = Wire.read();

  MPU_6050_Temp = TEMP_OUT_H_data << 8 | TEMP_OUT_L_data; // getting the signed 16-bit TEMP_OUT data.
  MPU_6050_Temp = MPU_6050_Temp/340 + 36.53;
  Serial.print("MPU-6050 Temp - "); Serial.print(MPU_6050_Temp);Serial.println(" [degrees C]");
  delay(1000);

}
