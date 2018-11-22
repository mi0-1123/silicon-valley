// ============================================
//  Base design http://simblee.com
//  Language Reference http://arduino.cc/en/Reference/HomePage
//  24kRAM, 128kROM
// ============================================
//  FILE INCLUDE 
// ============================================ 
   #include "Wire.h"                   // I2C 
   #include "FastLED.h"                // RGB LED       
   #include "SimbleeCOM.h"             // COM
   #include "para.h"                   //       
   
// ============================================
//  CONTROL VALIABLES
// ============================================
   CRGB leds[2];                       // RGB LED buffer
   long itime1, itime2, itime3,itime4, Ttime;         // event timer   
   boolean tgll = true; 
   int dist;   
   int rsi;
   int got;
   int recdist;
   int16_t Bx,By,Bz;
   int color_change_flag=0;
   int time_viv=0,change_hor=0,change_ver=0;
   int count = 0, bcou = 0;

   
   
// ============================================
//  SETUP
// ============================================
void setup() {
   Serial.begin(115200); 
   Wire.begin();               // SCL:12 SDA:25  
   pinMode( 7, INPUT );        // TACT SW
   pinMode( 4, OUTPUT);        // LED A
   pinMode( 3, OUTPUT);        // LED B
   pinMode( 6, OUTPUT);        // LED C
   pinMode( 9, OUTPUT);        // LED D
   pinMode(15, OUTPUT);        // LED  
   FastLED.addLeds<NEOPIXEL, 11>(leds, 2); // FastLED setup for RGB LEDs       
   leds[0] = CRGB(0, 5, 0); leds[1] = CRGB(0, 5, 0);  FastLED.show();
   SimbleeCOM.begin();     
   Gyrosetup();                // GYRO setup  
   APDS9960setup();          // PROXIMITY setup   
   getMotion9(); 
}
// ============================================
//  LOOP
// ============================================

void loop() {
  if(abs(ay) > 2000 && millis() - itime3 > 1000) { itime3 = millis();

   if(!payd1.stick) {     
     getMotion9();
    if(abs(ay) > 2000 && millis() - itime3 > 1000) { itime3 = millis();
//      Serial.print("ay: ");Serial.println(abs(ay));
      count++;
//      Serial.print("count : "); Serial.println(count);      
     if(count >= 3 && !payd1.stick) {
        bcou ++;
        bright(bcou%NUM);      
        payd1.ID = bcou%NUM;
        Serial.println(payd1.ID);
        count = 0;
      }
    if( got && payd2.ID==255){
      Serial.println("got");
      Serial.println(rsi);
        if ( payd2.stick && rsi > -50){
          Serial.println("nigiri!");
          payd1.stick = 1;   
        }
        got  = 0;
      }
    } else {
      Serial.println("send!");
       SimbleeCOM.send((const char*)&payd1, sizeof(payd1));   
    }
   }
  }
}

// ============================================
//  COM call back
// ============================================
void SimbleeCOM_onReceive(unsigned int esn, const char* payd1, int len, int rssi) { //Serial.print("len:"); Serial.print(len);
  if(esn == 1795547886 ||esn == 1205341633){
  memcpy(&payd2, payd1, len); 
  got = 1;
  rsi = rssi;
  if(payd2.dist>240)payd2.stick=true;
  else payd2.stick=false;
 /* Serial.print(payd2.ID);Serial.print("\t");Serial.print("shake: ");Serial.print(payd2.shake);
    Serial.print("\t");Serial.print("stick: ");Serial.print(payd2.stick);Serial.print("\t");
    Serial.print("dist: ");Serial.print(payd2.dist);
    Serial.print("\t"); Serial.print("Bz: ");Serial.print(payd2.z);
    Serial.print("\t"); Serial.print("By: ");Serial.print(payd2.y);
    Serial.print("\t"); Serial.print("rssi: ");Serial.println(rssi);*/
  //Serial.print(esn);Serial.print("\t");Serial.println(rsi);
  }
}

//====
// funcsion
//====

void bright(int itr){
  switch (itr) {
    case 0: //salmon
      leds[0] = leds[1] = CRGB(12, 2, 1);  FastLED.show();
      break;
    case 1: //tuna
      leds[0] = leds[1] = CRGB(10, 0, 0);  FastLED.show();
      break;
    case 2: //egg
      leds[0] = leds[1] = CRGB(5, 5, 0);  FastLED.show();
      break;
    default:
      leds[0] = leds[1] = CRGB(0, 5, 0);  FastLED.show();
      break;
  } 
}



