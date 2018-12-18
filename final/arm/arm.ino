#include <PS2X_lib.h>
#include <string.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

# define LL1 114.0
# define LL2 149.0

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
#define SERVO_NUM 10

#define moveEN 8
#define moveF 7
#define moveT 6

int counter;
int init_angle[SERVO_NUM] = {90, 90, 90, 90, 90, 90, 90, 90, 90, 90};
int angle[SERVO_NUM] = {90, 90, 90, 90, 90, 90, 90, 90, 90, 90};
int current[SERVO_NUM] = {90, 90, 90, 90, 90, 90, 90, 90, 90, 90};
int offset[SERVO_NUM] = {0, 0, 0, 0, 0, 5, 0, 0, 0, 0};
int set_angle[7][SERVO_NUM] = {
  {90, 30, 90, 0, -1, 90, 150, 90, 0, -1},
  {90, 90, -1, -1, 125, 90, 90, -1, -1, 65},
  {-1, -1, -1, -1, 65, -1, -1, -1, -1, 125},
  {90, 90, 90, -1, -1, 90, 90, 90, -1, -1},
  {90, 30, 90, 0, -1, 90, 150, 90, 0, -1},
  {170, 90, 50, 0, -1, 20, 90, 130, 0, -1}, 
  {70, 70, 20, 90, -1, 110, 110, 160, 90, -1},
  };
int state;
char get_char;
int get_t;

int armLx, armLy;

PS2X ps2x;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
int error = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  OCR0A = 254;            // compare match register 16MHz/256/2Hz
  TCCR0B |= (1 << WGM12);   // CTC mode
  TCCR0B |= (1 << CS12) || (1 << CS10);  //prescale
  TIMSK0 |= (1 << OCIE0A);  // enable timer compare interrupt
  counter = 0;
  state = 0;
  get_char = 0;
  get_t = 0;
  
  //error = ps2x.config_gamepad(PS2_clk, PS2_cmd, PS2_att, PS2_data, true, true);
  error = ps2x.config_gamepad(13,11,10,12, true, true);
  int type = ps2x.readType();
//  Serial.println(type);
  Serial.println(error);
  while (error != 0){
    delay(1000);
    //ps2x.reconfig_gamepad();
    //error = ps2x.config_gamepad(PS2_clk, PS2_cmd, PS2_att, PS2_data, true, true);
    error = ps2x.config_gamepad(13,11,10,12, true, true);
    Serial.println(error);
  }
  
  pinMode(moveEN, OUTPUT);
  pinMode(moveT, OUTPUT);
  pinMode(moveF, OUTPUT);
  pwm.begin();
  pwm.setPWMFreq(60);
  armLx = 0;
  armLy = 200;
  state = 0;
  delay(2000);
  
}

ISR(TIMER0_COMPA_vect)          // timer compare interrupt service routine
{
  counter++;
  if(counter == 20){
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

//void getcontrol(char*);
void loop() {
  for (int i=0;i<SERVO_NUM; i++){
    int t = map(current[i]+offset[i], 0, 180, SERVOMIN, SERVOMAX);
    pwm.setPWM(i+6, 0, t);
  }
  delay(50);
  
  ps2x.read_gamepad();
  int ry = 128;
  int rx = 128;
  ry = ps2x.Analog(PSS_RY);
  rx = ps2x.Analog(PSS_RX);
  int t = ps2x.readType();
  Serial.print(t);
  Serial.print(" ");
  Serial.print(ry);
  Serial.print(" ");
  Serial.println(rx);
  
  if(ry < 50) {      digitalWrite(moveEN, HIGH); digitalWrite(moveF, HIGH); digitalWrite(moveT, LOW); }
  else if(ry > 200) {digitalWrite(moveEN, HIGH); digitalWrite(moveF, LOW);  digitalWrite(moveT, LOW); }
  else if(rx > 200) {digitalWrite(moveEN, HIGH); digitalWrite(moveF, HIGH); digitalWrite(moveT, HIGH);}
  else if(rx <  50) {digitalWrite(moveEN, HIGH); digitalWrite(moveF, LOW);  digitalWrite(moveT, HIGH);}
  else {digitalWrite(moveEN, LOW);  digitalWrite(moveF, LOW);  digitalWrite(moveT, LOW);}

  
  bool right_arm_flag = (ps2x.Button(PSB_R1)) ? true : false;
  bool left_arm_flag = (ps2x.Button(PSB_R2)) ? true : false;
  if(ps2x.Button(PSB_R1) || (ps2x.Button(PSB_R2))) state = 0;
  Serial.print(state);
  Serial.print(" ");
  for (int i=0; i<SERVO_NUM; i++){Serial.print(angle[i]); Serial.print(" ");}

  if(ps2x.Button(PSB_PINK)){
    if (state == 0) state = 10;
    else if (state == 10){
      if(ps2x.Button(PSB_GREEN)) for (int i=0; i<SERVO_NUM; i++) angle[i] = (set_angle[4][i] >= 0) ? set_angle[4][i] : angle[i];
      else if(ps2x.Button(PSB_RED)) for (int i=0; i<SERVO_NUM; i++) angle[i] = (set_angle[5][i] >= 0) ? set_angle[5][i] : angle[i];
      else if(ps2x.Button(PSB_BLUE)) for (int i=0; i<SERVO_NUM; i++) angle[i] = (set_angle[6][i] >= 0) ? set_angle[6][i] : angle[i];
    }
    return;
  }
  
  if(ps2x.ButtonPressed(PSB_GREEN)){
    if(state == 0){
      state = 1;
      for (int i=0; i<SERVO_NUM; i++) angle[i] = (set_angle[0][i] >= 0) ? set_angle[0][i] : angle[i];
    }
    else if (state == 1){
      state = 2;
      for (int i=0; i<SERVO_NUM; i++) angle[i] = (set_angle[1][i] >= 0) ? set_angle[1][i] : angle[i];
    }
    else if (state == 2){
      state = 3;
      for (int i=0; i<SERVO_NUM; i++) angle[i] = (set_angle[2][i] >= 0) ? set_angle[2][i] : angle[i];
    }
    else if (state == 3){
      state = 0;
      for (int i=0; i<SERVO_NUM; i++) angle[i] = (set_angle[3][i] >= 0) ? set_angle[3][i] : angle[i];
    }
    return;
  }

  if(ps2x.Button(PSB_RED)){
    for (int i=0; i<SERVO_NUM; i++) angle[i] = init_angle[i];
    return;
  }

  if(state == 10 && (!ps2x.Button(PSB_PINK))) state = 0;
  
  if(right_arm_flag){
    byte ly = ps2x.Analog(PSS_LY);
    byte lx = ps2x.Analog(PSS_LX);
    if(ly > 150) angle[1] -= 2;
    else if(ly < 100) angle[1] += 2;
    if(lx >150) angle[2] -= 2;
    else if(lx < 100) angle[2] += 2;
    angle[1] = (angle[1] > 170) ? 170 : (angle[1] < 5) ? 5 : angle[1];
    angle[2] = (angle[2] > 170) ? 170 : (angle[2] < 5) ? 5 : angle[2];
    //cal_armL(&armLx, &armLy, dx, dy, &angle[1], &angle[2]);
    
    if(ps2x.Button(PSB_PAD_UP)) angle[0] += 2;
    else if(ps2x.Button(PSB_PAD_DOWN)) angle[0] -= 2;
    if(ps2x.Button(PSB_PAD_RIGHT)) angle[3] -= 5;
    else if(ps2x.Button(PSB_PAD_LEFT)) angle[3] += 5;
    if(ps2x.Button(PSB_L1)) angle[4] += 10;
    else if(ps2x.Button(PSB_L2)) angle[4] -= 10;
    angle[0] = (angle[0] > 170) ? 170 : (angle[0] < 5) ? 5 : angle[0];
    angle[3] = (angle[3] > 170) ? 170 : (angle[3] < 5) ? 5 : angle[3];
    angle[4] = (angle[4] > 125) ? 125 : (angle[4] < 65) ? 65 : angle[4];
  }
  if(left_arm_flag){
    byte ly = ps2x.Analog(PSS_LY);
    byte lx = ps2x.Analog(PSS_LX);
    if(ly > 150) angle[6] += 2;
    else if(ly < 100) angle[6] -= 2;
    if(lx >150) angle[7] += 2;
    else if(lx < 100) angle[7] -= 2;
    angle[6] = (angle[6] > 170) ? 170 : (angle[6] < 5) ? 5 : angle[6];
    angle[7] = (angle[7] > 170) ? 170 : (angle[7] < 5) ? 5 : angle[7];
    //cal_armL(&armLx, &armLy, dx, dy, &angle[1], &angle[2]);
    
    if(ps2x.Button(PSB_PAD_UP)) angle[5] -= 2;
    else if(ps2x.Button(PSB_PAD_DOWN)) angle[5] += 2;
    if(ps2x.Button(PSB_PAD_RIGHT)) angle[8] += 5;
    else if(ps2x.Button(PSB_PAD_LEFT)) angle[8] -= 5;
    if(ps2x.Button(PSB_L1)) angle[9] += 10;
    else if(ps2x.Button(PSB_L2)) angle[9] -= 10;
    angle[5] = (angle[5] > 170) ? 170 : (angle[5] < 5) ? 5 : angle[5];
    angle[8] = (angle[8] > 170) ? 170 : (angle[8] < 5) ? 5 : angle[8];
    angle[9] = (angle[9] > 125) ? 125 : (angle[9] < 65) ? 65 : angle[9];
  }
  if(ps2x.ButtonPressed(PSB_BLUE) && state == 0){
    byte ly = ps2x.Analog(PSS_LY);
    byte lx = ps2x.Analog(PSS_LX);
    if(ly > 150 || ly < 100) angle[6] = 180-angle[1];
    if(lx > 150 || lx < 100) angle[7] = 180-angle[2];
    
    if(ps2x.Button(PSB_PAD_UP) || ps2x.Button(PSB_PAD_DOWN)) angle[5] = 180-angle[0];
    if(ps2x.Button(PSB_PAD_RIGHT) || ps2x.Button(PSB_PAD_LEFT)) angle[8] = 180-angle[3];
    if(ps2x.Button(PSB_L1)) angle[9] = angle[4];
  }

  if(!left_arm_flag && !right_arm_flag && state == 2){
    if(ps2x.Button(PSB_PAD_UP)) angle[5] -= 2;
    else if(ps2x.Button(PSB_PAD_DOWN)) angle[5] += 2;
    angle[5] = (angle[5] > 170) ? 170 : (angle[5] < 5) ? 5 : angle[5];
    byte lx = ps2x.Analog(PSS_LX);
    if(lx >150) angle[2] -= 2;
    else if(lx < 100) angle[2] += 2;
    angle[2] = (angle[2] > 170) ? 170 : (angle[2] < 5) ? 5 : angle[2];
    
    angle[7] = 180-angle[2];
  }
  
}
