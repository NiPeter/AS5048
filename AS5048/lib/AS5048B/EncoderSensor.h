
#ifndef _ENCODERSENSOR_H_
#define _ENCODERSENSOR_H_
#include <Arduino.h>

typedef enum { NONE, CW, CCW } Direction;

class EncoderSensor {
public:
  virtual void begin() = 0;
  virtual void task() = 0;

  virtual long getAngle() = 0;
  virtual short getRotations() = 0;

  // Direction getDirection() { return direction; };

protected:
  Direction direction = NONE;
};

#endif
