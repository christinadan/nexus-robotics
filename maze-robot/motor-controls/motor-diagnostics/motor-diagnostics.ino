#include <SoftwareSerial.h>

int leftMotorCh0Pin = 2;
int leftMotorCh1Pin = 3;
int rightMotorCh0Pin = 4;
int rightMotorCh1Pin = 5;

int waitTime = 3000;

byte newByte;

//SoftwareSerial Xbee(8,9);

void setup() {
  // put your setup code here, to rnu once:
  pinMode(leftMotorCh0Pin, OUTPUT);
  pinMode(leftMotorCh1Pin, OUTPUT);
  pinMode(rightMotorCh0Pin, OUTPUT);
  pinMode(rightMotorCh1Pin, OUTPUT);

  Serial3.begin(9600);
  Serial.begin(9600);

  Serial.println("Ready, waiting for command");

}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial3.available() > 0) {
    newByte = Serial3.read();
    Serial3.println(newByte);
  
    if (newByte != -1) { 
      switch (newByte) {
        case '\r':
        case '\n':
        case 255:
          break;
          
        case 's':
          Serial.println("Motor Diagnostics Started");
          Serial3.println("Motor Diagnostics Started");
          runDiagnostics();
          break;
      }
    }
  }
}

void runDiagnostics() {
  driveForward(140);
  delay(waitTime);
  driveStop();
  delay(waitTime);
  
  driveBackward(140);
  delay(waitTime);
  driveStop();
  delay(waitTime);

  pivotLeft(140);
  delay(waitTime);
  driveStop();
  delay(waitTime);

  pivotRight(140);
  delay(waitTime);
  driveStop();
  delay(waitTime);
}

void driveForward(const int speed) {
  Serial3.println("Driving Forwards");

  analogWrite(leftMotorCh0Pin, speed);
  analogWrite(leftMotorCh1Pin, 0);

  analogWrite(rightMotorCh0Pin, speed);
  analogWrite(rightMotorCh1Pin, 0);
  
}

void driveBackward(const int speed) {
  Serial3.println("Driving Backwards");
  
  analogWrite(leftMotorCh0Pin, 0);
  analogWrite(leftMotorCh1Pin, speed);

  analogWrite(rightMotorCh0Pin, 0);
  analogWrite(rightMotorCh1Pin, speed);
}

void driveStop() {
  Serial3.println("Done");
  
  analogWrite(leftMotorCh0Pin, 0);
  analogWrite(leftMotorCh1Pin, 0);

  analogWrite(rightMotorCh0Pin, 0);
  analogWrite(rightMotorCh1Pin, 0);
}

void pivotLeft(const int speed) {
  Serial3.println("Pivoting Left");
  
  analogWrite(leftMotorCh0Pin, 0);
  analogWrite(leftMotorCh1Pin, speed);

  analogWrite(rightMotorCh0Pin, speed);
  analogWrite(rightMotorCh1Pin, 0);
}

void pivotRight(const int speed) {
  Serial3.println("Pivoting Right");
  
  analogWrite(leftMotorCh0Pin, speed);
  analogWrite(leftMotorCh1Pin, 0);

  analogWrite(rightMotorCh0Pin, 0);
  analogWrite(rightMotorCh1Pin, speed);
}

