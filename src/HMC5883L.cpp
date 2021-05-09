#include <iostream>
#include <cerrno>
#include <unistd.h>
#include <string.h>
#include <endian.h>

#include "I2C.hpp"
#include "HMC5883L.hpp"

HMC5883L::HMC5883L(I2C &i2c) : _i2c(i2c) {
  _a_register_config = 0x10;
  _b_register_config = 0x20;
  _mode = 0x01;
}

bool HMC5883L::set_register_a_configuration(uint8_t configuration) {
  bool b = this->writeRegister(HMC5883_CONFIG_REGISTER_A, configuration);
  if(b) {
    this->_a_register_config = configuration;
  }
  return b;
}

uint8_t HMC5883L::get_register_a_configuration() {
  return _a_register_config;
}

bool HMC5883L::set_register_b_configuration(uint8_t configuration) {
  bool b = this->writeRegister(HMC5883_CONFIG_REGISTER_B, configuration);
  if(b) {
    this->_b_register_config = configuration;
  }

  uint8_t gain = ((configuration & (HMC5883_GN_2 | HMC5883_GN_1 | HMC5883_GN_0)) >> 5);
  switch(gain) {
    case 0:
      _digital_resolution = 0.73;
      break;
    case 1:
      _digital_resolution = 0.92;
      break;
    case 2:
      _digital_resolution = 1.22;
      break;
    case 3:
      _digital_resolution = 1.52;
      break;
    case 4:
      _digital_resolution = 2.27;
      break;
    case 5:
      _digital_resolution = 2.56;
      break;
    case 6:
      _digital_resolution = 3.03;
      break;
    case 7:
      _digital_resolution = 4.35;
      break;
  }

  return b;
}

uint8_t HMC5883L::get_register_b_configuration() {
  return _b_register_config;
}

bool HMC5883L::set_mode_register(uint8_t mode) {
  bool b = this->writeRegister(HMC5883_MODE_REGISTER, mode);
  if(b) {
    this->_mode = mode;
  }
  return b;
}

uint8_t HMC5883L::get_mode_register(void) {
  return _mode;
}

float HMC5883L::get_x_value(void) {
  return _x_axis;
}

float HMC5883L::get_y_value(void) {
  return _y_axis;
}

float HMC5883L::get_z_value(void) {
  return _z_axis;
}

void HMC5883L::get_raw_data(void) {
  uint16_t data[3];
  if(readRegister(HMC5883_DATA_OUTPUT_X_MSB, (uint8_t *)data, 6)) {
    _x_axis = ((int16_t)htobe16(data[0]))*_digital_resolution;
    _y_axis = ((int16_t)htobe16(data[1]))*_digital_resolution;
    _z_axis = ((int16_t)htobe16(data[2]))*_digital_resolution;
  }
}

uint8_t HMC5883L::read_status_register(void) {
  uint8_t data = 0;

  readRegister(HMC5883_STATUR_REGISTER, &data);

  return data;
}

bool HMC5883L::writeRegister(uint8_t address, uint8_t data) {
  bool b = _i2c.write_register(HMC5883_DEFAULT_ADDRESS, address, &data);
	if (!b) {
		throw(std::runtime_error(std::string("Error writing byte to register: ") + strerror(errno)));
	}
  return b;
}

bool HMC5883L::readRegister(uint8_t address, uint8_t *data, uint8_t length) {
  _i2c.write_register(HMC5883_DEFAULT_ADDRESS, address, data, 0); // send address to read from

  uint8_t b = _i2c.read_register(HMC5883_DEFAULT_ADDRESS, address, data, length);
  if (!b) {
		throw(std::runtime_error("Error reading word from register"));
    return false;
	}
  return true;
}

int main(void) {
  I2C i2c("/dev/i2c-2");
  HMC5883L magneto(i2c);

  while(1) {
    magneto.get_raw_data();
    std::cout << "X-axis: " << magneto.get_x_value() << std::endl;
    std::cout << "Y-axis: " << magneto.get_y_value() << std::endl;
    std::cout << "Z-axis: " << magneto.get_z_value() << std::endl;
    sleep(1);
  }

  return 0;
}
