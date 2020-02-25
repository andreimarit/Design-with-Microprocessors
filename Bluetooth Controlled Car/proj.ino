#include <Servo.h>
// Pinii motor 1
#define mpin00 5
#define mpin01 6
// Pinii motor 2
#define mpin10 3
#define mpin11 11
//Pin motor Servo
#define mServoPin 8
//pin buzzer
#define buzzerPin 7
//pinii sonar sensor
#define trigPin 9
#define echoPin 10

long duration;
int distance;
//char from bluethoot
char t;
int myDistance = 1;
int mySpeed;
bool servoBool = false;
Servo srv;

void setup() {
 // configurarea pinilor motor ca iesire, initial valoare 0
 digitalWrite(mpin00, 0);
 digitalWrite(mpin01, 0);
 digitalWrite(mpin10, 0);
 digitalWrite(mpin11, 0);
 pinMode (mpin00, OUTPUT);
 pinMode (mpin01, OUTPUT);
 pinMode (mpin10, OUTPUT);
 pinMode (mpin11, OUTPUT);
 // pin LED
 pinMode(13, OUTPUT);
 pinMode(9, OUTPUT);
 //Bluetooth device
 Serial.begin(9600);//serial for bluetooth
 //Serial1.begin(9600); // Serial interface for Board
 //Sonar Sensor
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 //Buzzer
 pinMode(buzzerPin, OUTPUT);
}
// Function to control a motor
// Input: pins m1 and m2, direction and speed
void StartMotor (int m1, int m2, int forward, int speed)
{
 if (speed==0) // stop
 {
 digitalWrite(m1, 0);
 digitalWrite(m2, 0);
 }
 else
 {
 if (forward)
 {
 digitalWrite(m2, 0);
 analogWrite(m1, speed); // use PWM
 }
 else
 {
 digitalWrite(m1, 0);
 analogWrite(m2, speed);
 }
 }
}
// Safety function
// Commands motors to stop, then delays
void delayStopped(int ms)
{
 StartMotor (mpin00, mpin01, 0, 0);
 StartMotor (mpin10, mpin11, 0, 0);
 delay(ms);
 //noTone(buzzerPin);
}

void playWithServo(int pin)
{
 srv.attach(pin);
 srv.write(0);
 delay(1000);
 srv.write(180);
 delay(1000);
 srv.write(90);
 delay(1000);
 srv.detach();
}

void ServoToInitialPosition(int pin)
{
  srv.attach(pin);
  srv.write(90);
  delay(100);
  srv.detach();
}

void ServoToFullRight(int pin)
{
  srv.attach(pin);
  srv.write(0);
  delay(100);
  srv.detach();
}

void ServoToFullLeft(int pin)
{
  srv.attach(pin);
  srv.write(180);
  delay(100);
  srv.detach();
}

void ServoSteeringRight(int pin)
{
  srv.attach(pin);
  srv.write(45);
  delay(100);
  srv.detach();
}

void ServoSteeringLeft(int pin)
{
  srv.attach(pin);
  srv.write(135);
  delay(250);
  srv.detach();
}

void ServoAction(int pin)
{
  //SONAR SENSOR
 digitalWrite(trigPin, LOW);
 delayMicroseconds(2);

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 distance = duration*0.034/2;
 if (distance <1) distance=100;
}

void buzzerFunction(int pin, int dist)
{
  myDistance = myDistance + 100/dist;
  
  if (myDistance >=  30){
    tone(pin, 1000, 250);
    //noTone(pin);
    myDistance=0;
  }
  
}

void loop() {
 //Bluetooth part
if(Serial.available()){
  t = Serial.read();
//  Serial.print("distance=");
//  Serial.println(distance);
//
//  Serial.print("myDistance=");
//  Serial.println(myDistance);
  
  
  //Serial.println(myDistance);
//  Serial.println(t);
//if(Serial1.available()){
//   Serial.write(Serial1.read());
//   t = Serial1.read();
//   //Serial.print(t);
}
ServoAction(mServoPin);
if (distance < 5) {
    tone(buzzerPin, 1000, 1000);
    delayStopped(10);
    
  }
    else if (distance < 99) buzzerFunction(buzzerPin, distance);
delay(250);
if (t == 'S') {
  delayStopped(10);
  ServoToInitialPosition(mServoPin);
  servoBool = false;
  digitalWrite(13, LOW);
 }
 else if(t == 'F'){ 
  if (distance < 15) 
       {tone(buzzerPin, 1000, 1000);
        delayStopped(10);
       }
  else{
      StartMotor (mpin00, mpin01, 1, 128);
      StartMotor (mpin10, mpin11, 1, 128);
      }
 }
 
else if(t == 'B'){      
  
  StartMotor (mpin00, mpin01, 0, 128);
  StartMotor (mpin10, mpin11, 0, 128);
} 

else if(t=='X'){
  playWithServo(mServoPin);
 }
 
 else if(t=='R'){
   if(servoBool == false) {
    ServoToFullRight(mServoPin);
    servoBool = true;
   }
   //ServoAction(mServoPin);
if (distance < 10) delayStopped(10);
  else{
 StartMotor (mpin00, mpin01, 1, 128);
 StartMotor (mpin10, mpin11, 0, 128);
 }
 }
  
  else if(t=='L'){
   if(servoBool == false){
    ServoToFullLeft(mServoPin);
    servoBool = true;
   }
   //ServoAction(mServoPin);
if (distance < 10) delayStopped(10);
  else{
 StartMotor (mpin00, mpin01, 0, 128);
 StartMotor (mpin10, mpin11, 1, 128);
      }
  }

  else if(t=='Q'){
   if(servoBool == false){
    servoBool = true;
    ServoSteeringLeft(mServoPin);
   }
   //ServoAction(mServoPin);
  if (distance < 10) delayStopped(10);
  else{
 StartMotor (mpin00, mpin01, 1, 64);
 StartMotor (mpin10, mpin11, 1, 128);
      }
  }

  else if(t=='E'){
   if(servoBool == false){
    servoBool = true;
    ServoSteeringRight(mServoPin);
   }
   //ServoAction(mServoPin);
  if (distance < 10) delayStopped(10);
  else{
 StartMotor (mpin00, mpin01, 1, 128);
 StartMotor (mpin10, mpin11, 1, 64);
      }
  }

  //ServoAction(mServoPin);
  
      
} 
