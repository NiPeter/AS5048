
#include <Arduino.h>
#include <ClickEncoder.h>
#include <Debug.h>
#include <Filter.h>
#include <Servo.h>
#include <Timer.h>

#define POT A0
#define EL A1
#define ER A2
#define EB A3

bool disp = true;

Timer timer;
Servo servo;
ClickEncoder *encoder;
bool speedOrpower = true;

short speedTarget;
bool PonM = false;

void processSerialInputs();
void help();

void setup() {

  // Initialise Serial and I2C
  Serial.begin(115200L);
  servo.Begin();

  encoder = new ClickEncoder(EL, ER, EB);

  // Initialise servo
  servo.Pid.SetTunings(servo.Pid.GetKp(), servo.Pid.GetKi(), servo.Pid.GetKd(),
                       (PonM) ? (P_ON_M) : (P_ON_E));
  servo.PidOn(false);

  timer.every(1, []() {
    encoder->service();

  });

  // Timer
  timer.every(10, []() {
    ClickEncoder::Button b = encoder->getButton();
    if (b != ClickEncoder::Open) {
      switch (b) {
      case ClickEncoder::Clicked:
        speedOrpower = !speedOrpower;
        speedTarget = 0;
        break;
      }
    }

    if (disp) {
      Serial.println((String)speedTarget + ";" + (String)servo.GetSpeed() +
                     ";");
    }
  });

  timer.every(100, []() {
    if (speedOrpower) {
      speedTarget += 10 * encoder->getValue();
      servo.PidOn(true);
      servo.SetSpeed(speedTarget);
    } else {
      speedTarget += 10 * encoder->getValue();
      servo.PidOn(false);
      servo.SetPower(speedTarget);
    }

  });
}

void loop() {
  if (Serial.available())
    processSerialInputs();

  servo.Task();
  timer.update();
  // Serial.println((String)servo.GetSpeed());
}

void processSerialInputs() {
  char cmd = Serial.read();
  if (cmd > 'Z')
    cmd -= 32;
  switch (cmd) {
  case 'P':
    servo.Pid.SetTunings(Serial.parseFloat(), servo.Pid.GetKi(),
                         servo.Pid.GetKd());
    break;
  case 'I':
    servo.Pid.SetTunings(servo.Pid.GetKp(), Serial.parseFloat(),
                         servo.Pid.GetKd());
    break;
  case 'D':
    servo.Pid.SetTunings(servo.Pid.GetKp(), servo.Pid.GetKi(),
                         Serial.parseFloat());
    break;
  case 'O':
    PonM = !PonM;
    servo.Pid.SetTunings(servo.Pid.GetKp(), servo.Pid.GetKi(),
                         servo.Pid.GetKd(), (PonM) ? (P_ON_M) : (P_ON_E));
    break;
  case 'Q':
    DPRINTLN("Speed target =" + (String)speedTarget);
    DPRINTLN("Kp=" + (String)servo.Pid.GetKp() + " Ki=" +
             (String)servo.Pid.GetKi() + " Kd=" + (String)servo.Pid.GetKd());
    break;
  case 'S':
    servo.PidOn(false);
    speedOrpower = false;
    speedTarget = Serial.parseInt();
    servo.SetPower(speedTarget);
    break;

  case 'V':
    servo.PidOn(true);
    speedOrpower = true;
    speedTarget = Serial.parseInt();
    servo.SetSpeed(speedTarget);
    break;
  case '?':
    disp = !disp;
    break;
  case 'H':
    help();
    break;
  default:
    break;
  }
}

void help() {
  Serial.println(F("\nPID DC motor controller"));
  Serial.println(F("Available serial commands: (lines end with CRLF or LF)"));
  Serial.println(F("P123.34 sets proportional term to 123.34"));
  Serial.println(F("I123.34 sets integral term to 123.34"));
  Serial.println(F("D123.34 sets derivative term to 123.34"));
  Serial.println(F("R1234 sets dc motor ramp to 1234ms"));
  Serial.println(F("S123.34 sets setpoint to 1234.56"));
  Serial.println(F("? prints out current speed, output and setpoint values"));
  Serial.println(
      F("Q will print out the current values of P, I and D parameters"));
  Serial.println(F("H will print this help message again"));
}
