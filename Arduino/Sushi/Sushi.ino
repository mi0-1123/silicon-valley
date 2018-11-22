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
    leds[0] = CRGB(1, 0, 0); leds[1] = CRGB(0, 10, 0);  FastLED.show();
    SimbleeCOM.begin();
    Gyrosetup();                // GYRO setup
    APDS9960setup();          // PROXIMITY setup
}
// ============================================
//  LOOP
// ============================================
void onoff4(int a, int b, int c, int d) {
    digitalWrite(4, a); digitalWrite(3, b); digitalWrite(6, c); digitalWrite(9, d);
    leds[0] = CRGB(a*50, b*50, c*50); leds[1] = leds[0]; FastLED.show();
    delay(100);
}
void loop() {

    if(millis() - itime3 > 100) { itime3 = millis();  // 送信処理
        payd1.dist = readProximity();
        //Serial.println(payd1.dist);
        //Serial.println("send");

        if(payd1.dist>240)payd1.stick=true;
        else payd1.stick=false;// distance
        getMotion9();
        payd1.z=Bz;
        payd1.y=By;

        if(payd2.stick){
            //payd1.stick=true;
            if(rsi>-44){//rssi value
                if(payd2.z<0){ //nigiri
                    Serial.println("tate");
                    //nigiri_sound==1; input struct value
                    if(payd2.ID==0 && payd1.ID!=0){
                        payd1.shake=true;
                        Serial.println("SHAKE");
                    }
                }else payd1.shake=false;
            }
        }//end if dist
        else {
            Serial.println("bad");
            //payd1.stick=false;
            payd1.shake=false;
            time_viv++;
            if(color_change_flag==1 && time_viv>=7){time_viv=0;color_change_flag=0;}
            if(By<0 && color_change_flag==0){
                if(change_hor==1){leds[0] = CRGB(1, 1, 1);leds[1] = CRGB(1, 1, 1);  FastLED.show();change_hor=2; payd1.ID=0;}//Serial.println("Rice");
                else if(change_hor==2) {leds[0] = CRGB(1, 0, 0);leds[1] = CRGB(1, 0, 0);FastLED.show();change_hor=0;payd1.ID=1;}//Serial.println("Tuna");
                else {leds[0] = CRGB(5, 4, 0);leds[1] = CRGB(5, 4, 0);FastLED.show();change_hor=1;payd1.ID=2;}//Serial.println("Egg");
                color_change_flag=1;
            }//end By
        }//end else dist

        SimbleeCOM.send((const char*)&payd1, sizeof(payd1));
    }
    /*if(got){ //signal accept
    //Serial.println(rsi);
    got = 0;
    }*/
}

// ============================================
//  COM call back
// ============================================
void SimbleeCOM_onReceive(unsigned int esn, const char* payd1, int len, int rssi) { //Serial.print("len:"); Serial.print(len);
    if(esn == 1795547886 ||esn == 1205341633){
        memcpy(&payd2, payd1, len);
        // got = 1;
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




