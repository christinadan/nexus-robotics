#include "HitGong.h"
#include "DriveMotor.h"
#include "RangeSensors.h"


HitGong::HitGong(const byte sp) {
  servoPin = sp;
  
  servo.attach(servoPin);
}

void HitGong::findAndHitGong() {

}



