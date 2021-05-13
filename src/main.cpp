#include <iostream>
#include <unistd.h>

#include "I2C.hpp"
#include "ADXL345.hpp"
#include "ITG_3205.hpp"
#include "HMC5883L.hpp"

int main(void) {

  I2C i2c("/dev/i2c-2");
  ADXL345 accelero(i2c);
  //HMC5883L compass(i2c);
  //ITG_3205 gyroscope(i2c);

  accelero.set_power_ctrl(ADXL345_MEASURE);

  accelero.offset_calibration();
  while(1) {
    if(accelero.self_test()) {
      std::cout << "Self test OK" << std::endl;
    } else {
      std::cout << "Self test Fail" << std::endl;
    }
    sleep(1);
  }

  return 0;
}
