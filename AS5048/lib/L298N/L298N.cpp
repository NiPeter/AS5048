/*
  L298N.cpp - Libreria per gestire i movimenti di un motore DC con il modulo
  L298N
  Autore:   Andrea Lombardo
  Sito web: http://www.lombardoandrea.com
*/

#include "L298N.h"

L298N::L298N(uint8_t pinEnable, uint8_t pinIN1, uint8_t pinIN2) {
  _pinEnable = pinEnable;
  _pinIN1 = pinIN1;
  _pinIN2 = pinIN2;
  _pwmVal = 0;
  _destVal = _pwmVal;

  pinMode(_pinEnable, OUTPUT);
  pinMode(_pinIN1, OUTPUT);
  pinMode(_pinIN2, OUTPUT);
}

short L298N::trim(short value) {
  if (value > 255) {
    return value = 255;
  }
  if (value < -255) {
    return value = -255;
  }

  return value;
}

void L298N::accToSpeed(short destVal, unsigned short ramp) {
  if (destVal == _destVal)
    return;

  short pwmVal = _pwmVal * ((_dir == FORWARD) ? 1 : -1);
  _destVal = L298N::trim(destVal);

  _a = 25500 / ramp;

  if (_a == 0) {
    _a = 1;
  }

  if (_destVal > pwmVal) {
    _accState = ACC;
  } else if (_destVal < pwmVal) {
    _accState = DCC;
  } else {
    _accState = NONE;
  }
}

void L298N::task(uint8_t period_ms) {
  static short pwmVal = 0;

  short deltaVal = (period_ms * _a);

  switch (_accState) {
  case NONE:
    return;
    break;

  case ACC:
    pwmVal += deltaVal;
    if (pwmVal >= _destVal * 100) {
      pwmVal = _destVal * 100;
      _accState = NONE;
    }

    break;
  case DCC:
    pwmVal -= deltaVal;
    if (pwmVal <= _destVal * 100) {
      pwmVal = _destVal * 100;
      _accState = NONE;
    }
    break;
  }
  setSpeed(pwmVal / 100);
}

void L298N::setSpeed(short pwmVal) {
  pwmVal = trim(pwmVal);

  if (pwmVal < 0) {
    _pwmVal = -pwmVal;
    backward();
  } else {
    _pwmVal = pwmVal;
    forward();
  }
  analogWrite(_pinEnable, _pwmVal);
}

short L298N::getSpeed() { return _pwmVal * ((_dir == FORWARD) ? 1 : -1); }

void L298N::forward() {
  _dir = FORWARD;
  digitalWrite(_pinIN1, HIGH);
  digitalWrite(_pinIN2, LOW);
}

void L298N::backward() {
  _dir = BACKWARD;
  digitalWrite(_pinIN1, LOW);
  digitalWrite(_pinIN2, HIGH);
}

void L298N::stop() {
  digitalWrite(_pinIN1, LOW);
  digitalWrite(_pinIN2, LOW);

  analogWrite(_pinEnable, 255);
}
