#include <iostream>
#include <unistd.h>

#include "I2C.hpp"
#include "ADXL345.hpp"
#include "ITG_3205.hpp"
#include "HMC5883L.hpp"

int main(void) {

  I2C i2c("/dev/i2c-2");
  ADXL345 accelero(i2c);
  HMC5883L compass(i2c);
  ITG_3205 gyroscope(i2c);

  accelero.set_power_ctrl(ADXL345_MEASURE);

  while(1) {
    accelero.get_raw_data();
    compass.get_raw_data();
    gyroscope.get_raw_data();
    std::cout << "Accelerometer X-axis: " << accelero.get_x_value() << std::endl;
    std::cout << "Accelerometer Y-axis: " << accelero.get_y_value() << std::endl;
    std::cout << "Accelerometer Z-axis: " << accelero.get_z_value() << std::endl;
    std::cout << "Gyroscope X-axis: " << gyroscope.get_x_value() << std::endl;
    std::cout << "Gyroscope Y-axis: " << gyroscope.get_y_value() << std::endl;
    std::cout << "Gyroscope Z-axis: " << gyroscope.get_z_value() << std::endl;
    std::cout << "Compass X-axis: " << compass.get_x_value() << std::endl;
    std::cout << "Compass Y-axis: " << compass.get_y_value() << std::endl;
    std::cout << "Compass Z-axis: " << compass.get_z_value() << std::endl;
    sleep(1);
  }

  return 0;
}
