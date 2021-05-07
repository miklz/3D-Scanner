#pragma once

#include <cstdint>

class I2C {
  public:
    /*
    * @bref Constructor
    * @param system path like "/dev/i2c-2"
    */
    I2C(char const *i2c_path);
    /*
    * @bref Read from I2C device
    * @param Address of the i2c device
    * @param Register address to read from
    * @param Buffer of data to store the values read
    * @param How many bytes to read
    * @return true is succeeded and false if don't
    */
    bool read_register(uint8_t deviceAddress, uint8_t registerAddress,
      uint8_t *dataPointer, uint8_t length=1);
    /*
    * @bref Write to I2C device
    * @param Address of the i2c device
    * @param Register address to read from
    * @param Buffer of data to send to device
    * @param How many bytes to write
    * @return true is succeeded and false if don't
    */
    bool write_register(uint8_t deviceAddress, uint8_t registerAddress,
      uint8_t *dataPointer, uint8_t length=1);

  private:
    int _i2c_file;
};
