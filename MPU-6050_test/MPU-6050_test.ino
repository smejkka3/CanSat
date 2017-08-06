#include<Wire.h> // I2C library

const int MPU_6050_addr = 0x68; // I2C address of the MPU-6050
const byte PWR_MGMT_1 = 0x6B; // PWR_MGMT_1 register address of the MPU-6050
const byte WHO_AM_I = 0x75; // WHO_AM_I register address of the MPU-6050

byte WHO_AM_I_data;

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
  Wire.write(WHO_AM_I);
  Wire.endTransmission();
  Wire.requestFrom(MPU_6050_addr,1); // requesting 1 byte from the MPU-6050
  WHO_AM_I_data = Wire.read();
  Serial.print("Who am I? - "); Serial.println(WHO_AM_I_data);
  delay(1000);

}
