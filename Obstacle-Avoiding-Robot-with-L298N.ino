#include <Servo.h>          //Servo motor library. This is standard library

//our L298N control pins
const int LeftMotorForward = 4;
const int LeftMotorBackward = 5;
const int RightMotorForward = 7;
const int RightMotorBackward = 6;

//sensor pins
#define trig A1 //analog input 1
#define echo A2 //analog input 2

#define maximum_distance 200
boolean goesForward = false;
int duration, distance = 100;

Servo servo_motor; //our servo name


void setup(){
  //Serial.begin(9600);

  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);
  
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  servo_motor.attach(9); //our servo pin
  servo_motor.write(60);
}

void loop(){

  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);
  Serial.println(distance);

  if (distance <= 25){
    moveStop();
    delay(300);
    moveBackward();
    delay(200);
    moveStop();
    delay(300);
    distanceRight = lookRight();
    delay(300);
    distanceLeft = lookLeft();
    delay(300);

    if (distance >= distanceLeft){
      turnRight();
      moveStop();
    }
    else{
      turnLeft();
      moveStop();
    }
  }
  else{
    moveForward(); 
  }
    // Write a pulse to the HC-SR04 Trigger Pin
            digitalWrite(trig, LOW);
            delayMicroseconds(2);
            digitalWrite(trig, HIGH);
            delayMicroseconds(10);
            digitalWrite(trig, LOW);
            // Measure the response from the HC-SR04 Echo Pin
            duration = pulseIn(echo, HIGH);
            // Determine distance from duration
            // Use 343 metres per second as speed of sound
            distance = (duration / 2) * 0.0343;
}

int lookRight(){  
  servo_motor.write(0);
  delay(500);
            digitalWrite(trig, LOW);
            delayMicroseconds(2);
            digitalWrite(trig, HIGH);
            delayMicroseconds(10);
            digitalWrite(trig, LOW);
            duration = pulseIn(echo, HIGH);
            distance = (duration / 2) * 0.0343;
  delay(100);
  servo_motor.write(60);
  return distance;
}

int lookLeft(){
  servo_motor.write(130);
  delay(500);
            digitalWrite(trig, LOW);
            delayMicroseconds(2);
            digitalWrite(trig, HIGH);
            delayMicroseconds(10);
            digitalWrite(trig, LOW);
            duration = pulseIn(echo, HIGH);
            distance = (duration / 2) * 0.0343;
  delay(100);
  servo_motor.write(60);
  return distance;
  delay(100);
}

void moveStop(){
  
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
}

void moveForward(){

  if(!goesForward){

    goesForward=true;
    
    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(RightMotorForward, HIGH);
  
    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW); 
  }
}

void moveBackward(){

  goesForward=false;

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
}

void turnRight(){

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
  delay(250);
  
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
 
  
  
}

void turnLeft(){

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);

  delay(250);
  
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
}