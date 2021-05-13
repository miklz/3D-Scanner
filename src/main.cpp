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

  while(1) {
    if(accelero.offset_calibration()) {
      std::cout << "Calibration OK" << std::endl;
    }
    sleep(1);
  }

  return 0;
}
