#ifndef _DRIVE_MOTOR_H_
#define _DRIVE_MOTOR_H_

#define DEBUG_SERIAL_PORT (Serial3)

#include "Arduino.h"

class DriveMotor {
  private:
    byte leftMotorCh0Pin;
    byte leftMotorCh1Pin;
    byte rightMotorCh0Pin;
    byte rightMotorCh1Pin;
    
  public:
    DriveMotor(const byte lm0, const byte lm1, const byte rm0, const byte rm1);
    int driveForward(const int leftDist, const int lastError);
    void stopMotors();
};

#endif
