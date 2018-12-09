#include <SoftwareSerial.h>

SoftwareSerial mySerial(7,8);

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  Serial.begin(115200);
}

void send_data(int v){
    int n = v;
    Serial.println(n);
    if(n > 0){
      int n1 = n%128;
      mySerial.write((char)128);
      mySerial.write((char)n1);
      mySerial.write((char)130);
      mySerial.write((char)n1);
    }
    else if(n < 0){
      int n1 = -n%128;
      mySerial.write((char)129);
      mySerial.write((char)n1);
      mySerial.write((char)131);
      mySerial.write((char)n1);
    }
    else{
      mySerial.write((char)132);
      mySerial.write((char)133);
    }
}


void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    int a = Serial.parseInt();
    send_data(a);
  }
  /*
  int t[10] = {128, 11, 0, 130, 11, 0};
  for (int i=0; i<6; i++) mySerial.write((char)t[i]);
  delay(1000);
  */
  //int t2[10] = {128, 10, 0, 130, 100, 0};
  //for (int i=0; i<6; i++) mySerial.write((char)t2[i]);
  //delay(1000);
  //int t3[10] = {128, 100, 0, 130, 1000, 0};
  //for (int i=0; i<6; i++) mySerial.write((char)t3[i]);
  //delay(1000);
  //int t3[10] = {132, 133};
  //for (int i=0; i<2; i++) mySerial.write((char)t3[i]);
  //delay(10);
}
