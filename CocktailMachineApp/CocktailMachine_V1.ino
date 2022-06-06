#include <DHT.h>
#include<Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

/* 8 ~ 9 , 6 ~ 7번핀 */

/* 관련 핀 상수화하기 */  
#define TX 2
#define RX 3

#define DHTPIN 4
#define DHTTYPE DHT11

// 드라이버 모듈 1
#define WATHER_1AA 13
#define WATHER_1AB 12
#define WATHER_1BB 11
#define WATHER_1BA 10

// 드라이버 모듈 2
#define WATHER_2AA 9
#define WATHER_2AB 8
#define WATHER_2BB 7
#define WATHER_2BA 6

DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial blueToothSerial(TX, RX);   // 블루투스 객체 선언
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD 객체 선언 SDA A4 SCL A5

void setup() {
  /* 시리얼 창 설정 */
  Serial.begin(9600);
  blueToothSerial.begin(9600);
  dht.begin();

  Wire.begin();  
  delay(500); 
  Wire.requestFrom(8, 20); 
  delay(1000);  


  if (Wire.available()) {
    while (Wire.available()) {
      byte c = Wire.read();

      if(char(c) == 'z'){
        Serial.print(char(c));
        blueToothSerial.print(char(c));
        break;
      }

      if (char(c) != 'X') {
        Serial.print(char(c));
        blueToothSerial.print(char(c));
      }
    }
  }
  else {
    Serial.println("물 수위 센서 값 받아오기 실패");
  }


  Serial.println("\n-- 아두이노 세팅 완료 --");
  delay(100); // 온습도를 읽어 오려면 약간의 대기 시간이 필요합니다.

  int tmp = dht.readTemperature();
  blueToothSerial.print(String(tmp));


  // 값 읽기에 오류가 있으면 오류를 출력합니다.
  if (isnan(tmp) ) {
    Serial.println("Failed to read from DHT sensor!");
  }

  pinMode(WATHER_1AA, OUTPUT);
  pinMode(WATHER_1AB, OUTPUT);

  pinMode(WATHER_1BB, OUTPUT);
  pinMode(WATHER_1BA, OUTPUT);

  pinMode(WATHER_2AA, OUTPUT);
  pinMode(WATHER_2AB, OUTPUT);

  pinMode(WATHER_2BB, OUTPUT);
  pinMode(WATHER_2BA, OUTPUT);

  /* Lcd 판 기본설정 */
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("7 Team");

  Serial.print("Temperature: ");  // 이하생략
  Serial.print(tmp);
  Serial.println(" C");


}

void loop() {
  if (blueToothSerial.available()) {
    char message = blueToothSerial.read();

    if (message >= '0' && message <= '6') {
      Serial.print(message);
      Serial.println(" 음료수를 만듭니다.");

      /* 칵테일 만들기 전 LCD on */

      setLCDText("MAKING...");

      int drinkNum = (int)(message - 48);

      // 탄산수
      digitalWrite(13, HIGH);
      digitalWrite(12, HIGH);

      // 사용자 선택 음료
      digitalWrite(drinkNum * 2 + 6, HIGH);
      digitalWrite(drinkNum * 2 + 7, HIGH);
      delay(3000);

      // 탄산수
      digitalWrite(WATHER_1AA, LOW);
      digitalWrite(WATHER_1AB, LOW);

      // 사용자 선택 음료
      digitalWrite(drinkNum * 2 + 6, LOW);
      digitalWrite(drinkNum * 2 + 7, LOW);

      /* 음료를 만든 후에 LCD off */
      delay(500);

      setLCDText("COMPLETE!");

      /* 완성메제지 보내기  */
      blueToothSerial.print("9");
 

    } else {
      Serial.print(message);
    }
  }
}

void setLCDText(String text) {
  lcd.setCursor(0, 1);
  lcd.print(text);
}
