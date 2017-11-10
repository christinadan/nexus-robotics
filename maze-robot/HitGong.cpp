#include "HitGong.h"
#include "DriveMotor.h"
#include "RangeSensors.h"

#define servoPosition 180
#define swingDelay 500


HitGong::HitGong(const byte sp) {
  servoPin = sp;
  
  servo.attach(servoPin);
}

void HitGong::swingMallet() {
  servo.write(servoPosition);
  delay(swingDelay);
  servo.write(0);
  delay(swingDelay); 
}



