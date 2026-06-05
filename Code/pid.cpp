#include "pid.h"
#include "config.h"

static float integral  = 0;
static float lastError = 0;

void pidReset() {
  integral  = 0;
  lastError = 0;
}

float pidCompute(float error, float dt) {
  integral += error * dt;
  integral  = constrain(integral, -50, 50);  // clamp to prevent windup

  float derivative = (error - lastError) / dt;
  lastError = error;

  return (Kp * error) + (Ki * integral) + (Kd * derivative);
}
