#ifndef L298N_h
#define L298N_h

#include "Arduino.h"

class L298N {
public:
  typedef enum { FORWARD = 0, BACKWARD = 1 } Direction;
  typedef enum { NONE = 0, ACC, DCC } AccelerationEnum;

  L298N(uint8_t pinEnable, uint8_t pinIN1, uint8_t pinIN2);
  void setSpeed(short pwmVal);
  void accToSpeed(short pwmVal, unsigned short ramp = 100);
  short getSpeed();
  void forward();
  void backward();
  void run(uint8_t direction);
  void stop();
  void task(uint8_t period_ms);

private:
  byte _pinEnable;
  byte _pinIN1;
  byte _pinIN2;

  Direction _dir;
  short _pwmVal;
  short _destVal;
  byte _a;
  AccelerationEnum _accState;

  static short trim(short value);
};

#endif
