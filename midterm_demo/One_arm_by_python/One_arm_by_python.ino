#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
//#include<Servo.h>
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
#define SERVO_NUM 5

//Servo myServo1;
//Servo myServo2;
//Servo myServo[SERVO_NUM];
//int servo_pin[SERVO_NUM] = {8, 9, 10, 11, 12};

int counter;
int angle[SERVO_NUM] = {90, 90, 90, 90, 90};
//int angle2;
int current[SERVO_NUM] = {90, 90, 90, 90, 90};
//int current2;
int state;

char get_char;
int get_t;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  // put your setup code here, to run once:
  pwm.begin();
  pwm.setPWMFreq(60);
  /*
  for (int i=0; i<SERVO_NUM; i++){
    myServo[i].attach(servo_pin[i]);
  }
  */
  //myServo1.attach(9);
  //myServo2.attach(10);
  pinMode(13, OUTPUT);

  
  OCR0A = 254;            // compare match register 16MHz/256/2Hz
  TCCR0B |= (1 << WGM12);   // CTC mode
  TCCR0B |= (1 << CS12) || (1 << CS10);  //prescale
  TIMSK0 |= (1 << OCIE0A);  // enable timer compare interrupt
  
  Serial.begin(115200);
  //angle1 = 45;
  //angle2 = 45;
  //current1 = 45;
  //current2 = 45;
  counter = 0;
  state = 0;
  get_char = 0;
  get_t = 0;
}

void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000000;  // convert to us
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

ISR(TIMER0_COMPA_vect)          // timer compare interrupt service routine
{
  counter++;
  if(counter == 5){
    digitalWrite(13, digitalRead(13) ^ 1);   // toggle LED pin
    for (int i=0; i<SERVO_NUM; i++){
      if (angle[i] > current[i]){
        current[i] ++;
      }
      else if (angle[i] < current[i]){
       current[i]--;
      }
      counter = 0;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  if (Serial.available() > 0){
    get_char = Serial.read();
    if (get_char == ' '){
    angle[state] = get_t;
    get_t = 0;
    state=state+1;
    get_char = 0;
    if (state==SERVO_NUM){
      state=0;
    }
   }
   else{
        get_t = get_t*10+((int)get_char-48);
   }
  }*/

  

  //Serial.println(get_t);
  //Serial.print(get_char);
  //Serial.print(get_t);
  //Serial.print(" ");
  //Serial.print(angle[0]);
  //Serial.println(current[0]);

  for (int i=0;i<SERVO_NUM; i++){
    int t = map(current[i], 0, 180, SERVOMIN, SERVOMAX);
    pwm.setPWM(i+11, 0, t);
  }
  //myServo[SERVO_NUM].write(current[SERVO_NUM]);
}
