char result;

void setup(){
  Serial.begin(9600);
}
void loop(){
  while(Serial.available()>0){
    result = Serial.read();
    switch(result){
      case '0':
          Serial.println("0");
          break;
      case '1':
          Serial.println("1");
          break;
      case '2':
          Serial.println("2");
          break;
      case '3':
          Serial.println("3");
          break;
    }
  }
  delay(100);
}