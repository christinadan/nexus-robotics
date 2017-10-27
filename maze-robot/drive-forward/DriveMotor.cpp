#include "DriveMotor.h"

#define Kp 0.25
#define Kd 0.25
#define rightMaxSpeed 250
#define leftMaxSpeed 250
#define rightBaseSpeed 150
#define leftBaseSpeed 150
#define setDist 146.05

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

  int leftMotorSpeed = leftBaseSpeed + delta;
  int rightMotorSpeed = rightBaseSpeed - delta;

  if (rightMotorSpeed > rightMaxSpeed) rightMotorSpeed = rightMaxSpeed;
  if (leftMotorSpeed > leftMaxSpeed) leftMotorSpeed = leftMaxSpeed;
  if (rightMotorSpeed < 0) rightMotorSpeed = 0;
  if (leftMotorSpeed < 0) leftMotorSpeed = 0;

  analogWrite(leftMotorCh0Pin, leftMotorSpeed);
  analogWrite(leftMotorCh1Pin, 0);

  analogWrite(rightMotorCh0Pin, rightMotorSpeed);
  analogWrite(rightMotorCh1Pin, 0);

  return lastError;
}

void DriveMotor::stopMotors() {
  analogWrite(leftMotorCh0Pin, 0);
  analogWrite(leftMotorCh1Pin, 0);

  analogWrite(rightMotorCh0Pin, 0);
  analogWrite(rightMotorCh1Pin, 0);
}
