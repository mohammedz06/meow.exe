#include "motors.h"
#include "config.h"
#include <AccelStepper.h>

static AccelStepper motor1(AccelStepper::DRIVER, STEP1, DIR1);
static AccelStepper motor2(AccelStepper::DRIVER, STEP2, DIR2);

void motorsInit() {
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, LOW);

  motor1.setMaxSpeed(MAX_SPEED);
  motor1.setAcceleration(MAX_ACCEL);
  motor2.setMaxSpeed(MAX_SPEED);
  motor2.setAcceleration(MAX_ACCEL);
}

// Positive speed = forward, negative = backward
// Motor 2 is physically flipped so its direction is inverted
void motorsSetSpeed(float speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(-speed);
}

void motorsRun() {
  motor1.runSpeed();
  motor2.runSpeed();
}

void motorsStop() {
  motor1.setSpeed(0);
  motor2.setSpeed(0);
}
