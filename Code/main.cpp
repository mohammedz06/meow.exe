#include <Arduino.h>
#include "config.h"
#include "imu.h"
#include "pid.h"
#include "motors.h"
#include "obstacle.h"

// Robot state machine
enum State { BALANCING, REVERSING, SCANNING, TURNING };
static State state = BALANCING;

static unsigned long lastIMUTime   = 0;
static unsigned long stateTimer    = 0;
static int           turnDirection = 1;   // -1 = left, 1 = right
static float         turnBias      = 0;   // applied to motor speeds during turning

void setup() {
  Serial.begin(115200);

  if (!imuInit()) {
    Serial.println("MPU6050 not found — check wiring");
    while (1) delay(10);
  }

  motorsInit();
  obstacleInit();

  Serial.println("Meow.exe online.");
}

void loop() {
  unsigned long now = millis();

  // PID runs at 100Hz regardless of state — robot must always balance
  if (now - lastIMUTime >= 10) {
    float dt    = (now - lastIMUTime) / 1000.0;
    lastIMUTime = now;

    float pitch = imuGetPitch();
    float error = pitch - SETPOINT;

    if (abs(error) < DEAD_ZONE) {
      motorsStop();
      pidReset();
    } else {
      float pidOutput = pidCompute(error, dt);

      // Apply forward bias or turn bias on top of the balance output
      // FORWARD_SPEED_BIAS nudges the robot to walk forward when balanced
      // turnBias offsets left/right motor speeds to arc in a direction
      float leftSpeed  = constrain(pidOutput + FORWARD_SPEED_BIAS + turnBias, -MAX_SPEED, MAX_SPEED);
      float rightSpeed = constrain(pidOutput + FORWARD_SPEED_BIAS - turnBias, -MAX_SPEED, MAX_SPEED);

      // motorsSetSpeed drives both at the same rate; for turning we set them individually
      // Positive pidOutput = lean forward = drive forward to catch itself
      motorsSetSpeed((leftSpeed + rightSpeed) / 2.0);
    }

    Serial.print("State: "); Serial.print(state);
    Serial.print("  Pitch: "); Serial.print(pitch, 1);
    Serial.print("  Error: "); Serial.println(error, 1);
  }

  motorsRun();

  // State machine for obstacle avoidance
  // This runs independently of the PID — balancing continues during avoidance
  switch (state) {

    case BALANCING: {
      turnBias = 0;
      float dist = obstacleReadCm();

      if (dist < OBSTACLE_DISTANCE_CM) {
        Serial.print("Obstacle at ");
        Serial.print(dist);
        Serial.println("cm — reversing");
        state      = REVERSING;
        stateTimer = now;
      }
      break;
    }

    case REVERSING: {
      // Override forward bias to push the robot backward
      // The PID still balances, we just flip the walk direction
      turnBias = -FORWARD_SPEED_BIAS * 2;

      if (now - stateTimer >= REVERSE_DURATION_MS) {
        state      = SCANNING;
        stateTimer = now;
      }
      break;
    }

    case SCANNING: {
      // obstacleAvoid() moves the head servo and picks a side
      turnDirection = obstacleAvoid();
      Serial.print("Turning: ");
      Serial.println(turnDirection == -1 ? "left" : "right");
      state      = TURNING;
      stateTimer = now;
      break;
    }

    case TURNING: {
      // Apply a lateral bias to arc the robot in the chosen direction
      turnBias = turnDirection * FORWARD_SPEED_BIAS * 0.8;

      // Turn for a fixed duration then resume walking forward
      if (now - stateTimer >= 800) {
        state    = BALANCING;
        turnBias = 0;
      }
      break;
    }
  }
}
