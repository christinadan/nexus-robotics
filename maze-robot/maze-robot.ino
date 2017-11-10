#include <Wire.h>
#include <VL53L0X.h>
#include <Servo.h>

#include "RangeSensors.h"
#include "DriveMotor.h"
#include "HitGong.h"

#define FRONT_SHUTDOWN_PIN 42
#define LEFTFRONT_SHUTDOWN_PIN 43
#define LEFTBACK_SHUTDOWN_PIN 45
#define RIGHTFRONT_SHUTDOWN_PIN 40
#define RIGHTBACK_SHUTDOWN_PIN 44
#define TOP_SHUTDOWN_PIN 47

#define FRONT_SHUTDOWN_ADDRESS 0x30
#define LEFTFRONT_SHUTDOWN_ADDRESS 0x31
#define LEFTBACK_SHUTDOWN_ADDRESS 0x32
#define RIGHTFRONT_SHUTDOWN_ADDRESS 0x33
#define RIGHTBACK_SHUTDOWN_ADDRESS 0x34
#define TOP_SHUTDOWN_ADDRESS 0x35

#define MOTOR_LEFT_CH0 4
#define MOTOR_LEFT_CH1 5
#define MOTOR_RIGHT_CH0  2
#define MOTOR_RIGHT_CH1  3

#define SERVO_PIN 9

#define frontDistTolerance 200
#define sideDistTolerance 250
#define gongDistance 150

DriveMotor myMotor(MOTOR_LEFT_CH0, MOTOR_LEFT_CH1,
                    MOTOR_RIGHT_CH0, MOTOR_RIGHT_CH1);

RangeSensors sensors(FRONT_SHUTDOWN_PIN, LEFTFRONT_SHUTDOWN_PIN, LEFTBACK_SHUTDOWN_PIN, RIGHTFRONT_SHUTDOWN_PIN, RIGHTBACK_SHUTDOWN_PIN, TOP_SHUTDOWN_PIN, FRONT_SHUTDOWN_ADDRESS, LEFTFRONT_SHUTDOWN_ADDRESS, LEFTBACK_SHUTDOWN_ADDRESS, RIGHTFRONT_SHUTDOWN_ADDRESS, RIGHTBACK_SHUTDOWN_ADDRESS, TOP_SHUTDOWN_ADDRESS);

HitGong hitGong(SERVO_PIN);

int lastError = 0;
int mazeEnd = false;
int statusCode = 100;
byte newByte = 'c';
boolean isOkay = true;
boolean boxFront = false;
boolean boxLeft = true;
boolean boxRight = true;

void setup() {
  Wire.begin();
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial3.begin(9600);
  
  sensors.reset();
  sensors.turnOff();
  sensors.init();
  sensors.start();


  Serial3.println("system ready");
  delay(1000);
}

void loop() {
  int leftFrontDist = sensors.getLeftFrontDistance();
  int leftBackDist = sensors.getLeftBackDistance();
  int topDist = sensors.getTopDistance();
  int frontDist = sensors.getFrontDistance();
  int rightFrontDist = sensors.getRightFrontDistance();
  int rightBackDist = sensors.getRightBackDistance();

  if (Serial3.available() > 0) {
    newByte = Serial3.read();
    Serial3.println(newByte);
  }

//  if (topDist < 150 && topDist > 25) {
//    
//  } else 
  if (newByte != 'c') {
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
        Serial3.println("statuscode 400");
        statusCode = 400;
        newByte = 'c';
        break;
      case 'p':
        statusCode = 205;
        newByte = 'c';
        break;
    }
    newByte = 'c';
  } 

  switch(statusCode) {
    case 100:                         
      break;
    case 101:
      if (frontDist < frontDistTolerance) {
        Serial3.println("front distance exceeding tolerance!!");
        Serial3.println(frontDist);
        boxFront = true;
        statusCode = 200;
      } 
      if (leftFrontDist > sideDistTolerance || leftBackDist > sideDistTolerance) {
        Serial3.println("left distance exceeding tolerance!!");
        Serial3.println(leftFrontDist);
        boxLeft = false;
        statusCode = 200;
      } 
      if (rightFrontDist > sideDistTolerance || rightBackDist > sideDistTolerance) {
        Serial3.println("right distance exceeding tolerance!!");
        Serial3.println(rightFrontDist);
        boxRight = false;
        statusCode = 200;
      } 
      if (!boxFront && !boxLeft && !boxRight) {
        lastError = myMotor.driveForward(leftFrontDist, lastError);
      }
      break;
    case 102:
      myMotor.stopMotors();
      break;
    case 103:
      Serial3.print("Last error ");
      Serial3.print(lastError);
      Serial3.println();
      myMotor.turnPositioning(500);
      myMotor.stopMotors();
      myMotor.turnLeft();
      myMotor.stopMotors();
      myMotor.adjustAfterLeftTurn();
      myMotor.stopMotors();
      
      if (sensors.getLeftFrontDistance() > sideDistTolerance) {
        myMotor.adjustAfterLeftTurn();
        myMotor.stopMotors();
      }

      myMotor.adjustWideLeftTurn(leftFrontDist);
      myMotor.stopMotors();
      delay(1000);
      lastError = 0;
      statusCode = 100;
      break;
    case 104:
      Serial3.println(statusCode);
      myMotor.turnRight();
      myMotor.stopMotors();
      delay(2000);
      lastError = myMotor.driveForward(leftFrontDist, lastError);
      statusCode = 100;
      break;
    case 105:
      Serial3.println(statusCode);
      myMotor.turnAround();
      myMotor.stopMotors();
      delay(2000);
      lastError = myMotor.driveForward(leftFrontDist, lastError);
      statusCode = 100;
      break;
    case 200:
      myMotor.stopMotors();
      if (boxFront && boxLeft && boxRight) {
        Serial3.println("I'm stuck, turning around");
        statusCode = 105;     
      } else if (boxFront && boxLeft) {
        Serial3.println("My only option is to turn right");
        statusCode = 104;
      } else if (boxFront && boxRight) {
        Serial3.println("My only option is to turn left");
        statusCode = 103;
      } else if (boxLeft && boxRight) {
        Serial3.println("My only option is forward, continuing forward");
        statusCode = 101;
      } else if (boxFront) {
        Serial3.println("Following left wall, turning left");
        statusCode = 103;
      } else if (boxLeft) {
        Serial3.println("Following left wall, going forward");
        statusCode = 101;
      } else if (boxRight) {
        Serial3.println("Following left wall, turning left");
        statusCode = 103;
      } else {
        Serial3.println("Following left wall, turning left");
        statusCode = 103;
      }
      boxFront = false;
      boxLeft = true;
      boxRight = true;
      break;
    case 300:
      if(frontDist < gongDistance) {
        lastError = myMotor.driveForward(leftFrontDist, lastError);
      } else {
        statusCode = 301;
      }
      break;
    case 301:
      hitGong.swingMallet();
      break;
    case 400: // sensor diagnostics
      Serial3.println("starting sensor diagnostics");
      
      if (leftFrontDist == -1){
        Serial3.println("leftFront sensor is unresponsive");
        isOkay = false;
      }
      if (leftBackDist == -1){
        Serial3.println("leftBack sensor is unresponsive");
        isOkay = false;
      }
//      if (topDist){
//        Serial3.println("top sensor is unresponsive");
//        isOkay = false;
//      }
      if (frontDist == -1) {
        Serial3.println("front sensor is unresponsive");
        isOkay = false;
      }
      if (rightFrontDist == -1) {
        Serial3.println("rightFront sensor is unresponsive");
        isOkay = false;
      }
      if (rightBackDist == -1) {
        Serial3.println("rightBack sensor is unresponsive");
        isOkay = false;
      }
      if (isOkay) {
        Serial3.println("Everything is good");
      }

      Serial3.println("Diagnostics complete");
      statusCode = 100;
         
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
