#include "imu.h"
#include "config.h"
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

static Adafruit_MPU6050 mpu;

bool imuInit() {
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!mpu.begin()) return false;

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  return true;
}

// Returns pitch angle in degrees derived from accelerometer
// Good enough for slow-moving balance; swap in a complementary filter if you
// start seeing drift at higher speeds
float imuGetPitch() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  return atan2(a.acceleration.x, a.acceleration.z) * 180.0 / PI;
}
