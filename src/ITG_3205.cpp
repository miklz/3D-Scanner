#include <iostream>
#include <cerrno>
#include <unistd.h>
#include <string.h>
#include <endian.h>

#include "I2C.hpp"
#include "ITG_3205.hpp"

ITG_3205::ITG_3205(I2C &i2c) : _i2c(i2c) {
  this->_id = 0x68;
  set_sample_rate_divider(7);
  set_digital_low_pass_filter_config(0);
  //set_interrupt_configuration(ITG_3205_LATCH_INT_EN +
  //  ITG_3205_INT_ANYRD_2CLEAR + ITG_3205_RAW_RDY_EN);
  set_power_management_configuration(0);
}

/**
 * @bref  Return the ID of the device.
 * @param None
 * @return The address of the device as a Byte
 */
uint8_t ITG_3205::get_id(void) {
  return _id;
}

/**
 * @bref  Set new ID
 * @param ID as a Byte
 * @return true if success or false if don't
 */
bool ITG_3205::set_id(uint8_t id) {
  bool b = this->writeRegister(ITG_3205_WHO_AM_I, id);
  if(b) {
    this->_id = id;
  }
  return b;
}

/**
 * @bref  Return the value of the divider
 * @param None
 * @return The current value as a value between 0-255 (Byte)
 */
uint8_t ITG_3205::get_sample_rate_divider(void) {
  return _sample_rt_div;
}

/**
 * @bref  Set new divider rate
 * @param Value between 0 and 255
 * @return true if success or false if don't
 */
bool ITG_3205::set_sample_rate_divider(uint8_t new_rate) {
  bool b = this->writeRegister(ITG_3205_SMPLRT_DIV, new_rate);
  if(b) {
    this->_sample_rt_div = new_rate;
  }
  return b;
}

/**
 * @bref  Return the current configuration of the filter
 * @param None
 * @return A Byte representing the configuration (see .hpp file)
 */
uint8_t ITG_3205::get_digital_low_pass_filter_config(void) {
  return _dlpf_cfg;
}

/**
 * @bref  Set configuration of the filter
 * @param Byte with the config
 * @return true if success or false if don't
 */
bool ITG_3205::set_digital_low_pass_filter_config(uint8_t config) {
  bool b = this->writeRegister(ITG_3205_DLPF_FS, _fs_sel + config);
  if(b) {
    this->_dlpf_cfg = config;
  }
  return b;
}

/**
 * @bref  Return the configuration of the interruptions
 * @param None
 * @return A Byte representing the configuration (see .hpp file)
 */
uint8_t ITG_3205::get_interrupt_configuration(void) {
  return _interrupt_config;
}

/**
 * @bref Set the configuration of the interruptions
 * @param A Byte representing the new configuration (see .hpp file)
 * @return true if success or false if don't
 */
bool ITG_3205::set_interrupt_configuration(uint8_t config) {
  bool b = this->writeRegister(ITG_3205_DLPF_FS, config);
  if(b) {
    this->_interrupt_config = config;
  }
  return b;
}

/**
 * @bref  Return the status of the interrut register
 * @param None
 * @return A Byte representing if new data is available
 */
uint8_t ITG_3205::get_interrupt_status(void) {
  uint8_t data = 0;

  readRegister(ITG_3205_INT_STATUS, &data);

  return data;
}

/**
 * @bref  Return the temperature value
 * @param None
 * @return float
 */
float ITG_3205::get_temperature(void) {
  return _temperature;
}

/**
 * @bref  Return the roll
 * @param None
 * @return float
 */
float ITG_3205::get_x_value(void) {
  return _x_axis;
}

/**
 * @bref  Return the pitch
 * @param None
 * @return float
 */
float ITG_3205::get_y_value(void) {
  return _y_axis;
}

/**
 * @bref  Return the yaw
 * @param None
 * @return float
 */
float ITG_3205::get_z_value(void) {
  return _z_axis;
}

/**
 * @bref  Load all at once and do the convertion
 * @param None
 * @return None
 */
void ITG_3205::get_raw_data(void) {
  uint16_t data[4];
  if(readRegister(ITG_3205_TEMP_OUT_H, (uint8_t *)data, 8)) {
    _temperature = 35 + (((int16_t)htobe16(data[0])) + 13200)/280;
    _x_axis = ((int16_t)htobe16(data[1]))/14.375;
    _y_axis = ((int16_t)htobe16(data[2]))/14.375;
    _z_axis = ((int16_t)htobe16(data[3]))/14.375;
  }
}

/**
 * @bref  Return the operation mode of the device (see .hpp)
 * @param None
 * @return The current configuration as a Byte
 */
uint8_t ITG_3205::get_power_management_configuration(void) {
  return _power_management;
}

/**
 * @bref  Set new power operation of the device
 * @param New configuration
 * @return true if success or false if don't
 */
bool ITG_3205::set_power_management_configuration(uint8_t config) {
  bool b = this->writeRegister(ITG_3205_PWR_MGM, config);
  if(b) {
    this->_power_management = config;
  }
  return b;
}

/**
 * @bref  Internal function to read using i2c
 * @param Register address
 * @param Data to read
 * @param How many bytes to read
 * @return true if success or false if don't
 */
bool ITG_3205::readRegister(uint8_t address, uint8_t *data, uint8_t length) {
  _i2c.write_register(this->_id, address, data, 0); // send address to read from

  uint8_t b = _i2c.read_register(this->_id, address, data, length);
  if (!b) {
		throw(std::runtime_error("Error reading word from register"));
    return false;
	}

  return true;
}

/**
 * @bref  Internal function to write using i2c
 * @param Register address
 * @param Data to write (byte)
 * @return true if success or false if don't
 */
bool ITG_3205::writeRegister(uint8_t address, uint8_t data) {
  bool b = _i2c.write_register(this->_id, address, &data);
	if (!b) {
		throw(std::runtime_error(std::string("Error writing byte to register: ") + strerror(errno)));
	}
  return b;
}
