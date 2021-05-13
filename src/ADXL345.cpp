#include <iostream>
#include <cerrno>
#include <cmath>
#include <string.h>
#include <unistd.h>
#include <endian.h>

#include "I2C.hpp"
#include "ADXL345.hpp"

ADXL345::ADXL345(I2C &i2c): _i2c(i2c) {
  // Default values (see datasheet)
  _id = ADXL345_DEFAULT_ADDRESS;
  _offset_x = 0;
  _offset_y = 0;
  _offset_z = 0;
  _tap_duration = 0;
  _tap_latency = 0;
  _tap_window = 0;
  _active_threshold = 0;
  _inactive_threshold = 0;
  _time_inactive = 0;
  _axes_tap_ctrl = 0;
  _free_fall_threshold = 0;
  _free_fall_time = 0;
  _data_rt_power_ctrl = 0x0A;
  _power_ctrl = 0;
  _interrupt_enable_ctrl = 0;
  _interrupt_map_pin_ctrl = 0;
  _data_format = 0;
  _scale_factor = 0.0039;
}

/** @brief
 *  Returns the default id of the device this can only be changed with the
 *  ALT ADDRESS pin in the IC (see datasheet). If pin is high the address is 0x1D
 *  if is low than the default address is 0x53.
 *  @param None
 *  @return Address of the device
 */
uint8_t ADXL345::get_device_id(void) {
  return _id;
}

/** @brief  Returns the threshold value to be considered a tap
 *  @param  None
 *  @return The threshold value in mg
 */
uint16_t ADXL345::get_threshold_tap(void) {
  return _threshold_tap;
}

/** @brief  Set the threshold value to be considered a tap
 *  @param  Value of the threshold in mg
 *  @return true if success and false if failure
 */
bool ADXL345::set_threshold_tap(uint16_t milli_gs) {
  uint8_t value = milli_gs*0.016; // Scale = 62.5mg/LSB => 0.016LSB/mg

  if(milli_gs > 15937) {
    value = 0xFF;
    milli_gs = 16000;
  }

  bool b = this->writeRegister(ADXL345_TAP_THRESHOLD, value);
  if(b) {
    this->_threshold_tap = milli_gs;
  }
  return b;
}

/** @brief  Return the offset value to the x-axis
 *  @param  None
 *  @return Value of the offset
 */
int8_t ADXL345::get_offset_x(void) {
  return _offset_x;
}

/** @brief  Return the offset value to the y-axis
 *  @param  None
 *  @return Value of the offset
 */
int8_t ADXL345::get_offset_y(void) {
  return _offset_y;
}

/** @brief  Return the offset value to the z-axis
 *  @param  None
 *  @return Value of the offset
 */
int8_t ADXL345::get_offset_z(void) {
  return _offset_z;
}

/** @brief  Set the offset value to the x-axis
 *  @param  value of the offset
 *  @return true if success and false if failure
 */
bool ADXL345::set_offset_x(int8_t value) {
  bool b = this->writeRegister(ADXL345_X_AXIS_OFFSET, (uint8_t)value);
  if(b) {
    this->_offset_x = value;
  }
  return b;
}

/** @brief  Set the offset value to the y-axis
 *  @param  value of the offset
 *  @return true if success and false if failure
 */
bool ADXL345::set_offset_y(int8_t value) {
  bool b = this->writeRegister(ADXL345_Y_AXIS_OFFSET, (uint8_t)value);
  if(b) {
    this->_offset_y = value;
  }
  return b;
}

/** @brief  Set the offset value to the z-axis
 *  @param  value of the offset
 *  @return true if success and false if failure
 */
bool ADXL345::set_offset_z(int8_t value) {
  bool b = this->writeRegister(ADXL345_Z_AXIS_OFFSET, (uint8_t)value);
  if(b) {
    this->_offset_z = value;
  }
  return b;
}

/** @brief  Return the tap duration considered for interrupt
 *  @param  None
 *  @return The value of the tap duration
 */
uint8_t ADXL345::get_tap_duration(void) {
  return _tap_duration;
}

/** @brief  Set the tap duration considered for interrupt
 *  @param  value in milliseconds of the new tap duration
 *  @return true if success and false if failure
 */
bool ADXL345::set_tap_duration(uint8_t milliseconds) {
  uint8_t value = milliseconds*1.6; // Scale = 625us/LSB => 1.6LSB/ms

  if(milliseconds > 159) {
    value = 0xFF;
    milliseconds = 159;
  }

  bool b = this->writeRegister(ADXL345_TAP_DURATION, value);
  if(b) {
    this->_tap_duration = milliseconds;
  }
  return b;
}

/** @brief  Returns how many milliseconds the tap should take to be considered
 *          as a tap
 *  @param  None
 *  @return The value of the latency time
 */
uint16_t ADXL345::get_tap_latency(void) {
  return _tap_latency;
}

/** @brief  Set the minimum time a tap should take to be considered a tap
 *  @param  value in milliseconds of the new latency time
 *  @return true if success and false if failure
 */
bool ADXL345::set_tap_latency(uint16_t milliseconds) {
  uint8_t value = milliseconds*0.8; // Scale = 1.25ms/LSB => 0.8LSB/ms

  if(milliseconds > 318) {
    value = 0xFF;
    milliseconds = 319;
  }

  bool b = this->writeRegister(ADXL345_TAP_LATENCY, value);
  if(b) {
    this->_tap_latency = milliseconds;
  }
  return b;
}

/** @brief  Returns how many milliseconds must take between one tap and other
 *          to be a double tap
 *  @param  None
 *  @return The value of the time tap window
 */
uint16_t ADXL345::get_tap_window(void) {
  return _tap_window;
}

/** @brief  Set the minimum time window between taps to be a double tap
 *  @param  value in milliseconds of the new latency time
 *  @return true if success and false if failure
 */
bool ADXL345::set_tap_window(uint16_t milliseconds) {
  uint8_t value = milliseconds*0.8; // Scale = 1.25ms/LSB => 0.8LSB/ms

  if(milliseconds > 318) {
    value = 0xFF;
    milliseconds = 319;
  }

  bool b = this->writeRegister(ADXL345_TAP_WINDOW, value);
  if(b) {
    this->_tap_window = milliseconds;
  }
  return b;
}

/** @brief  Returns the threshold force value considered for the device be active
 *  @param  None
 *  @return The value of threshold for the device to be active in mg
 */
uint16_t ADXL345::get_active_threshold(void) {
  return _active_threshold;
}

/** @brief  Set the threshold force value considered for the device be active
 *  @param  Value in mg to consider the device active
 *  @return true if success and false if failure to set new value
 */
bool ADXL345::set_active_threshold(uint16_t milli_gs) {
  uint8_t value = milli_gs*0.016; // Scale = 62.5mg/LSB => 0.016LSB/mg

  if (milli_gs > 15937) {
    value = 0xFF;
    milli_gs = 16000;
  }

  bool b = this->writeRegister(ADXL345_ACTIVITY_THRESHOLD, value);
  if(b) {
    this->_active_threshold = milli_gs;
  }
  return b;
}

/** @brief  Returns the threshold force value considered for the device be inactive
 *  @param  None
 *  @return The value of threshold for the device to be inactive in mg
 */
uint16_t ADXL345::get_inactive_threshold(void) {
  return _inactive_threshold;
}

/** @brief  Set the threshold force value considered for the device be inactive
 *  @param  Value in mg to consider the device inactive
 *  @return true if success and false if failure to set new value
 */
bool ADXL345::set_inactive_threshold(uint16_t milli_gs) {
  uint8_t value = milli_gs*0.016; // Scale = 62.5mg/LSB => 0.016LSB/mg

  if (milli_gs > 15937) {
    value = 0xFF;
    milli_gs = 16000;
  }

  bool b = this->writeRegister(ADXL345_INACTIVITY_THRESHOLD, value);
  if(b) {
    this->_inactive_threshold = milli_gs;
  }
  return b;
}

/** @brief  Returns the threshold time value considered for the device be inactive
 *  @param  None
 *  @return The value of threshold for the device to be inactive in seconds
 */
uint8_t ADXL345::get_time_inactive(void) {
  return _time_inactive;
}

/** @brief  Set the threshold time value considered for the device be inactive
 *  @param  Value in mg to consider the device inactive
 *  @return true if success and false if failure to set new value
 */
bool ADXL345::set_time_inactive(uint8_t seconds) {
  bool b = this->writeRegister(ADXL345_INACTIVITY_TIME, seconds);
  if(b) {
    this->_time_inactive = seconds; // Scale = 1s/LSB => 1LSB/s
  }
  return b;
}

/** @brief  Returns the configuration of axes to be active or inactive
 *  @param  None
 *  @return The configuration running on device
 */
uint8_t ADXL345::get_active_inactive_ctrl(void) {
  return _active_inactive_ctrl;
}

/** @brief  Set the configuration of interruptions active/inactive
 *  @param  Configuration for axes to be active or inactive
 *  @return true if success and false if failure to set new configuration
 */
bool ADXL345::set_active_inactive_ctrl(uint8_t value) {
  bool b = this->writeRegister(ADXL345_INACTIVITY_TIME, value);
  if(b) {
    this->_active_inactive_ctrl = value;
  }
  return b;
}

/** @brief  Set the value of force to be considered free fall
 *  @param  Value in mg of force
 *  @return true if success and false if failure to new value
 */
bool ADXL345::set_free_fall_threshold(uint16_t milli_gs) {
  uint8_t value = milli_gs*0.016; // 62.5mg/LSB => 0.016LSB/mg

  if (milli_gs > 15937) {
    value = 0xFF;
    milli_gs = 16000;
  }

  bool b = this->writeRegister(ADXL345_FREE_FALL_THRESHOLD, value);
  if(b) {
    this->_free_fall_threshold = milli_gs;
  }
  return b;
}

/** @brief  Set the value in time to be considered free fall
 *  @param  Value in milliseconds
 *  @return true if success and false if failure to new value
 */
bool ADXL345::set_free_fall_time(uint16_t milliseconds) {
  uint8_t value = milliseconds*0.2; // Scale = 5ms/LSB => 0.2LSB/ms

  if(milliseconds > 1275) {
    value = 0xFF;
    milliseconds = 1275;
  }

  bool b = this->writeRegister(ADXL345_FREE_FALL_TIME, value);
  if(b) {
    this->_free_fall_time = milliseconds;
  }
  return b;
}

/** @brief  Returns the configuration of which axis can identify taps
 *  @param  None
 *  @return The configuration of the axes
 */
uint8_t ADXL345::get_axes_tap_ctrl(void) {
  return _axes_tap_ctrl;
}

/** @brief  Set the configuration of axis to identify taps
 *  @param  New configuration
 *  @return true if success and false if failure to new configuration
 */
bool ADXL345::set_axes_tap_ctrl(uint8_t value) {
  bool b = this->writeRegister(ADXL345_AXIS_EN_CTRL_ACT_INA, value);
  if(b) {
    this->_axes_tap_ctrl = value;
  }
  return b;
}

/** @brief  Returns which axis detected the activity or a tap
 *  @param  None
 *  @return The source of activity or the tap
 */
uint8_t ADXL345::get_tap_source(void) {
  uint8_t data = 0;

  readRegister(ADXL345_TAP_SOURCE, &data);

  return data;
}

/** @brief  Returns which axis detected the activity or a tap
 *  @param  None
 *  @return The configuration of the power and data rate
 */
uint8_t ADXL345::get_data_rt_power_ctrl(void) {
  return _data_rt_power_ctrl;
}

/** @brief  Set the data rate and power mode
 *  @param  New configuration
 *  @return true if success and false if failure to new configuration
 */
bool ADXL345::set_data_rt_power_ctrl(uint8_t value) {
  bool b = this->writeRegister(ADXL345_DATA_RATE_PWR_CTRL, value);
  if(b) {
    this->_data_rt_power_ctrl = value;
  }
  return b;
}

/** @brief  Returns the power behavior
 *  @param  None
 *  @return The mode that the device is working
 */
uint8_t ADXL345::get_power_ctrl(void) {
  return _power_ctrl;
}

/** @brief  Set the power behavior of the device (auto-sleep or not)
 *  @param  New configuration
 *  @return true if success and false if failure to new configuration
 */
bool ADXL345::set_power_ctrl(uint8_t value) {
  bool b = this->writeRegister(ADXL345_PWR_SAVING_FEAT_CTRL, value);
  if(b) {
    this->_power_ctrl = value;
  }
  return b;
}

/** @brief  Returns the control configuration of interruption
 *  @param  None
 *  @return The control configuration of interruption
 */
uint8_t ADXL345::get_interrupt_enable_ctrl(void) {
  return _interrupt_enable_ctrl;
}

/** @brief  Set the control configuration of interruption
 *  @param  New configuration
 *  @return true if success and false if failure to new configuration
 */
bool ADXL345::set_interrupt_enable_ctrl(uint8_t value) {
  bool b = this->writeRegister(ADXL345_INTERRUPT_ENABLE_CTRL, value);
  if(b) {
    this->_interrupt_enable_ctrl = value;
  }
  return b;
}

/** @brief  Returns the mapping of interruptions to the external pins
 *  @param  None
 *  @return The mapping of interruptions to the external pins
 */
uint8_t ADXL345::get_interrupt_map_pin_ctrl(void) {
  return _interrupt_map_pin_ctrl;
}

/** @brief  Set the control configuration of interruption mapping to pins INT1 and INT2
 *  @param  New mapping
 *  @return true if success and false if failure to new configuration
 */
bool ADXL345::set_interrupt_map_pin_ctrl(uint8_t value) {
  bool b = this->writeRegister(ADXL345_INTERRUPT_MAP_CTRL, value);
  if(b) {
    this->_interrupt_map_pin_ctrl = value;
  }
  return b;
}

/** @brief  Returns which function has triggered the interruption
 *  @param  None
 *  @return Which function has triggered the interruption
 */
uint8_t ADXL345::get_interrupt_source(void) {
  uint8_t data = 0;

  readRegister(ADXL345_INTERRUPT_SOURCE, &data);

  return data;
}

/** @brief  Returns how the data is passed to the registers
 *  @param  None
 *  @return How the data is stored in registers
 */
uint8_t ADXL345::get_data_format(void) {
  return _data_format;
}

/** @brief  Set new configuration on how the data is passed to the registers
 *  @param  New configuration
 *  @return true if success and false if failure to new data format
 */
bool ADXL345::set_data_format(uint8_t value) {
  bool b = this->writeRegister(ADXL345_DATA_FORMAT_CTRL, value);
  if(b) {
    this->_data_format = value;
  }

  if(value & ADXL345_FULL_RES) {
    _scale_factor = 0.0039;
  } else if((value & ((ADXL345_RANGE_1) | ADXL345_RANGE_0)) == 0) {
    _scale_factor = 0.0039;
  } else if((value & ((ADXL345_RANGE_1) | ADXL345_RANGE_0)) == 1) {
    _scale_factor = 0.0078;
  } else if((value & ((ADXL345_RANGE_1) | ADXL345_RANGE_0)) == 2) {
    _scale_factor = 0.0156;
  } else {
    _scale_factor = 0.0312;
  }

  return b;
}

/** @brief  Get acceleration value in the x-axis
 *  @param  None
 *  @return Value in g
 */
float ADXL345::get_x_value(void) {
  return _gx;
}

/** @brief  Get acceleration value in the y-axis
 *  @param  None
 *  @return Value in g
 */
float ADXL345::get_y_value(void) {
  return _gy;
}

/** @brief  Get acceleration value in the z-axis
 *  @param  None
 *  @return Value in g
 */
float ADXL345::get_z_value(void) {
  return _gz;
}

/**
 * @bref  Load all data at once and do the convertion
 * @param None
 * @return None
 */
void ADXL345::get_raw_data(void) {
  uint16_t data[3];

  readRegister(ADXL345_DATA_X0, (uint8_t *)&data, 6);

  _gx = ((int16_t)htole16(data[0]))*_scale_factor;
  _gy = ((int16_t)htole16(data[1]))*_scale_factor;
  _gz = ((int16_t)htole16(data[2]))*_scale_factor;
}

/** @brief  Returns how the FIFO is working
 *  @param  None
 *  @return FIFO configuration
 */
uint8_t ADXL345::get_fifo_ctrl(void) {
  return _fifo_ctrl;
}

/** @brief  Set mode of how FIFO should work
 *  @param  None
 *  @return true if success and false if failure to set new FIFO configuration
 */
bool ADXL345::set_fifo_ctrl(uint8_t value) {
  bool b = this->writeRegister(ADXL345_FIFO_CTRL, value);
  if(b) {
    this->_fifo_ctrl = value;
  }
  return b;
}

/** @brief  Returns how many samples there is to read and if there were interruption
 *  @param  None
 *  @return How the FIFO is going
 */
uint8_t ADXL345::get_fifo_status(void) {
  uint8_t data = 0;

  readRegister(ADXL345_FIFO_STATUS, (uint8_t *)&data);

  return data;
}

/** @brief  Correct the offset of the accelerometer
 *  @param  None
 *  @return True if the offset was performed successfuly and false if not
 */
bool ADXL345::offset_calibration(void) {

  uint8_t old_data_format = get_data_format();
  uint8_t new_data_format = ADXL345_FULL_RES + ADXL345_RANGE_1 + ADXL345_RANGE_0;

  if(!set_data_format(new_data_format)) {
    return false;
  }

  uint8_t old_power_ctrl = get_power_ctrl();

  if(!set_power_ctrl(ADXL345_MEASURE)) {
    set_data_format(old_data_format);
    return false;
  }

  uint8_t old_data_rt_pwr_ctrl = get_data_rt_power_ctrl();
  if(!set_data_rt_power_ctrl(0x0A)) {
    set_data_format(old_data_format);
    set_power_ctrl(old_power_ctrl);
    return false;
  }

  uint16_t data[3];
  int16_t x_0g = 0, y_0g = 0, z_0g = 0;

  for(uint8_t i = 0; i < 64; ++i) {
    sleep(0.1);
    readRegister(ADXL345_DATA_X0, (uint8_t *)&data, 6);
    x_0g = (int16_t)htole16(data[0]) + x_0g;
    y_0g = (int16_t)htole16(data[1]) + y_0g;
    z_0g = (int16_t)htole16(data[2]) + z_0g;
  }

  // >> of six is for the average
  // >> of 2 is for convert the resolution of the axes to the offset register
  int8_t x_off, y_off, z_off;
  x_off = -(x_0g >> 8);
  y_off = -(y_0g >> 8);
  z_off = -(((z_0g >> 6) + 256) >> 2);

  if(!((set_offset_x(x_off)) && (set_offset_y(y_off)) && (set_offset_z(z_off)))) {
    set_data_format(old_data_format);
    set_power_ctrl(old_power_ctrl);
    set_data_rt_power_ctrl(old_data_rt_pwr_ctrl);
    return false;
  }

  if(!set_data_format(old_data_format) || !set_power_ctrl(old_power_ctrl) ||
      !set_data_rt_power_ctrl(old_data_rt_pwr_ctrl)) {
    return false;
  }

  return true;
}

/** @brief  Performs a test to verify if the accelerometer
 *  @param  None
 *  @return True if the device is withing the expected and false orderwise
 *          but a false dosen't necessary indicates failure (see datasheet)
 */
bool ADXL345::self_test(void) {
  uint8_t old_data_format = get_data_format();
  uint8_t new_data_format = ADXL345_FULL_RES + ADXL345_RANGE_1 + ADXL345_RANGE_0;

  if(!set_data_format(new_data_format)) {
    return false;
  }

  uint8_t old_power_ctrl = get_power_ctrl();

  if(!set_power_ctrl(ADXL345_MEASURE)) {
    set_data_format(old_data_format);
    return false;
  }

  uint8_t old_data_rt_pwr_ctrl = get_data_rt_power_ctrl();
  if(!set_data_rt_power_ctrl(0x0C)) {
    set_data_format(old_data_format);
    set_power_ctrl(old_power_ctrl);
    return false;
  }

  uint16_t data[3];
  int16_t Xst_off = 0, Yst_off = 0, Zst_off = 0;

  for(uint8_t i = 0; i < 64; ++i) {
    sleep(0.1);
    readRegister(ADXL345_DATA_X0, (uint8_t *)&data, 6);
    Xst_off = (int16_t)htole16(data[0]) + Xst_off;
    Yst_off = (int16_t)htole16(data[1]) + Yst_off;
    Zst_off = (int16_t)htole16(data[2]) + Zst_off;
  }

  Xst_off = (Xst_off >> 6);
  Yst_off = (Yst_off >> 6);
  Zst_off = (Zst_off >> 6);

  new_data_format = new_data_format + ADXL345_SELF_TEST;
  if(!set_data_format(new_data_format)) {
    set_data_format(old_data_format);
    set_power_ctrl(old_power_ctrl);
    set_data_rt_power_ctrl(old_data_rt_pwr_ctrl);
    return false;
  }

  // It need to colect a few samples to settle
  for(uint8_t i = 0; i < 10; ++i) {
    sleep(0.1);
    readRegister(ADXL345_DATA_X0, (uint8_t *)&data, 6);
  }

  int16_t Xst_on = 0, Yst_on = 0, Zst_on = 0;

  for(uint8_t i = 0; i < 64; ++i) {
    sleep(0.1);
    readRegister(ADXL345_DATA_X0, (uint8_t *)&data, 6);
    Xst_on = (int16_t)htole16(data[0]) + Xst_on;
    Yst_on = (int16_t)htole16(data[1]) + Yst_on;
    Zst_on = (int16_t)htole16(data[2]) + Zst_on;
  }

  Xst_on = (Xst_on >> 6);
  Yst_on = (Yst_on >> 6);
  Zst_on = (Zst_on >> 6);

  int16_t Xst = Xst_on - Xst_off;
  int16_t Yst = Yst_on - Yst_off;
  int16_t Zst = Zst_on - Zst_off;

  // Scale factor for 3.30V
  std::cout << Xst << std::endl;
  std::cout << Yst << std::endl;
  std::cout << Zst << std::endl;
  if((Xst < 6*1.77) || (Xst > 67*1.77) || (Yst < -67*1.77) ||
  (Yst > -6*1.77) || (Zst < 10*1.47) || (Zst > 110*1.47)) {
      set_data_format(old_data_format);
      set_power_ctrl(old_power_ctrl);
      set_data_rt_power_ctrl(old_data_rt_pwr_ctrl);
      return false;
    }

  set_data_format(old_data_format);
  set_power_ctrl(old_power_ctrl);
  set_data_rt_power_ctrl(old_data_rt_pwr_ctrl);

  return true;
}

 /**
  * @bref  Internal function to write using i2c
  * @param Register address
  * @param Data to write (byte)
  * @return true if success or false if don't
  */
bool ADXL345::writeRegister(uint8_t address, uint8_t data) {
  bool b = _i2c.write_register(this->_id, address, &data);
	if (!b) {
		throw(std::runtime_error(std::string("Error writing byte to register: ") + strerror(errno)));
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
bool ADXL345::readRegister(uint8_t address, uint8_t *data, uint8_t length) {
  _i2c.write_register(this->_id, address, data, 0); // send address to read from

  uint8_t b = _i2c.read_register(this->_id, address, data, length);
  if (!b) {
		throw(std::runtime_error("Error reading word from register"));
    return false;
	}
  return true;
}
