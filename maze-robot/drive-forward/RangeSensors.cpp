#include "RangeSensors.h"

RangeSensors::RangeSensors(const byte fsp, const byte lsp, const byte rsp, const byte tsp, const byte fsa, const byte lsa, const byte rsa, const byte tsa) {
  frontShutdownPin = fsp;
  leftShutdownPin = lsp;
  rightShutdownPin = rsp;
  topShutdownPin = tsp;
  frontShutdownAddress = fsa;
  leftShutdownAddress = lsa;
  rightShutdownAddress = rsa;
  topShutdownAddress = tsa;
  
  pinMode(frontShutdownPin, OUTPUT);
  pinMode(leftShutdownPin, OUTPUT);
  pinMode(rightShutdownPin, OUTPUT);
  pinMode(topShutdownPin, OUTPUT);
}

void RangeSensors::reset() {
  delay(10);
  digitalWrite(frontShutdownPin, LOW);
  digitalWrite(leftShutdownPin, LOW);
  digitalWrite(rightShutdownPin, LOW);
  digitalWrite(topShutdownPin, LOW);
  delay(10);
  digitalWrite(frontShutdownPin, HIGH);
  digitalWrite(leftShutdownPin, HIGH);
  digitalWrite(rightShutdownPin, HIGH);
  digitalWrite(topShutdownPin, HIGH);
}

void RangeSensors::turnOff() {
  delay(10);
  digitalWrite(frontShutdownPin, LOW);
  digitalWrite(leftShutdownPin, LOW);
  digitalWrite(rightShutdownPin, LOW);
  digitalWrite(topShutdownPin, LOW);
}

void RangeSensors::init() {
  delay(100);
  digitalWrite(frontShutdownPin, HIGH);
  delay(100);
  Serial.println("Init front.");
  frontSensor.init(true);
  frontSensor.setAddress(0x30);

  digitalWrite(leftShutdownPin, HIGH);
  delay(100);
  Serial.println("Init left.");
  leftSensor.init(true);
  leftSensor.setAddress(0x31);

  digitalWrite(rightShutdownPin, HIGH);
  delay(100);
  Serial.println("Init right.");
  rightSensor.init(true);
  rightSensor.setAddress(0x32);

  digitalWrite(topShutdownPin, HIGH);
  delay(100);
  Serial.println(topShutdownPin);
  Serial.println(topShutdownAddress);
  topSensor.init(true);
  topSensor.setAddress(0x33);

  frontSensor.setTimeout(RANGE_SENSOR_TIMEOUT);   
  leftSensor.setTimeout(RANGE_SENSOR_TIMEOUT);
  rightSensor.setTimeout(RANGE_SENSOR_TIMEOUT);   
  topSensor.setTimeout(RANGE_SENSOR_TIMEOUT);
}

void RangeSensors::start() {
  frontSensor.startContinuous();
  leftSensor.startContinuous();
  rightSensor.startContinuous();
  topSensor.startContinuous();
}

int RangeSensors::getTopDistance() {
  return topSensor.readRangeContinuousMillimeters();
}

int RangeSensors::getLeftDistance() {
  return leftSensor.readRangeContinuousMillimeters();
}

int RangeSensors::getRightDistance() {
  return rightSensor.readRangeContinuousMillimeters();
}

int RangeSensors::getFrontDistance() {
  return frontSensor.readRangeContinuousMillimeters();
}

