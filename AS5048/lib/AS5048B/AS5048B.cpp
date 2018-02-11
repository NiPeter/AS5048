#include <AS5048b.h>

AS5048B::AS5048B(unsigned char i2c_address, unsigned char angleFilter_weight,
                 unsigned char speedFilter_weight)
    : AS5048B_Chip(i2c_address), angleFilter(angleFilter_weight, 0),
      speedFilter(speedFilter_weight, 0) {}

void AS5048B::begin(void) {
  // get angle and update lastAngle
  lastAngle = readAngleRegister();
  // update filter with getted angle
  angleFilter.SetCurrent(lastAngle);
  // update prevTime
  prevTime = micros();
}

void AS5048B::reset(void) {
  rotations = 0;
  // direction = NONE;
  begin();
}

void AS5048B::task() {
  // update current variables

  unsigned short currentAngle = this->readAngleRegister();

  // compute variable differences
  short deltaAngle = currentAngle - lastAngle;

  // chceck zero crossing
  if (deltaAngle < -AS5048B_MAX_ANGLE_DIFF) {
    // direction = CCW; // and zero crossing !
    rotations = rotations + AS5048B_RESOLUTION;

  } else if (deltaAngle > AS5048B_MAX_ANGLE_DIFF) {
    // direction = CW; // and zero crossing !
    rotations = rotations - AS5048B_RESOLUTION;
  };

  // update previous variables
  lastAngle = currentAngle;

  // compute and filter absolute angle
  long angle = rotations + currentAngle;
  angleFilter.Filter(angle);

  computeSpeed();
}

void AS5048B::computeSpeed() {
  static long prevSpeedAngle = 0;

  unsigned long currentTime = micros();
  unsigned short deltaTime = currentTime - prevTime;

  // compute speed if >=10ms passed
  // 7500
  if (deltaTime >= 7500) {
    long speedAngle = getAngle();
    long deltaSpeedAngle = speedAngle - prevSpeedAngle;

    deltaTime /= 1000;                            // to ms
    long speed = (deltaSpeedAngle) / (deltaTime); // raw/ms
    speed *= 1000;                                // raw/s
    speedFilter.Filter(speed);                    // speed in raw/s

    prevTime = currentTime;
    prevSpeedAngle = speedAngle;
  };
}

double AS5048B::convertRawToDeg(long rawAngle) {
  return (rawAngle * AS5048B_RAW2DEG_FACTOR);
}
