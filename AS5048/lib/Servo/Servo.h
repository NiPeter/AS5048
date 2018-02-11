#ifndef SERVO_H
#define SERVO_H
#include <Arduino.h>

#include <AS5048B.h>
#include <L298N.h>

#include <PID_v1.h>

#define EN 3
#define IN1 4
#define IN2 5

class Servo {
private:
  double Kp = 0.06, Ki = 0.9, Kd = 0.001;
  double motorPower = 0;
  double motorSpeed = 0;
  double speedTarget = 0;
  bool startup = true;

public:
  Servo()
      : Encoder(AS5048B_DEFAULT_ADDRESS, 10, 50), Motor(EN, IN1, IN2),
        Pid(&this->motorSpeed, &this->motorPower, &this->speedTarget, Kp, Ki,
            Kd, DIRECT){

        };
  AS5048B Encoder;
  L298N Motor;
  PID Pid;

  void SetSpeed(short speed) { this->speedTarget = speed; };
  void SetPower(short power) { this->Motor.setSpeed(power); };

  short GetSpeed() const { return this->motorSpeed; };
  short GetMotorPower() const { return this->motorPower; };
  short GetSpeedTarget() const { return this->speedTarget; };

  double Angle() { return AS5048B::convertRawToDeg(Encoder.getAngle()); };
  void PidOn(bool state) {
    if (state) {
      Pid.SetMode(AUTOMATIC);
    } else {
      Pid.SetMode(MANUAL);
    }
  }

  void Begin() {
    Wire.begin();
    Wire.setClock(3400000L);

    // Initialise Encoder
    Encoder.begin();
    Encoder.reverseDirection();

    // Initialise Pid
    Pid.SetOutputLimits(-255, 255);
    Pid.SetSampleTime(8);
    motorPower = 0;
    motorSpeed = 0;
    speedTarget = 0;
    Pid.SetMode(AUTOMATIC);
  }

  void Task() {
    Encoder.task();
    if (startup) {
      if (millis() > 500)
        startup = false;
    } else {
      this->motorSpeed = Encoder.getSpeed();
    }

    if (Pid.Compute()) {
      if ((this->speedTarget == 0) && (this->motorSpeed == 0)) {
        Motor.setSpeed(0);
      } else {
        Motor.setSpeed(this->motorPower);
      }
    }
  }
};

#endif /* end of include guard:SERVO_H  */
