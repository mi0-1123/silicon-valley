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

const int TactSW = 7;
const int rgbPin = 11;

// ============================================
//  SETUP
// ============================================
void setup() {
    Serial.begin(9600, 17, 19);
//    Serial.begin(9600);
    Wire.begin();               // SCL:12 SDA:25
    pinMode(TactSW, INPUT );        // TACT SW 押されたらLOW 何もない時HIGH(謎仕様?)
//    pinMode(15, OUTPUT);        // LED こいつをHighにしてもlowにしても何も変化しない
    FastLED.addLeds<NEOPIXEL, rgbPin>(leds, 1); // FastLED setup for RGB LEDs -NEOPIXEL系のLED
    SimbleeCOM.begin();
    //   Gyrosetup();                // GYRO setup
    //   APDS9960setup();          // PROXIMITY setup
}
// ============================================
//  LOOP
// ============================================

void loop() {
    int passTime = 0;

    if(millis() - passTime > 1000) { passTime = millis();
        if(got) {
            leds[0] = CRGB::Green;
            FastLED.show();
            String getmsg = (String)payd2.shake;
            Serial.print("send from arduino : ");
            Serial.println(getmsg);
            got = 0;
        } else {
            leds[0] = CRGB::Red;
            FastLED.show();
        }
    }
//    if(digitalRead(TactSW) == LOW) {
//        Serial.println("send message from arduino");
//        leds[0] = CRGB::HotPink;
//        FastLED.show();
//        delay(100);
//    } else {
//        leds[0] = CRGB::Yellow;
//        FastLED.show();
//        delay(100);
//    }

//    if(Serial.available() > 0) {
//        int incomingByte = Serial.read();
//        leds[0] = CRGB::Red;
//        FastLED.show();
//        delay(100);
//    }
}


//void loop() {
////    if(millis() - itime3 > 100) { itime3 = millis();  // 100msごとに繰り返し
//        //SimbleeCOM.send((const char*)&payd1, sizeof(payd1));
//
//        if(digitalRead(7) == HIGH){
//            Serial.println("pushed");
//            leds[0] = CRGB::White;
//            FastLED.show();
//            delay(100);
//        } else {
//            Serial.println("no pushed");
//            leds[0] = CRGB::HotPink;
//            FastLED.show();
//            delay(100);
//        }
//
////        if(got){
////            int ID = payd2.ID;
////            bool shake = payd2.shake;
////            bool stick = payd2.stick;
////            //   Serial.println(id);Serial.println(shake);Serial.println(stick);
////            String strid = String(ID);
////            // Serial.println(ID);
////            String strsh = String(shake);
////            String strst = String(stick);
////            String strrsi = String(rsi);
////            String sentmsg = strid+","+strsh+","+strst+","+strrsi;
////
////            //sentmessage = "ID(int),shake(bool),stick(bool),strrsi(int)"
////
////            if(sentmsg != "")Serial.println(sentmsg);
////            got = 0;
////        }
////    }
//}

// ============================================
//  COM call back
// ============================================
//void SimbleeCOM_onReceive(unsigned int esn, const char* payd1, int len, int rssi) { //Serial.print("len:"); Serial.print(len);
//    if(esn==1795547886||esn==1205341633){ //これはsishiのID
//        memcpy(&payd2, payd1, len);
//        if(payd2.ID==0){
//            got = 1;
//            rsi = rssi;
//        }
//    }
//}


void SimbleeCOM_onReceive(unsigned int esn, const char* payd2, int len, int rssi) {
    got = 1;
    //Serial.print("len:"); Serial.print(len);
//    memcpy(&payd2, payd1, len);
//    if(payd2.ID==0){
//        got = 1;
//        rsi = rssi;
//    }
}


