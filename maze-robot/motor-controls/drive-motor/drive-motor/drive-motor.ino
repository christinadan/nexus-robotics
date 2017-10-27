#include <SoftwareSerial.h>

int leftMotorCh0Pin = 2;
int leftMotorCh1Pin = 3;
int rightMotorCh0Pin = 4;
int rightMotorCh1Pin = 5;

byte newByte;

void setup() {
  // put your setup code here, to run once:
  pinMode(leftMotorCh0Pin, OUTPUT);
  pinMode(leftMotorCh1Pin, OUTPUT);
  pinMode(rightMotorCh0Pin, OUTPUT);
  pinMode(rightMotorCh1Pin, OUTPUT);

  Serial3.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  

}

void driveForward() {
  
}

