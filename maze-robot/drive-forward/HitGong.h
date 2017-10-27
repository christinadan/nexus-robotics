#ifndef _HIT_GONG_H_
#define _HIT_GONG_H_

#define DEBUG_SERIAL_PORT (Serial3)

#include "Arduino.h"
#include <Servo.h>

class HitGong {
  private:
    byte servoPin;
    Servo servo;
    
  public:
   HitGong(const byte sp);
   void findAndHitGong();
};

#endif
