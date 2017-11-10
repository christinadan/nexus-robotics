#ifndef _RANGE_SENSORS_H_
#define _RANGE_SENSORS_H_

#include <Wire.h>
#include <VL53L0X.h>

#define RANGE_SENSOR_TIMEOUT 500

class RangeSensors {
  private:
    byte frontShutdownPin;
    byte leftFrontShutdownPin;
    byte leftBackShutdownPin;
    byte rightFrontShutdownPin;
    byte rightBackShutdownPin;
    byte topShutdownPin;
    byte frontShutdownAddress;
    byte leftFrontShutdownAddress;
    byte leftBackShutdownAddress;
    byte rightFrontShutdownAddress;
    byte rightBackShutdownAddress;
    byte topShutdownAddress;
    VL53L0X frontSensor;
    VL53L0X leftFrontSensor;
    VL53L0X leftBackSensor;
    VL53L0X rightFrontSensor;
    VL53L0X rightBackSensor;
    VL53L0X topSensor;

  public:
    RangeSensors(const byte fsp, const byte lfsp, const byte lbsp, const byte rfsp, const byte rbsp, const byte tsp, const byte fsa, const byte lfsa, const byte lbsa, const byte rfsa, const byte rbsa, const byte tsa);
    int getTopDistance(void);
    int getFrontDistance(void);
    int getLeftFrontDistance(void);
    int getLeftBackDistance(void);
    int getRightFrontDistance(void);
    int getRightBackDistance(void);
    void start(void);
    void init(void);
    void turnOff(void);
    void reset(void);
};

#endif

