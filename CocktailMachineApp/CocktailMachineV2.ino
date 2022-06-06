#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TimerFreeTone.h>

int trigPin = 2;
int echoPin = 3;
int buzzer_flag = 0;
#define tone(...) void() // tone() inhibits receive timer 
#define noTone(a) void()

int buzzer = 12;
char array1[] = "[0] [1] [2] [3]";
//LiquidCrystal_I2C lcd(0x21, 16, 2);  // LCD 객체 선언 SDA A4 SCL A5

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  // lcd 기본 설정
  //lcd.init();
  //  lcd.backlight();
  // lcd.setCursor(0, 0);
  // I2C 통신
  Wire.begin(8);
  // 요청 받으면 Request Event를 실행
  Wire.onRequest(requestEvent);

  Serial.begin(9600); //시리얼 모니터를 시작합니다.
  Serial.println("Setup");
}

// 데이터를 수신할 때
void requestEvent() {
  // 마스터에게 전달해야 할 내용
  // 1. 남은 음료의 양
  // 2. 거리 감지를 통해 컵에 음료가 없다면 부저를 울리고 마스터에게 컵이 없다는 내용 전달
  Serial.println("장치가 연결되었습니다");
  // 거리를 가지고 Master에게 신호를 보내야함
  char buf[20];
  if (distance_check() > 6 ) {
    Serial.println("컵이 감지가 되지 않습니다.");
    Wire.write("z");
    
    buzzer_flag = 1;
    
  }
  else {
    int val1 = analogRead(A0);  // 탄산수의 수위 측정
    int val2 = analogRead(A1);  // 음료 1
    int val3 = analogRead(A2);  // 음료 2
    int val4 = analogRead(A3);  // 음료 2
    // 퍼센테이지로 변환
    int per1 = change(val1); //최대 430 ~ 730
    int per2 = change(val2); //최대 430 ~ 730
    int per3 = change(val3); //최대 430 ~ 730
    int per4 = change(val4);

//    if (per1 < 30 ||  per2 < 30 ||  per3 < 30 || per4 < 30) {
//      if (per1 < 30) {
//        Serial.println("1번 음료가 부족합니다.");
//      }
//      if (per2 < 30) {
//        Serial.println("2번 음료가 부족합니다.");
//      }
//      if (per3 < 30) {
//        Serial.println("3번 음료가 부족합니다.");
//      }
//      if (per4 < 30) {
//        Serial.println("4번 음료가 부족합니다.");
//      }
//    }


    String a = (String)per1 + "/" + (String)per2 + "/" + (String)per3 + "/" + (String)per4;
    a.toCharArray(buf, a.length() + 1);
    for (int i = a.length(); i < 20; i++)
      buf[i] = 'X';
    Serial.println(buf);
    Wire.write(buf);
  }

}

int change(int val) {

  double ret = val - 300;
  ret = (ret / 400) * 100;
  if (ret < 0)
    ret = 0;
  else if (ret > 100)
    ret = 100;

  return (int)ret;
}
int distance_check() {
  // 거리감지부터 해야 함
  float duration, distance;

  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);

  // echoPin에 HIGH 시간 저장
  duration = pulseIn(echoPin, HIGH);
  // HIGH일 때 초음파가 보냈다가 돌아오는 시간을 가지고 거리 계산
  // 10000은 cm로 단위 변환을 위해 사용
  distance = ((float)(340 * duration) / 10000) / 2;
  Serial.print("거리 : ");
  Serial.print(distance);
  Serial.println("cm");
  return (int)distance;
}
//void check_amount() {
//  int val1 = analogRead(A0);  // 탄산수의 수위 측정
//  int val2 = analogRead(A1);  // 음료 1
//  int val3 = analogRead(A2);  // 음료 2
//  int val4 = analogRead(A3);  // 음료 2
//  // 퍼센테이지로 변환
//  int per1 = change(val1); //최대 430 ~ 730
//  int per2 = change(val2); //최대 430 ~ 730
//  int per3 = change(val3); //최대 430 ~ 730
//  int per4 = change(val4);
//  char lcd_buf[16];
//
//  // sprintf(lcd_buf, "%03d %03d %03d %03d", per1, per2, per3, per4);
//
//  //  Serial.println(lcd_buf);
//  //  b.toCharArray(lcd_buf,b.length()+1);
//  // lcd.setCursor(0, 1);
//  // lcd.print(lcd_buf);
//
//}

void loop() {
  // lcd.setCursor(0, 0);
  //lcd.print(array1);
  //check_amount();
  
  
  // requestEvent();
  if(buzzer_flag==1){
    digitalWrite(buzzer,HIGH);
    buzzer_flag = 0;
    delay(1000);
    digitalWrite(buzzer,LOW);
  }
  
}
