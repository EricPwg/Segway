char a = '9';
bool blinkstate;
bool blinkstate2;
int delaynum;
int speed1, count, d;

void setup() {
  // put your setup code here, to run once:
  cli();
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.begin(250000);
  blinkstate = 0;
  blinkstate2 = 0;
  //delaynum = 10;
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 20;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);  //prescale
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  OCR2A = 10;            // compare match register 16MHz/256/2Hz
  TCCR2A |= (1 << WGM21);   // CTC mode
  TCCR2B |= (1 << CS22);  //prescale
  TIMSK2 |= (1 << OCIE2A);  // enable timer compare interrupt
  d = 0;
  count = 0;
  
  speed1 = 100;
  sei();
}

ISR(TIMER1_COMPA_vect){
  //Serial.println(TCNT1);
    blinkstate = !blinkstate;
    digitalWrite(13, blinkstate);
}

ISR(TIMER2_COMPA_vect){
    //Serial.println(TCNT2);
    blinkstate2 = !blinkstate2;
    digitalWrite(12, blinkstate2);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(CS10);
  //digitalWrite(12, d);
  return ;
  if (Serial.available()){
    a = Serial.read();
    TCNT1 = 0;
    if (a == '1'){
      OCR1A = 0;
    }
    else if (a == '2'){
      OCR1A = 1;
    }
    else if (a == '3'){
      OCR1A = 2;
    }
    else if (a == '4'){
      OCR1A = 16;
    }
    else if (a == '5'){
      OCR1A = 32;
    }
    else if (a == '6'){
      OCR1A = 64;
    }
    else if (a == '7'){
      OCR1A = 128;
    }
    else if (a == '8'){
      OCR1A = 254;
    }
  }
  
  //blinkstate = !blinkstate;
  //digitalWrite(13, blinkstate);
  //delayMicroseconds(delaynum);
}
