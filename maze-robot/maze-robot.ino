#include <Wire.h>
#include <VL53L0X.h>
#include <Servo.h>

#include "RangeSensors.h"
#include "DriveMotor.h"
#include "HitGong.h"

#define FRONT_SHUTDOWN_PIN 41
#define LEFT_SHUTDOWN_PIN 42
#define RIGHT_SHUTDOWN_PIN 43
#define TOP_SHUTDOWN_PIN 44

#define FRONT_SHUTDOWN_ADDRESS 0x30
#define LEFT_SHUTDOWN_ADDRESS 0x31
#define RIGHT_SHUTDOWN_ADDRESS 0x32
#define TOP_SHUTDOWN_ADDRESS 0x33

#define MOTOR_LEFT_CH0 4
#define MOTOR_LEFT_CH1 5
#define MOTOR_RIGHT_CH0  2
#define MOTOR_RIGHT_CH1  3

#define SERVO_PIN 9

#define frontDistTolerance 200
#define sideDistTolerance 5000

DriveMotor myMotor(MOTOR_LEFT_CH0, MOTOR_LEFT_CH1,
                    MOTOR_RIGHT_CH0, MOTOR_RIGHT_CH1);

RangeSensors sensors(FRONT_SHUTDOWN_PIN, LEFT_SHUTDOWN_PIN, RIGHT_SHUTDOWN_PIN, TOP_SHUTDOWN_PIN, FRONT_SHUTDOWN_ADDRESS, LEFT_SHUTDOWN_ADDRESS, RIGHT_SHUTDOWN_ADDRESS, TOP_SHUTDOWN_ADDRESS);

HitGong hitGong(SERVO_PIN);

int lastError = 0;
int mazeEnd = false;
int statusCode = 100;
byte newByte = 'c';

void setup() {
  Wire.begin();
  // put your setup code here, to run once:

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
  int rightDist = sensors.getRightDistance();

  if (Serial3.available() > 0) {
    newByte = Serial3.read();
    Serial3.println(newByte);
  }

  if (topDist < 150 && topDist > 25) {
    
  } else if (newByte != 'c') {
    switch(newByte) {
      case 's':
        statusCode = 101;
        newByte = 'c';
        break;
      case 'k':
        statusCode = 500;
        newByte = 'c';
        break;
      case 'y':
        statusCode = 104;
        newByte = 'c';
    }
    newByte = 'c';
  } 

  switch(statusCode) {
    case 100:
      Serial3.println("ready to start");
      break;
    case 101:
      if (frontDist < frontDistTolerance || leftDist > sideDistTolerance || rightDist > sideDistTolerance) {
        Serial3.println("distance exceeding tolerance!!");
        Serial3.println(leftDist);
        Serial3.println(rightDist);
        Serial3.println(frontDist);
        statusCode = 200;
      } else {
        lastError = myMotor.driveForward(leftDist, lastError);
      }
      break;
    case 102:
      myMotor.stopMotors();
      break;
    case 103:
      myMotor.turnLeft();
      myMotor.stopMotors();
      statusCode = 100;
      break;
    case 104:
      myMotor.turnRight();
      myMotor.stopMotors();
      statusCode = 100;
      break;
    case 105:
      break;
    case 106:
      break;
    case 200:
      myMotor.stopMotors();
      break;
    case 300:
      break;
    case 301:
      break;
    case 500:
      myMotor.stopMotors();
      statusCode = 100;
      break;
      
  }
//  switch(mazeEnd) {
//    case false:
//      if(topDist < 150 && topDist > 25) {
//        myMotor.stopMotors();
//        mazeEnd = true;
//      } else {
//        lastError = myMotor.driveForward(leftDist, lastError);    
//      }
//      break;
//    case true:
//      
//      break;  
//  }
}
