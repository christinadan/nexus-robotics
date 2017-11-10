#include "RangeSensors.h"

RangeSensors::RangeSensors(const byte fsp, const byte lfsp, const byte lbsp, const byte rfsp, const byte rbsp, const byte tsp, const byte fsa, const byte lfsa, const byte lbsa, const byte rfsa, const byte rbsa, const byte tsa) {
  frontShutdownPin = fsp;
  leftFrontShutdownPin = lfsp;
  leftBackShutdownPin = lbsp;
  rightFrontShutdownPin = rfsp;
  rightBackShutdownPin = rbsp;
  topShutdownPin = tsp;
  frontShutdownAddress = fsa;
  leftFrontShutdownAddress = lfsa;
  leftBackShutdownAddress = lbsa;
  rightFrontShutdownAddress = rfsa;
  rightBackShutdownAddress = rbsa;
  topShutdownAddress = tsa;
  
  pinMode(frontShutdownPin, OUTPUT);
  pinMode(leftFrontShutdownPin, OUTPUT);
  pinMode(leftBackShutdownPin, OUTPUT);
  pinMode(rightFrontShutdownPin, OUTPUT);
  pinMode(rightBackShutdownPin, OUTPUT);
  pinMode(topShutdownPin, OUTPUT);
}

void RangeSensors::reset() {
  delay(10);
  digitalWrite(frontShutdownPin, LOW);
  digitalWrite(leftFrontShutdownPin, LOW);
  digitalWrite(leftBackShutdownPin, LOW);
  digitalWrite(rightFrontShutdownPin, LOW);
  digitalWrite(rightBackShutdownPin, LOW);
  digitalWrite(topShutdownPin, LOW);
  delay(10);
  digitalWrite(frontShutdownPin, HIGH);
  digitalWrite(leftFrontShutdownPin, HIGH);
  digitalWrite(leftBackShutdownPin, HIGH);
  digitalWrite(rightFrontShutdownPin, HIGH);
  digitalWrite(rightBackShutdownPin, HIGH);
  digitalWrite(topShutdownPin, HIGH);
}

void RangeSensors::turnOff() {
  delay(10);
  digitalWrite(frontShutdownPin, LOW);
  digitalWrite(leftFrontShutdownPin, LOW);
  digitalWrite(leftBackShutdownPin, LOW);
  digitalWrite(rightFrontShutdownPin, LOW);
  digitalWrite(rightBackShutdownPin, LOW);
  digitalWrite(topShutdownPin, LOW);
}

void RangeSensors::init() {
  delay(100);
  digitalWrite(frontShutdownPin, HIGH);
  delay(100);
  Serial3.println("Init front.");
  frontSensor.init(true);
  frontSensor.setAddress(0x30);

  digitalWrite(leftFrontShutdownPin, HIGH);
  delay(100);
  Serial3.println("Init left.");
  leftFrontSensor.init(true);
  leftFrontSensor.setAddress(0x31);

  digitalWrite(leftBackShutdownPin, HIGH);
  delay(100);
  Serial3.println("Init left.");
  leftBackSensor.init(true);
  leftBackSensor.setAddress(0x32);

  digitalWrite(rightFrontShutdownPin, HIGH);
  delay(100);
  Serial3.println("Init right.");
  rightFrontSensor.init(true);
  rightFrontSensor.setAddress(0x33);

  digitalWrite(rightBackShutdownPin, HIGH);
  delay(100);
  Serial3.println("Init right.");
  rightBackSensor.init(true);
  rightBackSensor.setAddress(0x34);

  digitalWrite(topShutdownPin, HIGH);
  delay(100);
  Serial3.println(topShutdownPin);
  Serial3.println(topShutdownAddress);
  topSensor.init(true);
  topSensor.setAddress(0x35);

  frontSensor.setTimeout(RANGE_SENSOR_TIMEOUT);   
  leftFrontSensor.setTimeout(RANGE_SENSOR_TIMEOUT);
  leftBackSensor.setTimeout(RANGE_SENSOR_TIMEOUT);
  rightFrontSensor.setTimeout(RANGE_SENSOR_TIMEOUT);
  rightBackSensor.setTimeout(RANGE_SENSOR_TIMEOUT);   
  topSensor.setTimeout(RANGE_SENSOR_TIMEOUT);
}

void RangeSensors::start() {
  frontSensor.startContinuous();
  leftFrontSensor.startContinuous();
  leftBackSensor.startContinuous();
  rightFrontSensor.startContinuous();
  rightBackSensor.startContinuous();
  topSensor.startContinuous();
}

int RangeSensors::getTopDistance() {
  return topSensor.readRangeContinuousMillimeters();
}

int RangeSensors::getLeftFrontDistance() {
  return leftFrontSensor.readRangeContinuousMillimeters();
}

int RangeSensors::getLeftBackDistance() {
  return leftBackSensor.readRangeContinuousMillimeters();
}

int RangeSensors::getRightFrontDistance() {
  return rightFrontSensor.readRangeContinuousMillimeters();
}

int RangeSensors::getRightBackDistance() {
  return rightBackSensor.readRangeContinuousMillimeters();
}

int RangeSensors::getFrontDistance() {
  return frontSensor.readRangeContinuousMillimeters();
}

