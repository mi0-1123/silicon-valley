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
    Serial.begin(9600, 17, 19);
    //Serial.begin(9600);
    Wire.begin();               // SCL:12 SDA:25
    //   pinMode( 7, INPUT );        // TACT SW
    //   pinMode( 4, OUTPUT);        // LED A
    //   pinMode( 3, OUTPUT);        // LED B
    //   pinMode( 6, OUTPUT);        // LED C
    //   pinMode( 9, OUTPUT);        // LED D
    pinMode(15, OUTPUT);        // LED
    FastLED.addLeds<NEOPIXEL, 11>(leds, 2); // FastLED setup for RGB LEDs
    //   leds[0] = CRGB(1, 0, 0); leds[1] = CRGB(0, 10, 0);  FastLED.show();
    SimbleeCOM.begin();
    Gyrosetup();                // GYRO setup
    APDS9960setup();          // PROXIMITY setup
}
// ============================================
//  LOOP
// ============================================
void loop() {
    if(millis() - itime3 > 100) { itime3 = millis();  // 送信処理
        //SimbleeCOM.send((const char*)&payd1, sizeof(payd1));

        if(got){
            FastLED.show();
            int ID = payd2.ID;
            switch (ID) {
                case 0:
                    leds[0] = CRGB::White;
                    break;
                case 1:
                    leds[0] = CRGB::Red;
                    break;
                case 2:
                    leds[0] = CRGB::HotPink;
                    break;
                case 3:
                    leds[0] = CRGB::Yellow;
                    break;
                default:
                    leds[0] = CRGB::Blue;
                    break;
            }
            FastLED.show();
            bool shake = payd2.shake;
            bool stick = payd2.stick;
            //   Serial.println(id);Serial.println(shake);Serial.println(stick);
            String strid = String(ID);
            // Serial.println(ID);
            String strsh = String(shake);
            String strst = String(stick);
            String strrsi = String(rsi);
            String sentmsg = strid+","+strsh+","+strst+","+strrsi;

            //sentmessage = "ID(int),shake(bool),stick(bool),strrsi(int)"

            if(sentmsg != "") {
                Serial.println(sentmsg);
            }
            got = 0;
        } else {
            leds[0] = CRGB::Green;
            FastLED.show();
       }

    }
}

// ============================================
//  COM call back
// ============================================
void SimbleeCOM_onReceive(unsigned int esn, const char* payd1, int len, int rssi) { //Serial.print("len:"); Serial.print(len);
    if(esn == 3751699986 || esn == 1205341633){ //これはsishiのblootothのid //たぶん
        memcpy(&payd2, payd1, len);
//        if(payd2.ID == 0){ //シャリのみ受け取る
            got = 1;
            rsi = rssi;
//        }
    }
}

