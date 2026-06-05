#pragma once

// Motor pins
const int DIR1  = 12;
const int STEP1 = 14;
const int DIR2  = 26;
const int STEP2 = 25;
const int EN1   = 13;
const int EN2   = 27;

// IMU I2C pins
const int SDA_PIN = 21;
const int SCL_PIN = 22;

// Servo
const int SERVO_PIN = 19;

// Ultrasonic sensor
const int TRIG_PIN = 4;
const int ECHO_PIN = 5;

// PID gains — tune these
float Kp = 30.0;
float Ki = 0.0;
float Kd = 2.0;

// Balance target angle in degrees (positive = leaning slightly forward)
const float SETPOINT  = 3.0;
const float DEAD_ZONE = 1.0;

// Motor speed limits
const float MAX_SPEED = 1000.0;  // steps/sec
const float MAX_ACCEL = 2000.0;  // steps/sec²

// Forward bias applied to PID output when moving forward
// This nudges the setpoint so the robot leans into motion
const float FORWARD_SPEED_BIAS = 150.0;

// Obstacle avoidance
const float OBSTACLE_DISTANCE_CM = 20.0;  // back up if closer than this
const float REVERSE_DURATION_MS  = 600;   // how long to reverse
const float SCAN_PAUSE_MS        = 300;   // pause between left/right scans

// Servo positions
const int SERVO_CENTER = 90;
const int SERVO_LEFT   = 160;
const int SERVO_RIGHT  = 20;
