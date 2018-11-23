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

// ============================================
//  SETUP
// ============================================
void setup() {
    Serial.begin(115200);
    Wire.begin();               // SCL:12 SDA:25
//    pinMode( 7, INPUT );        // TACT SW
//    pinMode( 4, OUTPUT);        // LED A
//    pinMode( 3, OUTPUT);        // LED B
//    pinMode( 6, OUTPUT);        // LED C
//    pinMode( 9, OUTPUT);        // LED D
//    pinMode(15, OUTPUT);        // LED
    FastLED.addLeds<NEOPIXEL, 11>(leds, 2); // FastLED setup for RGB LEDs
    leds[0] = CRGB::Blue; leds[1] = CRGB::Green;  FastLED.show();
    SimbleeCOM.begin();
    Gyrosetup();                // GYRO setup
    APDS9960setup();          // PROXIMITY setup
}
// ============================================
//  LOOP
// ============================================
//void loop() { //test
//    if(millis() - itime3 > 100) { itime3 = millis();  // 100nsごとに処理
//        SimbleeCOM.send((const char*)&sendmsg, sizeof(sendmsg));
//
//
//    }
//}
//

void loop() {

    if(millis() - itime3 > 100) { itime3 = millis();  // 送信処理
        payd1.dist = readProximity();

        if(payd1.dist > 240) {
            payd1.stick=true;
        } else {
            payd1.stick=false;// distance
        }

        getMotion9();
        payd1.z=Bz;
        payd1.y=By;

        if(payd2.stick) {
            //payd1.stick=true;
            if(rsi>-44) {//rssi value
                if(payd2.z<0) { //nigiri
                    Serial.println("tate");
                    if(payd2.ID==0 && payd1.ID!=0){
                        payd1.shake=true;
                        Serial.println("SHAKE");
                    }
                } else {
                    payd1.shake=false;
                }
            }
        } else {
            Serial.println("bad");
            //payd1.stick=false;
            payd1.shake=false;
            time_viv++;
            if(color_change_flag == 1 && time_viv >= 7) {
                time_viv=0;
                color_change_flag=0;
            }

            if(By<0 && color_change_flag==0){
                change_hor++;
                changeColor(change_hor % 4);
                payd1.ID = change_hor % 4;
            }//end By
        }//end else dist

        SimbleeCOM.send((const char*)&payd1, sizeof(payd1));
    }
}

void changeColor(int colorId) {
    switch (colorId) {
        case 0:
            leds[0] = leds[1] = CRGB::White; FastLED.show();
            break;
        case 1:
            leds[0] = leds[1] = CRGB::Red; FastLED.show();
            break;
        case 2:
            leds[0] = leds[1] = CRGB::HotPink; FastLED.show();
            break;
        case 3:
            leds[0] = leds[1] = CRGB::Yellow; FastLED.show();
            break;
    }
}

// ============================================
//  COM call back
// ============================================
void SimbleeCOM_onReceive(unsigned int esn, const char* payd1, int len, int rssi) { //Serial.print("len:"); Serial.print(len);
    //Serial.println(esn);
    if(esn == 3751699986 ||esn == 1205341633){ //sushi子機以外のbleを受け取らない
        memcpy(&payd2, payd1, len);
        got = 1;
        rsi = rssi;
        payd2.stick = payd2.stick > 240;
        /* Serial.print(payd2.ID);Serial.print("\t");Serial.print("shake: ");Serial.print(payd2.shake);
           Serial.print("\t");Serial.print("stick: ");Serial.print(payd2.stick);Serial.print("\t");
           Serial.print("dist: ");Serial.print(payd2.dist);
           Serial.print("\t"); Serial.print("Bz: ");Serial.print(payd2.z);
           Serial.print("\t"); Serial.print("By: ");Serial.print(payd2.y);
           Serial.print("\t"); Serial.print("rssi: ");Serial.println(rssi);*/
        //Serial.print(esn);Serial.print("\t");Serial.println(rsi);
    }
}




