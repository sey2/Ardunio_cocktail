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
