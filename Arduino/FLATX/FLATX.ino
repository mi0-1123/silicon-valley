// ============================================
//  Base design http://simblee.com
//  Language Reference http://arduino.cc/en/Reference/HomePage
//  24kRAM, 128kROM
// ============================================
//  FILE INCLUDE
// ============================================
   #include "Wire.h"               	// I2C
   #include "FastLED.h"            	// RGB LED
   #include "SimbleeCOM.h"         	// COM
   #include "para.h"               	//
   int16_t Bx,By,Bz;
	int rss,flag=0;
   int time_viv=0,change_hor=0,change_ver=0;
// ============================================
//  CONTROL VALIABLES
// ============================================
   CRGB leds[2];                   	// RGB LED buffer
   long itime1, itime2, Ttime;     	// event timer
   boolean tgll = true;

// ============================================
//  SETUP
// ============================================
void setup() {
  // Serial.begin(115200);
	Serial.begin(9600);
   Wire.begin();           	// SCL:12 SDA:25
   pinMode( 7, INPUT );    	// TACT SW
   pinMode( 4, OUTPUT);    	// LED A
   pinMode( 3, OUTPUT);    	// LED B
   pinMode( 6, OUTPUT);    	// LED C
   pinMode( 9, OUTPUT);    	// LED D
   pinMode(15, OUTPUT);    	// LED
	pinMode(23, INPUT_PULLUP);
   FastLED.addLeds<NEOPIXEL, 11>(leds, 2); // FastLED setup for RGB LEDs
   leds[0] = CRGB(10, 0, 0); leds[1] = CRGB(10, 0, 0);  FastLED.show();
   SimbleeCOM.begin();
   Gyrosetup();            	// GYRO setup
   APDS9960setup();      	// PROXIMITY setup
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
  if(millis() - itime2 > 100 ) { itime2 = millis();  ;//
	digitalWrite(15, tgll); tgll = !tgll;
  //Serial.print("Oxaa");
	//payloadd.temp = 25;
	/*rgbvalue1.red = leds[0].red;
	rgbvalue1.blue = leds[0].blue;
	rgbvalue1.green = leds[0].green;*/
   // rgbvalue1.red = 0;
	//rgbvalue1.blue = 5;
	//rgbvalue1.green = 0;
  //  SimbleeCOM.send((const char*)&rgbvalue1, sizeof(rgbvalue1));
	//Serial.println(Ay);

   getMotion9();
   time_viv++;
   if(flag==1 && time_viv>=7){time_viv=0;flag=0;}
	if(By<0 && flag==0){
  	if(change_hor==1){leds[0] = CRGB(0, 0, 5);leds[1] = CRGB(0, 0, 5);  FastLED.show();change_hor=2; Serial.println("BLUE");}
  	else if(change_hor==2) {leds[0] = CRGB(5, 0, 0);leds[1] = CRGB(5, 0, 0);FastLED.show();change_hor=0;Serial.println("RED");}
  	else {leds[0] = CRGB(0, 5, 0);leds[1] = CRGB(0, 5, 0);FastLED.show();change_hor=1;Serial.println("GREEN");}
  	flag=1;
  	}
  	else if(Bz<0 && flag==0){
  	if(change_ver==0){leds[0] = CRGB(51, 51, 51);leds[1] = CRGB(51, 51, 51);  FastLED.show();change_ver=2; Serial.println("BLACK");}
  	else if(change_ver==1) {leds[0] = CRGB(51, 41, 0);leds[1] = CRGB(51, 41, 0);FastLED.show();change_ver=0;Serial.println("YELLOW");}
  	else {leds[0] = CRGB(51, 0, 51);leds[1] = CRGB(51, 0, 51);FastLED.show();change_ver=1;Serial.println("GREEN");}
  	flag=1;
  	}
}
 /* if(millis() - itime1 > 1000) { itime1 = millis();  //
 	digitalWrite(15, tgll);  //
 	digitalWrite(4, tgll);   // A
 	digitalWrite(3, tgll);   // B
 	digitalWrite(6, tgll);   // C
 	digitalWrite(9, tgll);   // D

 	if(tgll) { leds[0] = CRGB(4, 4, 0); leds[1] = CRGB(40, 40, 0); }
 	else 	{ leds[0] = CRGB(40, 40, 0); leds[1] = CRGB(4, 4, 0); } FastLED.show();
 	tgll =! tgll;
 	//payloadd.PP = 33;
 	//SimbleeCOM.send((char*)&payloadd, sizeof(payloadd));
  }*/

/*  if(millis() - itime2 > 5000) { itime2 = millis();  //
 	for(int n; n < 3; n++) { onoff4(1, 0, 0, 0); onoff4(0, 1, 0, 0); onoff4(0, 0, 1, 0); onoff4(0, 0, 0, 1); }
  }
  if(recvflg) { recvflg = 0; }
*/
}
// ============================================
//  COM call back
// ============================================
void SimbleeCOM_onReceive(unsigned int esn, const char *payload, int len, int rssi) {
  memcpy(&payloadd2, payload, len); recvflg = 1;  //
}




