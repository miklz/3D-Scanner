#pragma once

#define ACCELERATOR_ADDR  0x53
#define GYROSCOPE_ADDR    0x68
#define COMPASS_ADDR      0x1E

class GY_85 {
  public:
    GY_85(I2C &i2c);

    Accel get_acceleration_data(void);
    Gyros get_gyroscope_data(void);
    Magnt get_magnetometer_data(void);
  private:
    I2C &_i2c;
    ADXL345 accelero;
    HMC5883L compass;
    ITG_3205 gyroscope;


}
