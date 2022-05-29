#include <Wire.h>

int LED1 = 2;
int LED2 = 3;
int LED3 = 4;

int trigPin = 8;
int echoPin = 9;

int speakerPin = 12;

void setup() {
  Serial.begin(9600); //시리얼 모니터를 시작합니다.
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // I2C 통신 
  Wire.begin(8);
  // 요청 받으면 Request Event를 실행
  Wire.onRequest(requestEvent); 
  
}

// 데이터를 수신할 때
void requestEvent(){
  // 마스터에게 전달해야 할 내용
  // 1. 남은 음료의 양
  // 2. 거리 감지를 통해 컵에 음료가 없다면 부저를 울리고 마스터에게 컵이 없다는 내용 전달 

  // 거리를 가지고 Master에게 신호를 보내야함
  // if(distance()){
  //  
  // }
 
  int val1 = analogRead(A0);  // 탄산수의 수위 측정
  int val2 = analogRead(A1);  // 음료 1
  int val3 = analogRead(A2);  // 음료 2

  // water 함수에서 어떤걸 구현할지?
  // water에선 남은 음료의 양을 구해야 하고 가능하다면 실시간으로 물의 양을 알 수 있게 하는게 베스트
  
  byte data = water(val);
  delay(100);

  // 남은 물양 전송 
  Wire.write(data); 
}
int water(int val){
   byte check = 0;
   // 끝부분에 살짝 닿았을 경우 센서값은 500~
   // 센서가 거의 잠기면 ~680
  
   // 500부터 680까지의 값을 백분율로 표시해야 함 

   int per = (val - 500) / 2;


   // 음료가 부족하면 check, 0을 전달함
   if(per < 20){
    tone(speakerPin, 1000);
    // Wire.write(check);
   }
   return per;
}

bool distance(){
  // 거리감지부터 해야 함 
  float duration, distance; 
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin,LOW);

  // echoPin에 HIGH 시간 저장
  duration = pulseIn(echoPin, HIGH);
  // HIGH일 때 초음파가 보냈다가 돌아오는 시간을 가지고 거리 계산 
  distance = ((float)(340*duration) / 10000) / 2;
}
void loop() {
  
  Serial.println(val);   //시리얼 모니터에 값을 출력합니다.
  delay(100);
  
  Serial.print("Duration: ");
  Serial.println(duration);
  Serial.print("Distance: ");
  Serial.println (distance);
  
  delay(400);
  
  
}
