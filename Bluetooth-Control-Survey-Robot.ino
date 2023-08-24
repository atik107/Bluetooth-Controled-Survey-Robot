#include <SoftwareSerial.h>
#include "DHT.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

//L298n Motor Driver pins.
const int LeftMotorForward = 2;
const int LeftMotorBackward = 3;
const int RightMotorForward = 4;
const int RightMotorBackward = 5;

int en1=9;
int en2=6;


//Define the pins for the Bluetooth module
#define BT_RX_PIN 10
#define BT_TX_PIN 11

#define trig A2
#define echo A1

#define bazzer 13
#define PIRLED A3
#define GasLED 8

#define DHTPIN 12
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int flag=1;
float duration, distance;
int speed = 204; // 0 - 255.

// PIR motion sensor pin
const int pirPin = 7;

#define Threshold 130
#define MQ2pin 0
float sensorValue;  //variable to store sensor value

#define BMP280_ADDRESS 0x76
Adafruit_BMP280 bmp; // I2C


// Create the software serial object
SoftwareSerial B(BT_RX_PIN, BT_TX_PIN); // RX, TX pins //(pin 10 , 11)


void setup()
{
  B.begin(9600);

  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);
  //pinMode(en1,OUTPUT);
  //pinMode(en2,OUTPUT);

  pinMode(bazzer, OUTPUT);
  pinMode(PIRLED, OUTPUT);
  pinMode(GasLED, OUTPUT);

  dht.begin();

   // Set the PIR sensor pin as an input
  pinMode(pirPin, INPUT);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);


  unsigned status;
  status = bmp.begin(BMP280_ADDRESS);

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop()
{ 
  
  while(1 && flag==1) {
  if(B.available()){
    char value = B.read();

    switch (value) {
      case 'F':
            // Calculate distance
            digitalWrite(trig, LOW);
            delayMicroseconds(2);
            digitalWrite(trig, HIGH);
            delayMicroseconds(10);
            digitalWrite(trig, LOW);
            duration = pulseIn(echo, HIGH);
            distance = (duration / 2) * 0.0343;

        if(distance<=35)
        {
            moveStop();

            digitalWrite(bazzer, HIGH);
            delay(20);
            digitalWrite(bazzer,LOW);

        }
        else
        {
          moveForward();
        }
        break;
      case 'X':
          flag=0;
        break;
      case 'B':
        moveBackward();
        break;
      case 'L':
        turnLeft();
        break;
      case 'R':
        turnRight();
        break;
      case 'S':
        moveStop();
        break;
      case '0':
        speed = 100;
        break;
      case '1':
        speed = 140;
        break;
      case '2':
        speed = 153;
        break;
      case '3':
        speed = 165;
        break;
      case '4':
        speed = 178;
        break;
      case '5':
        speed = 191;
        break;
      case '6':
        speed = 204;
        break;
      case '7':
        speed = 216;
        break;
      case '8':
        speed = 229;
        break;
      case '9':
        speed = 242;
        break;
      case 'q':
        speed = 255;
        break;
      default:
        moveStop();
        break;
      }
    }
  }

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration / 2) * 0.0343;


  B.print(h);
  B.print(",");
  B.print(t);
  B.print(",");
  B.print(bmp.readPressure());
  B.print(",");
  B.print(distance);
  B.print(" cm");
  B.print(",");
  delay(20);


  // Read the PIR sensor output
  int motion = digitalRead(pirPin);

  // Check if motion is detected
  if (motion == HIGH) {
    B.print("YES");
    delay(100);
    digitalWrite(PIRLED, HIGH);
    delay(1000);
    digitalWrite(PIRLED,LOW);
    B.print(",");
  } else {
    B.print("NO");
    B.print(",");
  }


  sensorValue = analogRead(MQ2pin); // read analog input pin 0

  if(sensorValue > Threshold)
  {
    B.print("YES");
    delay(100);
    digitalWrite(GasLED, HIGH);
    delay(2000);
    digitalWrite(GasLED,LOW);
    flag=1;
    B.print(";");
  }
  else
  {
    B.print("NO");
    B.print(";");
  }
}

void moveStop(){
  analogWrite(en1,speed);
  analogWrite(en2,speed);
  
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
}

void moveForward(){
    analogWrite(en1,speed);
  analogWrite(en2,speed);
    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(RightMotorForward, HIGH);
    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW); 

}

void moveBackward(){
  analogWrite(en1,speed);
  analogWrite(en2,speed);

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
}

void turnRight(){
  analogWrite(en1,speed);
  analogWrite(en2,speed);

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
}

void turnLeft(){
  analogWrite(en1,speed);
  analogWrite(en2,speed);
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);

}
