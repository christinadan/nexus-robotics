#include <Wire.h>
#include <VL53L0X.h>
#include <Servo.h>

#include "RangeSensors.h"
#include "DriveMotor.h"
#include "HitGong.h"

#define FRONT_SHUTDOWN_PIN 40
#define LEFT_SHUTDOWN_PIN 41
#define RIGHT_SHUTDOWN_PIN 42
#define TOP_SHUTDOWN_PIN 43

#define FRONT_SHUTDOWN_ADDRESS 0x30
#define LEFT_SHUTDOWN_ADDRESS 0x31
#define RIGHT_SHUTDOWN_ADDRESS 0x32
#define TOP_SHUTDOWN_ADDRESS 0x33

#define MOTOR_LEFT_CH0 2
#define MOTOR_LEFT_CH1 3
#define MOTOR_RIGHT_CH0  4
#define MOTOR_RIGHT_CH1  5

#define SERVO_PIN 9

DriveMotor myMotor(MOTOR_LEFT_CH0, MOTOR_LEFT_CH1,
                    MOTOR_RIGHT_CH0, MOTOR_RIGHT_CH1);

RangeSensors sensors(FRONT_SHUTDOWN_PIN, LEFT_SHUTDOWN_PIN, RIGHT_SHUTDOWN_PIN, TOP_SHUTDOWN_PIN, FRONT_SHUTDOWN_ADDRESS, LEFT_SHUTDOWN_ADDRESS, RIGHT_SHUTDOWN_ADDRESS, TOP_SHUTDOWN_ADDRESS);

HitGong hitGong(SERVO_PIN);

int lastError = 0;
int mazeEnd = false;

void setup() {
  // put your setup code here, to run once:

  Wire.begin();
  Serial.begin(9600);
  Serial3.begin(9600);
  
  sensors.reset();
  sensors.turnOff();
  sensors.init();
  sensors.start();
}

void loop() {
  int leftDist = sensors.getLeftDistance();
  int topDist = sensors.getTopDistance();
  int frontDist = sensors.getFrontDistance();

  Serial3.println("topDist");
  Serial3.println(topDist);

  switch(mazeEnd) {
    case false:
      if(topDist < 150 && topDist > 25) {
        myMotor.stopMotors();
        mazeEnd = true;
      } else {
        lastError = myMotor.driveForward(leftDist, lastError);    
      }
      break;
    case true:
      
      break;

    
  }

  

}
