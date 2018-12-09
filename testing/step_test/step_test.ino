char a = '8';
bool blinkstate;
int delaynum;
int speed1, count, d;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.begin(9600);
  blinkstate = 0;
  //delaynum = 10;
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 100;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS10);  //prescale
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  d = 0;
  count = 0;
  
  speed1 = 100;
}

ISR(TIMER1_COMPA_vect){
  if (count > 0){
    blinkstate = !blinkstate;
    digitalWrite(12, blinkstate);
    if (!blinkstate) count--;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(CS10);
  if (Serial.available()){
    d = Serial.parseInt();
    speed1 = Serial.parseInt();
    count = Serial.parseInt();
  }
  digitalWrite(11, d);
  OCR1A = speed1;
  return ;
  if (a == '3'){
    //OCR0A = 2;
  }
  else if (a == '4'){
    //OCR0A = 16;
  }
  else if (a == '5'){
    //OCR0A = 32;
  }
  else if (a == '6'){
    //OCR0A = 64;
  }
  else if (a == '7'){
    //OCR0A = 128;
  }
  else if (a == '8'){
    //OCR0A = 254;
  }
  //blinkstate = !blinkstate;
  //digitalWrite(13, blinkstate);
  //delayMicroseconds(delaynum);
}
