#include "DriveMotor.h"

#define Kp 0.35
#define Kd 0.35
#define rightMaxSpeed 250
#define leftMaxSpeed 250
#define rightBaseSpeed 125
#define leftBaseSpeed 125
#define setDist 180

#define turnDelay 400
#define clearDelay 1000
#define wheelModifier 40

DriveMotor::DriveMotor(const byte lm0, const byte lm1, const byte rm0, const byte rm1) {
  leftMotorCh0Pin = lm0;
  leftMotorCh1Pin = lm1;
  rightMotorCh0Pin = rm0;
  rightMotorCh1Pin = rm1;

  pinMode(leftMotorCh0Pin, OUTPUT);
  pinMode(leftMotorCh1Pin, OUTPUT);
  pinMode(rightMotorCh0Pin, OUTPUT);
  pinMode(rightMotorCh1Pin, OUTPUT);
}

int DriveMotor::driveForward(const int leftDist, const int lastError) {
  
  int error = leftDist - setDist;

  int delta = (Kp * error) + (Kd * (error - lastError));

  int leftMotorSpeed = leftBaseSpeed - delta;
  int rightMotorSpeed = rightBaseSpeed + delta;

  if (rightMotorSpeed > rightMaxSpeed) rightMotorSpeed = rightMaxSpeed;
  if (leftMotorSpeed > leftMaxSpeed) leftMotorSpeed = leftMaxSpeed;
  if (rightMotorSpeed < 0) rightMotorSpeed = 0;
  if (leftMotorSpeed < 0) leftMotorSpeed = 0;

  analogWrite(leftMotorCh0Pin, leftMotorSpeed);
  analogWrite(leftMotorCh1Pin, 0);

  analogWrite(rightMotorCh0Pin, rightMotorSpeed);
  analogWrite(rightMotorCh1Pin, 0);
  return error;
}

void DriveMotor::turnPositioning(const int delayTime) {
  analogWrite(leftMotorCh0Pin, leftBaseSpeed);
  analogWrite(leftMotorCh1Pin, 0);

  analogWrite(rightMotorCh0Pin, rightBaseSpeed);
  analogWrite(rightMotorCh1Pin, 0);
  
  delay(delayTime);
}

void DriveMotor::adjustWideLeftTurn(const int leftDistance) {
  if(leftDistance > 400) {
    analogWrite(leftMotorCh0Pin, leftBaseSpeed - 20);
    analogWrite(leftMotorCh1Pin, 0);

    analogWrite(rightMotorCh0Pin, rightBaseSpeed);
    analogWrite(rightMotorCh1Pin, 0);

    delay(100);
  }
}

void DriveMotor::adjustAfterLeftTurn() {
  analogWrite(leftMotorCh0Pin, leftBaseSpeed);
  analogWrite(leftMotorCh1Pin, 0);

  analogWrite(rightMotorCh0Pin, rightBaseSpeed - 15);
  analogWrite(rightMotorCh1Pin, 0);
 
  delay(1200);
}

void DriveMotor::stopMotors() {
  analogWrite(leftMotorCh0Pin, 0);
  analogWrite(leftMotorCh1Pin, 0);

  analogWrite(rightMotorCh0Pin, 0);
  analogWrite(rightMotorCh1Pin, 0);
}

void DriveMotor::turnRight() {
  analogWrite(leftMotorCh0Pin, leftBaseSpeed);
  analogWrite(leftMotorCh1Pin, 0);

  analogWrite(rightMotorCh0Pin, 0);
  analogWrite(rightMotorCh1Pin, rightBaseSpeed);
  delay(turnDelay);
  
//  analogWrite(leftMotorCh0Pin, leftBaseSpeed + wheelModifier);
//  analogWrite(leftMotorCh1Pin, 0);
//
//  analogWrite(rightMotorCh0Pin, rightBaseSpeed);
//  analogWrite(rightMotorCh1Pin, 0);
//  delay(clearDelay);
}

void DriveMotor::turnLeft() {
  analogWrite(leftMotorCh0Pin, 0);
  analogWrite(leftMotorCh1Pin, leftBaseSpeed);

  analogWrite(rightMotorCh0Pin, rightBaseSpeed - 40);
  analogWrite(rightMotorCh1Pin, 0);
  delay(turnDelay);

//  analogWrite(leftMotorCh0Pin, leftBaseSpeed);
//  analogWrite(leftMotorCh1Pin, 0);
//
//  analogWrite(rightMotorCh0Pin, rightBaseSpeed + 100);
//  analogWrite(rightMotorCh1Pin, 0);
//  delay(clearDelay);
}

void DriveMotor::turnAround() {
  DriveMotor::turnLeft();
  DriveMotor::turnLeft();
  delay(turnDelay);
}

