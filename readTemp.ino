#include <Wire.h>
#define TMP102_I2C_ADDRESS 72 /* This is the I2C address for our chip.*/

void tempSetup() {
  Wire.begin(); // start the I2C library
} 

double getTemp102() {
  byte firstByte, secondByte;
  int tempVal;
  double convVal;
  
  // Initialize transmission
  Wire.beginTransmission(TMP102_I2C_ADDRESS);
  Wire.write((byte)0x00);
  Wire.endTransmission();
  
  Wire.requestFrom(TMP102_I2C_ADDRESS, 2);
  Wire.endTransmission();
  
  firstByte = Wire.read();
  secondByte = Wire.read();
  
  // First byte is MSB in 12-bit register. We therefore shift up by 4
  tempVal = firstByte << 4;
  // Second byte MSBs have the rest of the 12-bit register, so we shift down by 4
  tempVal |= secondByte >> 4;
  
  convVal = tempVal * 0.0625;
  /*
  Serial.println("firstByte is :");
  Serial.print(firstByte, BIN);
  Serial.println("");
  
  Serial.println("secondByte is :");
  Serial.print(secondByte, BIN);
  Serial.println("");

  Serial.println("tempVal is :");
  Serial.print(tempVal, BIN);
  Serial.println("");
  
  Serial.println("convVal is :");
  Serial.print(convVal);
  Serial.println("");
  */
  return convVal;
}

