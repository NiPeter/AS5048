#include "EncoderSensor.h"

EncoderSensor::EncoderSensor() : _filter(8, 0), _sensor(EncoderAddress) {}
//

void EncoderSensor::begin() { _sensor.begin(); }

Direction EncoderSensor::getDirection() { return _direction; }

int16_t EncoderSensor::getRotations() { return _rotations; }

double EncoderSensor::getAbsoluteAngle() {
  return 360.0 * getRotations() + getAngle();
}

float EncoderSensor::getAngle() {
  //  return _angle;
  return convertRawToDeg(_angle);
}

void EncoderSensor::task() { this->updateZeroCrossing(); }

/// PRIVATE FUNCTIONS

void EncoderSensor::updateZeroCrossing() {
  // update angles
  static uint16_t angle = 0;
  static uint16_t _lastAngle = 0;

  _lastAngle = angle;
  angle = _sensor.readReg16(AS5048B_ANGLMSB_REG);

  _filter.Filter(angle);
  _angle = (uint16_t)_filter.Current();

  if (_lastAngle == angle) {
    _direction = NONE;
    return;
  }

  bool isLastAngleGreater = _lastAngle > angle;
  if (isLastAngleGreater == false) {
    // normally CCW
    if (isNearMin(_lastAngle) && isNearMax(angle)) {
      // zero crossing CW
      _rotations--;
      _direction = CW;
      return;
    }
    _direction = CCW;

  } else {
    // normally CW
    if (isNearMax(_lastAngle) && isNearMin(angle)) {
      // zero crossing CCW
      _rotations++;
      _direction = CCW;
      return;
    }
    _direction = CW;
  }
}

double EncoderSensor::convertRawToDeg(int16_t rawAngle) {
  return ((rawAngle * 360.0) / AS5048B_RESOLUTION);
}
bool EncoderSensor::isNearMax(uint16_t angle) {
  return angle > (AS5048B_RESOLUTION - (_nearAngle / 2));
}
bool EncoderSensor::isNearMin(uint16_t angle) {
  return angle < (_nearAngle / 2);
}
