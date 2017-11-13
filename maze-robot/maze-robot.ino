 #include <Wire.h>
#include <VL53L0X.h>
#include <Servo.h>

#include "RangeSensors.h"
#include "DriveMotor.h"
#include "HitGong.h"

#define FRONT_SHUTDOWN_PIN 42
#define LEFT_SHUTDOWN_PIN 43
#define RIGHT_SHUTDOWN_PIN 44
#define TOP_SHUTDOWN_PIN 47

#define FRONT_SHUTDOWN_ADDRESS 0x30
#define LEFT_SHUTDOWN_ADDRESS 0x31
#define RIGHT_SHUTDOWN_ADDRESS 0x34
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

RangeSensors sensors(FRONT_SHUTDOWN_PIN, LEFT_SHUTDOWN_PIN, RIGHT_SHUTDOWN_PIN, TOP_SHUTDOWN_PIN, FRONT_SHUTDOWN_ADDRESS, LEFT_SHUTDOWN_ADDRESS, RIGHT_SHUTDOWN_ADDRESS, TOP_SHUTDOWN_ADDRESS);

HitGong hitGong(SERVO_PIN);

// initialize variables
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

  // initialize serial ports
  Serial.begin(9600);
  Serial3.begin(9600);

  // initialize sensors
  sensors.reset();
  sensors.turnOff();
  sensors.init();
  sensors.start();

  // tell user that the robot is ready
  Serial3.println("system ready");
  delay(1000);
}

void loop() {
  //ping all sensors
  int leftDist = sensors.getLeftDistance();
  int topDist = sensors.getTopDistance();
  int frontDist = sensors.getFrontDistance();
  int rightDist = sensors.getRightDistance();

  //check for new command
  if (Serial3.available() > 0) {
    newByte = Serial3.read();
    Serial3.println(newByte);
  }

  //check for canopy
//  if (topDist < 150 && topDist > 25) {
//    
//  } else
  //read new command
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
    case 100:     //Waiting for start command                         
      break;
    case 101:     //Drive forward
      //check distances and set where the boxes are
      if (frontDist < frontDistTolerance) {
        Serial3.println("front distance exceeding tolerance!!");
        Serial3.println(frontDist);
        boxFront = true;
        statusCode = 200;
      } 
      if (leftDist > sideDistTolerance) {
        Serial3.println("left distance exceeding tolerance!!");
        Serial3.println(leftDist);
        boxLeft = false;
        statusCode = 200;
      } 
      if (rightDist > sideDistTolerance) {
        Serial3.println("right distance exceeding tolerance!!");
        Serial3.println(rightDist);
        boxRight = false;
        statusCode = 200;
      } 
      break;
    case 102:     //stop driving
      myMotor.stopMotors();
      break;
    case 103:     //turn left
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

      myMotor.adjustWideLeftTurn(leftDist);
      myMotor.stopMotors();
      delay(1000);
      lastError = 0;
      statusCode = 100;
      break;
    case 104:     //turn right
      Serial3.println(statusCode);
      myMotor.turnRight();
      myMotor.stopMotors();
      delay(2000);
      lastError = myMotor.driveForward(leftDist, lastError);
      statusCode = 100;
      break;
    case 105:     //turn around
      Serial3.println(statusCode);
      myMotor.turnAround();
      myMotor.stopMotors();
      delay(2000);
      lastError = myMotor.driveForward(leftDist, lastError);
      statusCode = 100;
      break;
    case 200:     //determine new direction
      myMotor.stopMotors();
      //check where the boxes are
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
    case 300:     //find gong
      if(frontDist < gongDistance) {
        lastError = myMotor.driveForward(leftDist, lastError);
      } else {
        statusCode = 301;
      }
      break;
    case 301:     //hit gong
      hitGong.swingMallet();
      break;
    case 400: // sensor diagnostics
      Serial3.println("starting sensor diagnostics");
      
      if (leftDist == -1){
        Serial3.println("leftFront sensor is unresponsive");
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
      if (rightDist == -1) {
        Serial3.println("rightFront sensor is unresponsive");
        isOkay = false;
      }
      if (isOkay) {
        Serial3.println("Everything is good");
      }

      Serial3.println("Diagnostics complete");
      statusCode = 100;
         
      break;
    case 500:     //kill
      myMotor.stopMotors();
      statusCode = 100;
      break;    
  }


}
