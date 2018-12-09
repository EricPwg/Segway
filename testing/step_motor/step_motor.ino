#include <SoftwareSerial.h>

# define RW_Cpin 12
# define LW_Cpin 10
# define RW_Dpin 11
# define LW_Dpin 9
# define LFW_logic 1
# define RFW_logic 0

int enL;
int enR;
int counterL_reload;
int counterR_reload;
int counterL;
int counterR;
bool RW_state;
bool LW_state;

SoftwareSerial mySerial(7,8);

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  mySerial.begin(9600);
  //Serial.begin(9600);
  //delaynum = 10;
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 400;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS10);  //prescale
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  enR = 0;
  enL = 0;
  counterR_reload = 0;
  counterL_reload = 0;
  counterR = 0;
  counterL = 0;
  RW_state = false;
  LW_state = false;
}

ISR(TIMER1_COMPA_vect){
  if (enL){
    if(!counterL--){
        LW_state = !LW_state;
        digitalWrite(LW_Cpin, LW_state);
        counterL = counterL_reload;
    }
  }
  else digitalWrite(LW_Cpin, LOW);

  if (enR){
    if(!counterR--){
        RW_state = !RW_state;
        digitalWrite(RW_Cpin, RW_state);
        counterR = counterR_reload;
    }
  }
  else digitalWrite(RW_Cpin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  //enL = 1;
  //enR = 1;
  //counterL_reload = 50;
  //counterR_reload = 50;
  //digitalWrite(LW_Dpin, LFW_logic);
  //digitalWrite(RW_Dpin, RFW_logic);
  if(mySerial.available()){
    //Serial.print("123 ");
    char c = mySerial.read();
    //Serial.println((byte)c);
    if (((byte)c & 128) != 0){
      bool flag = false;
      int *p;
      switch((byte)c-128){
        case 0: digitalWrite(LW_Dpin, LFW_logic);  enL = 1; p = &counterL_reload; flag = true; break;
        case 1: digitalWrite(LW_Dpin, !LFW_logic); enL = 1; p = &counterL_reload; flag = true; break;
        case 2: digitalWrite(RW_Dpin, RFW_logic);  enR = 1; p = &counterR_reload; flag = true; break;
        case 3: digitalWrite(RW_Dpin, !RFW_logic); enR = 1; p = &counterR_reload; flag = true; break;
        case 4: enL = 0; break;
        case 5: enR = 0; break;
        default: break;
      }
      if (flag){
        while(!mySerial.available());
        char c1 = mySerial.read();
        if((int)c1 & 128 != 0) return;
        int t = (int)c1;
        *p = t;
      }
    }
  }
}
