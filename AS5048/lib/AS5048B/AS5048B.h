#ifndef _AS5048B_H_
#define _AS5048B_H_

#include <AS5048B_Chip.h>
#include <Arduino.h>
#include <Debug.h>
#include <EncoderSensor.h>
#include <Filter.h>
#include <Wire.h>

#define AS5048B_MAX_ANGLE_DIFF (AS5048B_RESOLUTION / 3)
#define AS5048B_RAW2DEG_FACTOR (360.0 / AS5048B_RESOLUTION)

class AS5048B : private EncoderSensor, public AS5048B_Chip {
public:
  AS5048B(unsigned char i2c_address = AS5048B_DEFAULT_ADDRESS,
          unsigned char angleFilter_weight = 12,
          unsigned char speedFilter_weight = 12);

  void begin(void);
  void task(void);
  void reset(void);

  void reverseDirection() { reverse = (reverse == 1) ? -1 : 1; }

  long getAngle() { return reverse * angleFilter.Current(); };
  long getAngleDeg() { return convertRawToDeg(getAngle()); };
  short getRotations() { return this->getAngle() / AS5048B_RESOLUTION; };

  // Get speed in rpm
  short getSpeed() {
    return (speedFilter.Current() * 60.0) / AS5048B_RESOLUTION;
  };

  static double convertRawToDeg(long rawAngle);

private:
  unsigned long prevTime;
  unsigned short lastAngle = 0;
  long rotations = 0;
  char reverse = 1;

  ExponentialFilter<unsigned char, long> angleFilter;
  ExponentialFilter<unsigned char, long> speedFilter;

  void computeSpeed();
};

#endif /* end of include guard _AMS_AS5048B_H_ */
