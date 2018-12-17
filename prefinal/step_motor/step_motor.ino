#include <SoftwareSerial.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <math.h>
#define PI 3.14159265

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
#define OUTPUT_READABLE_ACCELGYRO
bool blinkState = false;
int count;
bool getdata;
double sum;
double isum[10];
# define Kp -250
# define Ki -200

# define RW_Cpin 12
# define LW_Cpin 10
# define RW_Dpin 11
# define LW_Dpin 9
# define LFW_logic 1
# define RFW_logic 0

#define moveEN 8
#define moveF 7
#define moveT 6

int enL;
int enR;
int counterL_reload;
int counterR_reload;
int counterL;
int counterR;
bool RW_state;
bool LW_state;

//SoftwareSerial mySerial(7,8);

//MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;


void set_data(double v){
    int n = (abs(v) <5 ) ? 0 : 20000/v;
    if (n == 0){
      enL = 0;
      enR = 0;
    }
    //Serial.println(n);
    else if(n > 0){
      digitalWrite(LW_Dpin, LFW_logic);  enL = 1;
      digitalWrite(RW_Dpin, RFW_logic);  enR = 1;
      counterL_reload = n;
      counterR_reload = n;
    }
    else if(n < 0){
      digitalWrite(LW_Dpin, !LFW_logic);  enL = 1;
      digitalWrite(RW_Dpin, !RFW_logic);  enR = 1;
      counterL_reload = -n;
      counterR_reload = -n;
    }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  //mySerial.begin(9600);
  Serial.begin(115200);
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
  counterR_reload = 25;
  counterL_reload = 25;
  counterR = 0;
  counterL = 0;
  RW_state = false;
  LW_state = false;
/*
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif
  accelgyro.initialize();
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  while(!accelgyro.testConnection());
  accelgyro.setXAccelOffset(420);
  accelgyro.setYAccelOffset(-3099);
  accelgyro.setZAccelOffset(1080);
  accelgyro.setXGyroOffset(36);
  accelgyro.setYGyroOffset(-31);
  accelgyro.setZGyroOffset(16);
*/
  pinMode(moveEN, INPUT);
  pinMode(moveT, INPUT);
  pinMode(moveF, INPUT);
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
  getdata = true;
}

void loop() {
  int en = digitalRead(moveEN);
  int t = digitalRead(moveT);
  int f = digitalRead(moveF);
  if (en == 0){
    enL = 0;
    enR = 0;
  }
  else if(en==1 && f==1 && t==0){
    enL = 1;
    enR = 1;
    digitalWrite(LW_Dpin, LFW_logic);
    digitalWrite(RW_Dpin, RFW_logic);
  }
  else if(en==1 && f==0 && t==0){
    enL = 1;
    enR = 1;
    digitalWrite(LW_Dpin, !LFW_logic);
    digitalWrite(RW_Dpin, !RFW_logic);
  }
  else if(en==1 && f==1 && t==1){
    enL = 1;
    enR = 1;
    digitalWrite(LW_Dpin, LFW_logic);
    digitalWrite(RW_Dpin, !RFW_logic);
  }
  else if(en==1 && f==0 && t==1){
    enL = 1;
    enR = 1;
    digitalWrite(LW_Dpin, !LFW_logic);
    digitalWrite(RW_Dpin, RFW_logic);
  }
  
  blinkState = !blinkState;
  digitalWrite(13, blinkState);
}
