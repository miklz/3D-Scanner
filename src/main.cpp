#include <iostream>
#include <unistd.h>

#include "I2C.hpp"
#include "VL53L0X.hpp"
#include "ADXL345.hpp"
#include "ITG_3205.hpp"
#include "HMC5883L.hpp"

int main(void) {

  I2C i2c("/dev/i2c-2");
  //I2C i2c("/dev/i2c-0");
  //ADXL345 accelero(i2c);
  //HMC5883L compass(i2c);
  //ITG_3205 gyroscope(i2c);
  VL53L0X distance_sensor(i2c);
  distance_sensor.initialize();
  distance_sensor.setTimeout(200);
  // High precision
  distance_sensor.setMeasurementTimingBudget(200000);

  //accelero.set_power_ctrl(ADXL345_MEASURE);

  while(1) {
    std::cout << distance_sensor.readRangeSingleMillimeters() << std::endl;
    sleep(1);
  }

  return 0;
}
