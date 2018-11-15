// ============================================
// GYRO SENSOR MPU-9250
  #define  MPU9250_Adrs  0x68
  #define  MAG_Adrs     0x0C
  #define  GYRO_250_DPS 0x00  // max shock
  #define  GYRO_500_DPS 0x08
  #define  GYRO_1000_DPS   0x10
  #define  GYRO_2000_DPS   0x18
  #define  ACCEL_2_G    0x00  // max angle
  #define  ACCEL_4_G    0x08
  #define  ACCEL_8_G    0x10
  #define  ACCEL_16_G   0x18
  uint8_t Buf[14], Mag[8];  
// ============================================
void Gyrosleep() {
  I2Cwrite8(MPU9250_Adrs, 0x6B, 0x40);  // GYRO sleep mode
  I2Cwrite8(MPU9250_Adrs, 0x6C, 0x3F);  // Disable GYRO XYZ
  I2Cwrite8(MAG_Adrs, 0x0A, 0x00);      // MAG sleep mode
}
void Gyrosetup() {
  Serial.println(F("GYRO setup.."));
  byte ID = I2Cread8(MPU9250_Adrs, 0x75); Serial.print(F(" MPU9250 ID = 0x")); Serial.print(ID, HEX); //
  if(ID == 0x71) { Serial.println(F(" Running. ")); } else { Serial.println(F(" Failed. ")); }
  I2Cwrite8(MPU9250_Adrs, 0x6B, 0x80); delay(10);  // Reset device
  I2Cwrite8(MPU9250_Adrs, 0x6C, 0x00); delay(10);  // Enable GYRO
  
  I2Cwrite8(MPU9250_Adrs, 0x1D, 0x04);   // (29)ACCEL_CONFIG 2 accelerometers low pass filter at 5Hz
  // 7:250Hz delay  2ms  6:5Hz delay 67ms  5:10Hz delay 36ms  4:20Hz delay 20ms
  // 3: 40Hz delay 12ms  2:90Hz delay 8ms  1:180Hz delay 6ms  0:250Hz delay  2ms
  I2Cwrite8(MPU9250_Adrs, 0x1A, 0x05);   // (26)CONFIG Set gyroscope low pass filter at 5Hz
  // 7:3600Hz delay 0.17ms  6:5Hz delay 33ms  5:10Hz delay 18ms  4:20Hz delay 10ms
  // 3:  40Hz delay 6ms   2:90Hz delay 4ms  1:180Hz delay 3ms  0:250Hz delay 1ms  
  I2Cwrite8(MPU9250_Adrs, 0x1B, GYRO_250_DPS);  // (27)GYRO_CONFIG Configure gyroscope range
  I2Cwrite8(MPU9250_Adrs, 0x1C, ACCEL_2_G);   // (28)ACCEL_CONFIG Configure accelerometers range
  I2Cwrite8(MPU9250_Adrs, 0x37, 0x02);        // Set by pass mode for the magnetometers
  ID = I2Cread8(MAG_Adrs, 0x00); Serial.print(F(" MAG ID = 0x")); Serial.print(ID, HEX); //
  if(ID == 0x48) { Serial.println(F(" Running. ")); } else { Serial.println(F(" Failed. ")); }
  I2Cwrite8(MAG_Adrs, 0x0B, 0x01); delay(10);   // soft reset   
  I2Cwrite8(MAG_Adrs, 0x0A, 0x12);            // Request continuous magnetometer measurements in 16 bits  
}
void getMotion9() {
  I2CreadMulti(MPU9250_Adrs,0x3B,14,Buf); // read accelgyro ax,ay,az,gx,gy,gz
   by = ay; bx =ax; bz=az; hx=gx; hy=gy; hz=gz;
  
  ax = makeWord(Buf[0],Buf[1]); ay = makeWord(Buf[2],Buf[3]);   az = makeWord(Buf[4],Buf[5]);
  gx = makeWord(Buf[8],Buf[9]); gy = makeWord(Buf[10],Buf[11]); gz = makeWord(Buf[12],Buf[13]);
  //uint8_t ST1; do { I2CreadMulti(MAG_Adrs,0x02,1,&ST1); } while (!(ST1&0x01));
  //I2CreadMulti(MAG_Adrs,0x03,8,Mag);    // read mag mx,my,mz
  //mx=-(makeWord(Mag[3],Mag[2])); my=-(makeWord(Mag[1],Mag[0])); mz=-(makeWord(Mag[5],Mag[4])); 
  //dumpG();
  by = abs(ay-by); bx =abs(ax-bx); bz=abs(az-bz);
  hy = abs(gy-hy); hx =abs(gx-hx); hz=abs(gz-hz);
  By=by;//横ぶり判定
  Bz=bz;//縦ぶり判定
  //getter();
}
int16_t getter(){
  Serial.print(bx);Serial.print("\t");Serial.print(by);Serial.print("\t");Serial.print(bz);Serial.print("\t");
   Serial.print(hx);Serial.print("\t");Serial.print(hy);Serial.print("\t");Serial.println(hz);
 
  }
void dumpG() {
  Serial.print(ax); Serial.print("\t"); Serial.print(ay); Serial.print("\t"); Serial.print(az);
  Serial.print(gx); Serial.print("\t"); Serial.print(gy); Serial.print("\t"); Serial.print(gz);
  Serial.print(mx); Serial.print("\t"); Serial.print(my); Serial.print("\t"); Serial.print(mz);
  Serial.println(""); 
}
// ============================================
//  I2C
// ============================================
void I2Cwrite8(uint8_t addr, uint8_t reg, uint16_t data)  { // Write 1byte data to I2C
  Wire.beginTransmission(addr); Wire.write(reg); Wire.write(data); Wire.endTransmission();
}
uint8_t I2Cread8(uint8_t addr, uint8_t reg) {   // Read 1byte from I2C
  Wire.beginTransmission(addr); Wire.write(reg); Wire.endTransmission();
  Wire.beginTransmission(addr); Wire.requestFrom((uint8_t)addr, (uint8_t)1);
  uint8_t data = Wire.read(); Wire.endTransmission(); return data;
}
uint16_t I2Cread16i(uint8_t addr, uint8_t reg) { // Read 1word from I2C low/high
  Wire.beginTransmission(addr); Wire.write(reg); Wire.endTransmission();
  Wire.beginTransmission(addr); Wire.requestFrom((uint8_t)addr, (uint8_t)2);
  uint8_t data = Wire.read(); uint8_t data2 = Wire.read(); Wire.endTransmission();
  return makeWord(data2,data); 
}
void I2CreadMulti(uint8_t addr, uint8_t reg, uint8_t data, uint8_t* Data) { // Read multiple byte to buffer
  Wire.beginTransmission(addr); Wire.write(reg); Wire.endTransmission();
  Wire.requestFrom(addr, data); uint8_t m=0; while (Wire.available()) { Data[m++] = Wire.read(); }
}


