#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

/* 관련 핀 상수화하기 */
#define TX 2
#define RX 3
#define LED 5

SoftwareSerial blueToothSerial(TX, RX);   // 블루투스 객체 선언
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD 객체 선언

void setup() {
  Serial.begin(9600);
  Serial.println("Hi, serial");

  blueToothSerial.begin(9600);
  blueToothSerial.println("Hello, wolrd?");
}

void loop() {

  if(blueToothSerial.available()){
    Serial.write(blueToothSerial.read());
  }

  if(Serial.available()){
    blueToothSerial.write(Serial.read());
  }

}

