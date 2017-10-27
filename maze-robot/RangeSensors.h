#ifndef _RANGE_SENSORS_H_
#define _RANGE_SENSORS_H_

#include <Wire.h>
#include <VL53L0X.h>

#define RANGE_SENSOR_TIMEOUT 500

class RangeSensors {
  private:
    byte frontShutdownPin;
    byte leftShutdownPin;
    byte rightShutdownPin;
    byte topShutdownPin;
    byte frontShutdownAddress;
    byte leftShutdownAddress;
    byte rightShutdownAddress;
    byte topShutdownAddress;
    VL53L0X frontSensor;
    VL53L0X leftSensor;
    VL53L0X rightSensor;
    VL53L0X topSensor;

  public:
    RangeSensors(const byte fsp, const byte lsp, const byte rsp, const byte tsp, const byte fsa, const byte lsa, const byte rsa, const byte tsa);
    int getTopDistance(void);
    int getFrontDistance(void);
    int getLeftDistance(void);
    int getRightDistance(void);
    void start(void);
    void init(void);
    void turnOff(void);
    void reset(void);
};

#endif

