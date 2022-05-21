#include <DHT.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 4
#define DHTTYPE DHT11  

DHT dht(DHTPIN, DHTTYPE); 

/* 관련 핀 상수화하기 */
#define TX 2
#define RX 3

SoftwareSerial blueToothSerial(TX, RX);   // 블루투스 객체 선언

int recipes[7][7] = { {20, 0, 0, 20, 20, 0, 0},
                     {20, 20, 0, 0, 20, 0, 0},
                     {0, 15, 15, 15, 0, 15 ,0},
                     {0, 0, 0, 15, 0, 30, 15},
                     {24, 0, 0, 12, 0, 0, 24},
                     {0, 0, 0, 0, 30, 0, 30},
                     {0, 12, 24, 0, 12, 12 , 0} };

/* 
 * 여기에서 칵테일의 양을 조절할 수 있습니다
 * volume 값과 칵테일의 양은 정비례합니다
 */
 
 int volume = 100;

void setup() {
  /* 시리얼 창 설정 */
  Serial.begin(9600);
  blueToothSerial.begin(9600);
  
  Serial.println("-- 아두이노 세팅 완료 --");
  int tmp = dht.readTemperature();
  blueToothSerial.print(String(tmp));
  Serial.print("Temperature: ");  // 이하생략
  Serial.print(tmp);
  Serial.println(" C");
}

void loop() {
  if (blueToothSerial.available()){
    char message = blueToothSerial.read();

    if (message >= '0' && message <= '6') {
      Serial.print(message);
      Serial.println(" 칵테일을 만듭니다");
      
      /* 칵테일 만들기 전 LED on */

      delay(3000);
      
      /* 칵테일 만들기 */
      for (int i = 0; i < 7; i++) {
        digitalWrite(i + 7, HIGH);
        delay(volume * recipes[(int) message - 48][i]);
        digitalWrite(i + 7, LOW);
      }
  
      /* 칵테일 만든 후에 LED off */
      delay(1000);
    
      /* 
       * 완성메제지 보내기  
       * 최대한 짧게 보내는 게 좋습니다
       */
      blueToothSerial.print("0");
    } else {
      Serial.print(message);
    }
  }
}
