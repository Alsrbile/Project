#include <Wire.h>
#include <math.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm=Adafruit_PWMServoDriver();
#define degrees(rad) ((rad)*RAD_TO_DEG)

#include <Stepper.h>

const int STEPS = 2048;
Stepper stepper1 (STEPS, 1,2,3,4);
Stepper stepper2 (STEPS, 5,6,7,8);

int data;

//스텝모터 함수
void Car_lift_1(int a){

  int degree1=map(a,0,360,0,2048);
  stepper1.step(degree1);
    delay(1000);
}

void Car_lift_2(int b){

  int degree2=map(b,0,360,0,2048);
  stepper2.step(degree2);
    delay(1000);
}

void first_R_arm(){

  Inverse_k(138, 55, 154, 300);
  Inverse_k(143, 51, 154, 300);
  Inverse_k(148, 58, 154, 300);
  Inverse_k(153, 60, 154, 300);
  Inverse_k(158, 60, 154, 300);

  Inverse_k(163, 60, 154, 300);
  Inverse_k(168, 60, 154, 300);
  Inverse_k(173, 60, 154, 300);

  Inverse_k(178, 60, 154, 3000);
  
  Inverse_k(173, 60, 154, 300);
  Inverse_k(168, 60, 154, 300);
  Inverse_k(163, 60, 154, 300);
  Inverse_k(158, 60, 154, 300);
  Inverse_k(153, 55, 154, 300);
  
  Inverse_k(148, 50, 154, 300);
}

void second_R_arm(){
  Inverse_k(135, 55, 40, 300);
  Inverse_k(140, 50, 40, 300);
  Inverse_k(145, 45, 40, 300);
  Inverse_k(150, 40, 40, 300);
  Inverse_k(155, 40, 40, 300);

  Inverse_k(158, 40, 40, 300);
  Inverse_k(160, 40, 40, 300);
  Inverse_k(163, 40, 40, 300);
  Inverse_k(165, 40, 40, 300);
  Inverse_k(170, 40, 40, 300);

  Inverse_k(173, 40, 40, 300);

  Inverse_k(175, 40, 40, 6000);

  Inverse_k(173, 40, 40, 300);
  Inverse_k(170, 40, 40, 300);
  Inverse_k(167, 40, 40, 300);
  Inverse_k(165, 40, 40, 300);
  Inverse_k(160, 40, 40, 300);

  Inverse_k(155, 40, 40, 300);
  Inverse_k(150, 40, 40, 300);
  Inverse_k(145, 45, 40, 300);
  Inverse_k(140, 48, 40, 300);
  Inverse_k(135, 50, 40, 300);
}

void func_pwm(int angle1, int angle2, int angle3, int angle4, int delay_a=0){
  
  int m1 = map(angle1,0,180,150,600);
  int m2 = map(angle2,0,180,150,600);
  int m3 = map(angle3,0,180,150,600);
  int m4 = map(angle4,0,180,150,600);

  pwm.setPWM(0, 0, m1);
  pwm.setPWM(4, 0, m2);
  pwm.setPWM(8, 0, m3);
  pwm.setPWM(12, 0, m4);

  delay(delay_a);
}

void Inverse_k(int x, int y, int angle4, int delay_s){
  int a1 = 60;    
  int a2 = 86;    
  int a3 = 64;    

  int px=x;
  int py=-y;
  int theta_d = 0;
  
  int p2x = px - a3*cos(theta_d);
  int p2y = py - a3*sin(theta_d);

  float c2 = (pow(p2x,2) + pow(p2y,2) - pow(a1,2) - pow(a2,2))/(2*a1*a2);
  float s2 = sqrt(1 - pow(c2,2));
  float theta_2 = -atan2(s2, c2); 

  float s1 = ((a1+a2*c2)*p2y - a2*s2*p2x)/(pow(p2x,2) + pow(p2y,2)); //calculation for theta1
  float c1 = ((a1+a2*c2)*p2x + a2*s2*p2y)/(pow(p2x,2) + pow(p2y,2));
  float theta_1 = -atan2(s1,c1);

  float theta_3 = 0-theta_1-theta_2;

  float theta_1_d = degrees(theta_1) - 32;
  float theta_2_d = -degrees(theta_2) - 16;
  float theta_3_d = degrees(theta_3) + 80;
  
  int m1 = map(theta_1_d,0,180,150,600);
  int m2 = map(theta_2_d,0,180,150,600);
  int m3 = map(theta_3_d,0,180,150,600);
  int m4 = map(angle4,0,180,150,600);

  pwm.setPWM(0, 0, m1);
  pwm.setPWM(4, 0, m2);
  pwm.setPWM(8, 0, m3);
  pwm.setPWM(12, 0, m4);

  delay(delay_s);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  // digitalWrite(13, LOW);

  pwm.begin();
  pwm.setPWMFreq(51);

  // 들어가는 스위치
  pinMode(11, INPUT_PULLUP);

  stepper1.setSpeed(1);
  stepper2.setSpeed(2);
  
  pinMode(12, INPUT_PULLUP);

  Car_lift_2(2);
  //pinMode(12, INPUT_PULLUP);
  func_pwm(90,90,90,90,10);
}



void loop() {
  // put your main code here, to run repeatedly:

  // int sw1=digitalRead(12);
  
  int sw=digitalRead(11);

  if(sw == LOW){
    Serial.println(10);
    delay(500);
  }
  

  Serial.println(0);
  delay(80);

  while (Serial.available()){
    data = Serial.read();
    Serial.print(data);
  }

  
if (data == '1'){
  Serial.end();
// first_R_arm();
// func_pwm(90,90,90,90,10);

  Car_lift_2(0);
  Car_lift_1(27);
  delay(100);
  Car_lift_2(82);
  delay(100);
  Car_lift_1(-27);
  delay(100);
  Car_lift_2(-82);
  
  delay(5000);
  
  Car_lift_2(82);
  delay(100);
  Car_lift_1(27);
  delay(100);
  Car_lift_2(-82);
  delay(100);
  Car_lift_1(-27);

}
else{
    func_pwm(90,90,90,90,10);
  }
}