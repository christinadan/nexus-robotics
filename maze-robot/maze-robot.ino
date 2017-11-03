#include <Wire.h>
#include <VL53L0X.h>
#include <Servo.h>

#include "RangeSensors.h"
#include "DriveMotor.h"
#include "HitGong.h"

#define FRONT_SHUTDOWN_PIN 42
#define LEFT_SHUTDOWN_PIN 41
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
#define sideDistTolerance 250

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


  Serial3.println(sensors.getLeftDistance());
  Serial3.println(sensors.getFrontDistance());
  delay(1000);
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
         statusCode = 200;
      } else if (leftDist > sideDistTolerance) {
        Serial3.println("left distance exceeding tolerance!!");
        Serial3.println(leftDist);
        statusCode = 200;
      } else if (rightDist > sideDistTolerance) {
        Serial3.println("right distance exceeding tolerance!!");
        Serial3.println(rightDist);
        statusCode = 200;
      } else {
        lastError = myMotor.driveForward(leftDist, lastError);
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
      
      if (sensors.getLeftDistance() > sideDistTolerance) {
        myMotor.adjustAfterLeftTurn();
        myMotor.stopMotors();
      }

//      myMotor.adjustWideLeftTurn(leftDist);
//      myMotor.stopMotors();
      delay(1000);
      lastError = 0;
      statusCode = 100;
      break;
    case 104:
      Serial3.println(statusCode);
      myMotor.turnRight();
      myMotor.stopMotors();
      delay(2000);
      lastError = myMotor.driveForward(leftDist, lastError);
      statusCode = 100;
      break;
    case 105:
      Serial3.println(statusCode);
      myMotor.turnAround();
      myMotor.stopMotors();
      delay(2000);
      lastError = myMotor.driveForward(leftDist, lastError);
      statusCode = 100;
      break;
    case 106:
      break;
    case 200:
      myMotor.stopMotors();
      if (leftDist > sideDistTolerance) {
        Serial3.println("turning left :D");
        Serial3.println(leftDist);
        // We can turn left.
        statusCode = 103; 
      } else if (rightDist > sideDistTolerance) {
        // We can turn right.
        statusCode = 104;
      } else if (frontDist < frontDistTolerance) {
        // We are stuck, turn around.
//        statusCode = 105;
      }
      break;
    case 205:
      myMotor.driveForward();
      delay(4000);
      myMotor.stopMotors();
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
