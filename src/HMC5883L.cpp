#include <iostream>
#include <cerrno>
#include <unistd.h>
#include <string.h>
#include <endian.h>

#include "I2C.hpp"
#include "HMC5883L.hpp"

HMC5883L::HMC5883L(I2C &i2c) : _i2c(i2c) {
  // Default values (see datasheet)
  _a_register_config = 0x10;
  _b_register_config = 0x20;
  _mode = 0x01;
}

/**
 * @bref  Configure data rate and measurement mode
 * @param (char) New configuration
 * @return (bool) true if success false if don't
 */
bool HMC5883L::set_register_a_configuration(uint8_t configuration) {
  bool b = this->writeRegister(HMC5883_CONFIG_REGISTER_A, configuration);
  if(b) {
    this->_a_register_config = configuration;
  }
  return b;
}

/**
 * @bref  Return the configuration of the data rate and measurement mode
 * @param None
 * @return (char) current config
 */
uint8_t HMC5883L::get_register_a_configuration() {
  return _a_register_config;
}

/**
 * @bref  Configure the gain of the device
 * @param (char) New configuration
 * @return (bool) true if success false if don't
 */
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

/**
 * @bref  Return the gain configuration
 * @param None
 * @return (char) current config
 */
uint8_t HMC5883L::get_register_b_configuration() {
  return _b_register_config;
}

/**
 * @bref  Configure the I2C speed operation mode
 * @param (char) New configuration
 * @return (bool) true if success false if don't
 */
bool HMC5883L::set_mode_register(uint8_t mode) {
  bool b = this->writeRegister(HMC5883_MODE_REGISTER, mode);
  if(b) {
    this->_mode = mode;
  }
  return b;
}

/**
 * @bref  Return how the device is operating
 * @param None
 * @return (char) current mode
 */
uint8_t HMC5883L::get_mode_register(void) {
  return _mode;
}

/**
 * @bref  Value in mGauss of x-axis
 * @param None
 * @return (float) field value in x
 */
float HMC5883L::get_x_value(void) {
  return _x_axis;
}

/**
 * @bref  Value in mGauss of y-axis
 * @param None
 * @return (float) field value in y
 */
float HMC5883L::get_y_value(void) {
  return _y_axis;
}

/**
 * @bref  Value in mGauss of y-axis
 * @param None
 * @return (float) field value in y
 */
float HMC5883L::get_z_value(void) {
  return _z_axis;
}

/**
 * @bref  Update the values for all axes
 * @param None
 * @return None
 */
void HMC5883L::get_raw_data(void) {
  uint16_t data[3];
  if(readRegister(HMC5883_DATA_OUTPUT_X_MSB, (uint8_t *)data, 6)) {
    _x_axis = ((int16_t)htobe16(data[0]))*_digital_resolution;
    _y_axis = ((int16_t)htobe16(data[1]))*_digital_resolution;
    _z_axis = ((int16_t)htobe16(data[2]))*_digital_resolution;
  }
}

/**
 * @bref  Return the status if there's new values to read
 * @param None
 * @return (char) register value
 */
uint8_t HMC5883L::read_status_register(void) {
  uint8_t data = 0;

  readRegister(HMC5883_STATUR_REGISTER, &data);

  return data;
}

/**
 * @bref  Internal function to write in the registers
 * @param (char) Register address to write to
 * @param (char) Byte to write in the register
 * @return (bool) true if success or false if don't
 */
bool HMC5883L::writeRegister(uint8_t address, uint8_t data) {
  bool b = _i2c.write_register(HMC5883_DEFAULT_ADDRESS, address, &data);
	if (!b) {
		throw(std::runtime_error(std::string("Error writing byte to register: ") + strerror(errno)));
	}
  return b;
}

/**
 * @bref  Internal function to read in the registers
 * @param (char) Register address to read from
 * @param (char*) Buffer to store the values
 * @param (char) How many bytes to read
 * @return (bool) true if success or false if don't
 */
bool HMC5883L::readRegister(uint8_t address, uint8_t *data, uint8_t length) {
  _i2c.write_register(HMC5883_DEFAULT_ADDRESS, address, data, 0); // send address to read from

  uint8_t b = _i2c.read_register(HMC5883_DEFAULT_ADDRESS, address, data, length);
  if (!b) {
		throw(std::runtime_error("Error reading word from register"));
    return false;
	}
  return true;
}
