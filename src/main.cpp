#include <iostream>
#include <unistd.h>
#include <iomanip>
#include <cmath>

#include "I2C.hpp"
#include "VL53L0X.hpp"
#include "ADXL345.hpp"
#include "ITG_3205.hpp"
#include "HMC5883L.hpp"

void accuracy_vl53l0x(int n_samples) {
  I2C i2c("/dev/i2c-2");
  VL53L0X distance_sensor(i2c);
  distance_sensor.initialize();
  distance_sensor.setTimeout(200);
  // High precision
  distance_sensor.setMeasurementTimingBudget(200000);

  uint16_t measure, samples[n_samples+1];
  uint32_t sum;
  for(int i = 0; i < n_samples+1; ++i) {
    measure = distance_sensor.readRangeSingleMillimeters();
    samples[i] = measure;
    sum += measure;
    sleep(1);
  }
  sum -= samples[0];
  uint32_t mean = sum/n_samples;
  uint16_t variance = 0;
  for(int i = 1; i < n_samples; ++i) {
    variance += (samples[i] - mean)*(samples[i] - mean);
  }
  variance = variance/n_samples;


  std::cout << "VL53L0X - Distance Sensor" << std::endl;
  std::cout << "Mean Value: " << mean << std::endl;
  std::cout << "Variance: " << variance << std::endl;
  std::cout << "STD: " << std::sqrt(variance) << std::endl;
  std::cout << std::endl;
}

void accuracy_accelero(int n_samples) {
  I2C i2c("/dev/i2c-2");
  ADXL345 accelero(i2c);
  accelero.set_power_ctrl(ADXL345_MEASURE);

  float x_acc[n_samples], y_acc[n_samples], z_acc[n_samples];
  float sum_x = 0, sum_y = 0, sum_z = 0;
  for(int i = 0; i < n_samples; ++i) {
      accelero.get_raw_data();
      x_acc[i] = accelero.get_x_value();
      y_acc[i] = accelero.get_y_value();
      z_acc[i] = accelero.get_z_value();

      sum_x += accelero.get_x_value();
      sum_y += accelero.get_y_value();
      sum_z += accelero.get_z_value();
      sleep(1);
  }

  float mean_x = sum_x/n_samples, mean_y = sum_y/n_samples, mean_z = sum_z/n_samples;
  float variance_x = 0, variance_y = 0, variance_z = 0;
  for(int i = 1; i < n_samples; ++i) {
    variance_x += (x_acc[i] - mean_x)*(x_acc[i] - mean_x);
    variance_y += (y_acc[i] - mean_y)*(y_acc[i] - mean_y);
    variance_z += (z_acc[i] - mean_z)*(z_acc[i] - mean_z);
  }
  variance_x = variance_x/n_samples;
  variance_y = variance_y/n_samples;
  variance_z = variance_z/n_samples;

  std::cout << "Accelerometer" << std::endl;
  std::cout << std::setw(20) << "X-axis" << std::setw(20) << "Y-Axis" << std::setw(20) << "Z-Axis" << std::endl;
  std::cout << "Mean" << std::setw(19) << mean_x << std::setw(20);
  std::cout << mean_y << std::setw(20) << mean_z << std::endl;

  std::cout << "Variance" << std::setw(15) << variance_x << std::setw(20);
  std::cout << variance_y << std::setw(20) << variance_z << std::endl;

  std::cout << "STD" << std::setw(20) << std::sqrt(variance_x) << std::setw(20);
  std::cout << std::sqrt(variance_y) << std::setw(20) << std::sqrt(variance_z) << std::endl;
  std::cout << std::endl;
}

void accuracy_gyroscope(int n_samples) {
  I2C i2c("/dev/i2c-2");
  ITG_3205 gyroscope(i2c);

  float x_gyro[n_samples], y_gyro[n_samples], z_gyro[n_samples];
  float sum_x = 0, sum_y = 0, sum_z = 0;
  for(int i = 0; i < n_samples; ++i) {
    gyroscope.get_raw_data();
    x_gyro[i] = gyroscope.get_x_value();
    y_gyro[i] = gyroscope.get_y_value();
    z_gyro[i] = gyroscope.get_z_value();

    sum_x += gyroscope.get_x_value();
    sum_y += gyroscope.get_y_value();
    sum_z += gyroscope.get_z_value();
    sleep(1);
  }

  float mean_x = sum_x/n_samples, mean_y = sum_y/n_samples, mean_z = sum_z/n_samples;
  float variance_x = 0, variance_y = 0, variance_z = 0;
  for(int i = 1; i < n_samples; ++i) {
    variance_x += (x_gyro[i] - mean_x)*(x_gyro[i] - mean_x);
    variance_y += (y_gyro[i] - mean_y)*(y_gyro[i] - mean_y);
    variance_z += (z_gyro[i] - mean_z)*(z_gyro[i] - mean_z);
  }
  variance_x = variance_x/n_samples;
  variance_y = variance_y/n_samples;
  variance_z = variance_z/n_samples;

  std::cout << "Gyroscope" << std::endl;
  std::cout << std::setw(20) << "X-axis" << std::setw(20) << "Y-Axis" << std::setw(20) << "Z-Axis" << std::endl;
  std::cout << "Mean" << std::setw(19) << mean_x << std::setw(20);
  std::cout << mean_y << std::setw(20) << mean_z << std::endl;

  std::cout << "Variance" << std::setw(15) << variance_x << std::setw(20);
  std::cout << variance_y << std::setw(20) << variance_z << std::endl;

  std::cout << "STD" << std::setw(20) << std::sqrt(variance_x) << std::setw(20);
  std::cout << std::sqrt(variance_y) << std::setw(20) << std::sqrt(variance_z) << std::endl;
  std::cout << std::endl;
}

void accuracy_magnetometer(int n_samples) {
  I2C i2c("/dev/i2c-2");
  HMC5883L compass(i2c);

  float x_comp[n_samples], y_comp[n_samples], z_comp[n_samples];
  float sum_x = 0, sum_y = 0, sum_z = 0;
  for(int i =0; i < n_samples; ++i) {
    compass.get_raw_data();
    x_comp[i] = compass.get_x_value();
    y_comp[i] = compass.get_y_value();
    z_comp[i] = compass.get_z_value();

    sum_x += compass.get_x_value();
    sum_y += compass.get_y_value();
    sum_z += compass.get_z_value();
    sleep(1);
  }

  float mean_x = sum_x/n_samples, mean_y = sum_y/n_samples, mean_z = sum_z/n_samples;
  float variance_x = 0, variance_y = 0, variance_z = 0;
  for(int i = 1; i < n_samples; ++i) {
    variance_x += (x_comp[i] - mean_x)*(x_comp[i] - mean_x);
    variance_y += (y_comp[i] - mean_y)*(y_comp[i] - mean_y);
    variance_z += (z_comp[i] - mean_z)*(z_comp[i] - mean_z);
  }
  variance_x = variance_x/n_samples;
  variance_y = variance_y/n_samples;
  variance_z = variance_z/n_samples;

  std::cout << "Magnetometer" << std::endl;
  std::cout << std::setw(20) << "X-axis" << std::setw(20) << "Y-Axis" << std::setw(20) << "Z-Axis" << std::endl;
  std::cout << "Mean" << std::setw(19) << mean_x << std::setw(20);
  std::cout << mean_y << std::setw(20) << mean_z << std::endl;

  std::cout << "Variance" << std::setw(15) << variance_x << std::setw(20);
  std::cout << variance_y << std::setw(20) << variance_z << std::endl;

  std::cout << "STD" << std::setw(20) << std::sqrt(variance_x) << std::setw(20);
  std::cout << std::sqrt(variance_y) << std::setw(20) << std::sqrt(variance_z) << std::endl;
  std::cout << std::endl;
}

int main(void) {

  accuracy_vl53l0x(100);
  accuracy_accelero(100);
  accuracy_gyroscope(100);
  accuracy_magnetometer(100);

  return 0;
}
