// ============================================
// APDS Setup (address = 0x39)
// ============================================
void APDS9960setup() { // initialize, check int motion, set parameters
  Serial.print("APD9960 Initialize -ID:0x"); Serial.println(I2Cread8(0x39, 0x92), HEX);    
  Serial.print(" [INT:");      Serial.print(digitalRead(24));  Serial.print("]");  
  Serial.print(" reg 93:0x");  Serial.print(I2Cread8(0x39, 0x93), HEX);   // read int (0x04)
  Serial.print("/fifo:");      Serial.println(I2Cread8(0x39, 0xAE));      // fifo count
  
  Serial.println("#Reset all mode(0x80=0x01):");    I2Cwrite8(0x39, 0x80, 0x01); // reset all   
  Serial.print(" [INT:");      Serial.print(digitalRead(24));  Serial.print("]");  
  Serial.print(" reg 93:0x");  Serial.print(I2Cread8(0x39, 0x93), HEX);   // read int (0x04)
  Serial.print("/fifo:");      Serial.println(I2Cread8(0x39, 0xAE));      // fifo count
  
  Serial.println("#Force int(E4=0x01):"); I2Cwrite8(0x39, 0xE4, 0x01);    // force int 
  Serial.print(" [INT:");      Serial.print(digitalRead(24));  Serial.print("]");  
  Serial.print(" reg 93:0x");  Serial.println(I2Cread8(0x39, 0x93), HEX); // read int (0x04)
   
  Serial.println("#Reset int(E5-7=0x01):"); 
      I2Cwrite8(0x39, 0xE5, 0x01); // Proximity int reset  
      I2Cwrite8(0x39, 0xE6, 0x01); // ALS int reset 
      I2Cwrite8(0x39, 0xE7, 0x01); // All non-gesture int reset    
  Serial.print(" [INT:");      Serial.print(digitalRead(24));  Serial.print("]"); 
  Serial.print(" reg 93:0x");  Serial.println(I2Cread8(0x39, 0x93), HEX); // read int (0x04)
  Serial.println("#Setup registers(Gainx4):"); 
      I2Cwrite8(0x39, 0x90, 0x31); // BOOST
      I2Cwrite8(0x39, 0x81, 0xDB); // ADC Integration Time  
      I2Cwrite8(0x39, 0x8C, 0x11); // Persistence   
      I2Cwrite8(0x39, 0x8F, 0x09); // 09:PGAINx4/AGAINx4
      I2Cwrite8(0x39, 0xA3, 0x41); // 41:GGAINx4/GWTIME 2.8ms
  Serial.println("---------------------------");  
}

// ============================================
// GESTURE
// ============================================
void GMonitor() {
  if(!digitalRead(24)) {
    int dd[4]; int len = I2Cread8(0x39, 0xAE); // Serial.print("fifo length:"); Serial.println(len); 
    for (int i = 0; i < len; i++) { for (int m = 0; m < 4; m++) { dd[m] = I2Cread8(0x39, 0xFC+m); } // FIFO read
      k = 0; for (int p = 0; p < 3; p++) { if(dd[p+1] < dd[k]) { k = p+1; } } // Serial.print("Dir:"); Serial.print(k);
      stayC++;                // count process
    }
    Oshot = millis();       // Set one shot timer    
  }  
}
void GFine(unsigned long interval) { // fix Gesture direction
  if(!GestureGo && Oshot != 0 && millis() > Oshot + interval && !Gmask) {
  // GestureGo解除済み、Oshotがセットされ時間経過、上空の障害物がなくなった後に実行
    if(stayC > 200 && stayC < 400) { k += 7; } // 滞空時間による区分け 7,8,9,10
    if(stayC > 400) { k = 4; }                 // 4
    Gesture = k; GestureGo = 1;  // GESTURE = 0,1,2,3,4,7,8,9,10
    Gmask = 1;                   // Mask gesture sense (reset if distance < 10)
    Oshot = 0;                   // reset One shot timer (Job done)
    stayC = 0;                   // ato shori clr GMonitor Req     
    Serial.print("G:"); Serial.println(Gesture);
  }
}

// ============================================
// Proximity/Color [0x80  GEN(6) PEN(2) AEN(1) PON(0)]
// ============================================          
int readProximity() { //Serial.println("#Setup Proximity mode:");   
  I2Cwrite8(0x39, 0x80, 0x05);     // enable APDS Proximity    
  I2Cwrite8(0x39, 0xAB, 0x00);     // reset GMODE
  distance = I2Cread8(0x39, 0x9C); // Serial.print("distance:"); Serial.println(distance); 
  if(distance < 10) { Gmask = 0; } // release Gesture mask when air is open
  //Serial.println(distance);  
  return distance;
}
void readColor() { //Serial.println("#Setup Amibient/Color mode:"); 
  I2Cwrite8(0x39, 0x80, 0x03);     // enable APDS ALS
  I2Cwrite8(0x39, 0xAB, 0x00);     // reset GMODE
  ambD   = I2Cread16i(0x39, 0x94); //Serial.print("CLR:"); Serial.println(ambD);  
  redD   = I2Cread16i(0x39, 0x96); //Serial.print("RED:"); Serial.println(redD);    
  greenD = I2Cread16i(0x39, 0x98); //Serial.print("GRE:"); Serial.println(greenD);   
  blueD  = I2Cread16i(0x39, 0x9A); //Serial.print("BUE:"); Serial.println(blueD);   
}

// ============================================
// Gesture setup [0x80  GEN(6) PEN(2) AEN(1) PON(0)]
// ============================================
void Gestureset() { // Serial.println("#Setup GMODE: (GEN/PEN/Enter,Exit level=0x20):"); 
  I2Cwrite8(0x39, 0x80, 0x4D);     // Gesture/Proximity/Wait Enable  
  I2Cwrite8(0x39, 0xAB, 0x06);     // INT Set (0x04:CLR) *GMODE set by PROXIMITY
  I2Cwrite8(0x39, 0xA0, 0x20);     // A0 Enter Level (28)
  I2Cwrite8(0x39, 0xA1, 0x20);     // A1 Exit  Level (1E)
}

