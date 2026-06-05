#include "obstacle.h"
#include "config.h"
#include <ESP32Servo.h>

static Servo headServo;

void obstacleInit() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  headServo.attach(SERVO_PIN);
  headServo.write(SERVO_CENTER);
  delay(400);
}

float obstacleReadCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);  // 30ms timeout ~= 5m max
  if (duration == 0) return 999.0;  // nothing detected, return large value

  return duration * 0.034 / 2.0;
}

// Moves the servo head, waits for it to settle, then reads distance
static float scanDirection(int servoAngle) {
  headServo.write(servoAngle);
  delay(SCAN_PAUSE_MS);
  return obstacleReadCm();
}

int obstacleAvoid() {
  // Scan left and right to find the clearer path
  float distLeft  = scanDirection(SERVO_LEFT);
  float distRight = scanDirection(SERVO_RIGHT);

  // Return head to center regardless of which way we turn
  headServo.write(SERVO_CENTER);
  delay(200);

  // Pick whichever side has more room
  // Caller uses this to apply a turning bias to the motor speeds
  if (distLeft > distRight) {
    return -1;  // turn left
  } else {
    return 1;   // turn right
  }
}
