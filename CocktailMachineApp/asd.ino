
void setup() {
  /* 시리얼 창 설정 */
  Serial.begin(9600);
  blueToothSerial.begin(9600);
  dht.begin();

  Wire.begin();  
  Wire.requestFrom(8, 20); 
}
