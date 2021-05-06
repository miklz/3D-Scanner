
// Library to handle files
#include <iostream>
#include <fstream>
#include <cstring>

// System files to use i2c
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

// Identify the error
#include <errno.h>

// Our I2C Class
#include "I2C.hpp"

I2C::I2C(char *i2c_path) {

  _i2c_file = open(i2c_path, O_RDWR);

  if(_i2c_file < 0) {
    perror("Open I2C system file failed: ");
    exit(-1);
  }
}

bool I2C::read_register(uint8_t deviceAddress, uint8_t registerAddress,
                        uint8_t *dataPointer, uint8_t length) {

  if (ioctl(_i2c_file, I2C_SLAVE, deviceAddress) < 0) {
    perror("Problem with device communication: ");
    return false;
  }

  if (read(_i2c_file, dataPointer, length) != length) {
    perror("I2C Read failed: ");
    return false;
  }

  return true;
}

bool I2C::write_register(uint8_t deviceAddress, uint8_t registerAddress,
                         uint8_t *dataPointer, uint8_t length) {

  if (ioctl(_i2c_file, I2C_SLAVE, deviceAddress) < 0) {
    perror("Problem with device communication: ");
    return false;
  }

  uint8_t buffer[(length + 1)];
  buffer[0] = registerAddress;

  memcpy(&buffer[1], dataPointer, length);

  if(write(_i2c_file, buffer, length+1)) {
    perror("I2C Write failed: ");
    return false;
  }

  return true;
}

int main (void) {
  return 0;
}
