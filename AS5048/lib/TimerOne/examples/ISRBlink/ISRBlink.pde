#include <Arduino.h>
#include <L298N.h>
#include <TimerOne.h>

const int en = 3;
const int in1 = 4;
const int in2 = 5;

// create a motor instance
L298N motor(en, in1, in2);
short speed = 255;

void motorTask() { motor.task(); }

void setup() {

	// used for display information
	Serial.begin(9600);

	Timer1.initialize(1000); // initialize timer1, and set a 1/2 second period
	Timer1.attachInterrupt(motorTask);
	delay(1000);
	motor.accToSpeed(speed, 1000);
}

char msg[40];
void loop() {

	if (Serial.available() == true) {
		Serial.readBytesUntil('\n', msg, 40);
	}
}
