#include <Adafruit_NeoPixel.h>

#define ledMain 3
#define ledSec 5
#define pumpMain 13
#define pumpSec 12
#define lightSensor A0
#define moiSensor A1
#define ledNum 6
#define ledpower1 7
#define ledpower2 10

Adafruit_NeoPixel stripMain = Adafruit_NeoPixel(ledNum, ledMain, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripSec = Adafruit_NeoPixel(ledNum, ledSec, NEO_GRB + NEO_KHZ800);

int moiLine = 500; //습도 기준
int lightLine = 450; //빛 기준

int timeline = 0;

void booting() {
  pumpControl(1);
  rgbControl(255, 0, 0);
  delay(500);
  rgbControl(0, 0, 0);
  rgbControl(0, 255, 0);
  delay(500);
  rgbControl(0, 0, 0);
  rgbControl(0, 0, 255);
  delay(500);
  rgbControl(0, 0, 0);
  rgbControl(255, 255, 255);
  delay(500);
  rgbControl(0, 0, 0);
  pumpControl(0);
}

void pumpControl(int state) {
  if (state == 1) {
    rgbControl(0, 0, 100);
    digitalWrite(pumpMain, HIGH);
    digitalWrite(pumpSec, HIGH);
  }
  else {
    digitalWrite(pumpMain, LOW);
    digitalWrite(pumpSec, LOW);
    rgbControl(0, 0, 0);
  }
}

void ledControl(int state) {
  if (state == 1) {
    digitalWrite(ledpower1, HIGH);
    digitalWrite(ledpower2, HIGH);
    for (int i = 0; i < ledNum; i++) {
      stripMain.setPixelColor(i, 255, 51, 51);
      stripSec.setPixelColor(i, 255, 51, 51);
    }
  }
  else {
    for (int i = 0; i < ledNum; i++) {
      stripMain.setPixelColor(i, 0, 0, 0);
      stripSec.setPixelColor(i, 0, 0, 0);
    }
    digitalWrite(ledpower1, LOW);
    digitalWrite(ledpower2, LOW);
  }
  stripMain.show();
  stripSec.show();
}

void rgbControl(int r, int g, int b) {
  digitalWrite(ledpower1, HIGH);
  digitalWrite(ledpower2, HIGH);
  for (int i = 0; i < ledNum; i++) {
    stripMain.setPixelColor(i, r, g, b);
    stripSec.setPixelColor(i, r, g, b);
  }
  stripMain.show();
  stripSec.show();
  if (((r == 0) && (g == 0)) && b == 0) {
    digitalWrite(ledpower1, LOW);
    digitalWrite(ledpower2, LOW);
  }
}

void timewater() {
  if ((millis() - timeline) >= 1800000) {
    timeline = millis();
    if (analogRead(moiSensor) >= moiLine) {
      while (true) {
        //water on
        pumpControl(1);
        if (analogRead(moiSensor) <= moiLine) {
          //water off
          pumpControl(0);
          break;
        }
      }
    }
  }
}

void setup() {
  //int toggleLight = 0;
  //int toggleLight = 0;
  pinMode(pumpMain, OUTPUT);
  pinMode(pumpSec, OUTPUT);
  pinMode(ledpower1, OUTPUT);
  pinMode(ledpower2, OUTPUT);
  stripMain.begin();
  stripSec.begin();
  stripMain.show();
  stripSec.show();
  booting();
}

void loop() {
  int lightNow = analogRead(lightSensor);
  if (lightNow >= lightLine) {
    //toggleLight = 1;
    //onlight
    ledControl(1);
    if (analogRead(moiSensor) >= moiLine) {
      while (true) {
        //water on
        pumpControl(1);
        if (analogRead(moiSensor) <= moiLine) {
          //water off
          pumpControl(0);
          break;
        }
      }
    }

  }
  else {
    //offlight
    ledControl(0);
  }
  timewater();
}