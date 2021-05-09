#pragma once

#include <cstdint>

// Registers
#define ITG_3205_WHO_AM_I      0x00  // Store the sensor address
#define ITG_3205_SMPLRT_DIV    0x15  // divider between 0-255
#define ITG_3205_DLPF_FS       0x16  // Digital Low Pass Filter and Scale
#define ITG_3205_INT_CFG       0x17  // Interupt configuration
#define ITG_3205_INT_STATUS    0x1A  // Interrupt Status
#define ITG_3205_TEMP_OUT_H    0x1B
#define ITG_3205_TEMP_OUT_L    0x1C
#define ITG_3205_GYRO_XOUT_H   0x1D
#define ITG_3205_GYRO_XOUT_L   0x1E
#define ITG_3205_GYRO_YOUT_H   0x1F
#define ITG_3205_GYRO_YOUT_L   0x20
#define ITG_3205_GYRO_ZOUT_H   0x21
#define ITG_3205_GYRO_ZOUT_L   0x22
#define ITG_3205_PWR_MGM       0x3E  // Power Management

// Digital Low Pass Configuration Parameters
#define ITG_3205_FS_SEL_1          (1 << 4)
#define ITG_3205_FS_SEL_0          (1 << 3)
#define ITG_3205_DLPF_CFG_2        (1 << 2)
#define ITG_3205_DLPF_CFG_1        (1 << 1)
#define ITG_3205_DLPF_CFG_0        (1 << 0)

// Interruption Parameters
#define ITG_3205_ACTL              (1 << 7) // Logic level for INT output pin – 1=active low, 0=active high
#define ITG_3205_OPEN              (1 << 6) // Drive type for INT output pin – 1=open drain, 0=push-pull
#define ITG_3205_LATCH_INT_EN      (1 << 5) // Latch mode – 1=latch until interrupt is cleared, 0=50us pulse
#define ITG_3205_INT_ANYRD_2CLEAR  (1 << 4) // Latch clear method – 1=any register read, 0=status register read only
#define ITG_3205_ITG_RDY_EN        (1 << 2) // Enable interrupt when device is ready (PLL ready after changing clock source)
#define ITG_3205_RAW_RDY_EN        (1 << 0) // Enable interrupt when data is available

// Interruption status Parameters
#define ITG_3205_INT_RDY           (1 << 2) // PLL ready
#define ITG_3205_RAW_DATA_RDY      (1 << 0) // Raw data is ready

// Power Management settings
#define ITG_3205_H_RESET           (1 << 7) // Reset device and internal registers to the power-up-default settings
#define ITG_3205_SLEEP             (1 << 6) // Enable low power sleep mode
#define ITG_3205_STBY_XG           (1 << 5) // Put gyro X in standby mode (1=standby, 0=normal)
#define ITG_3205_STBY_YG           (1 << 4) // Put gyro Y in standby mode (1=standby, 0=normal)
#define ITG_3205_STBY_ZG           (1 << 3) // Put gyro Z in standby mode (1=standby, 0=normal)

class ITG_3205 {
  public:
    ITG_3205(I2C &i2c);

    /*
    * Return the device address
    */
    uint8_t get_id(void);
    /*
    * Set new device address
    * Bit 7 is always 0
    * Bits 6-1 is address
    * Bit 0 is defined in hardware
    */
    bool set_id(uint8_t address);
    /*
    * Return the value that divides the internal frequency to get the sample frequency
    * Value is between 0-255
    */
    uint8_t get_sample_rate_divider(void);
    /*
    * Set the divider value to divide the frequency
    * F_sample = F_internal/(divider + 1)
    */
    bool set_sample_rate_divider(uint8_t new_rate);
    /*
    * Return which the configuration of the low pass filter
    */
    uint8_t get_digital_low_pass_filter_config(void);
    /*
    * Set the divider value to divide the frequency
    * Bit | Function
    * 7   - 0
    * 6   - 0
    * 5   - 0
    * 4   - full scale select
    * 3   - full scale select
    * 2   - digital low pass filter config
    * 1   - digital low pass filter config
    * 0   - digital low pass filter config
    *
    * Full scale select | Gyro Full-Scale Range
    * 0                 - Reserved
    * 1                 - Reserved
    * 2                 - Reserved
    * 3                 - +/-2000°/sec
    *
    * Digital low pass filter configuration = DLPF_CFG
    * Low pass filter bandwidth = LPFB
    * Internal Sample Rate = ISR
    *
    * DLPF_CFG | LPFB     | ISR
    * 0        | 256Hz    | 8kHz
    * 1        | 188Hz    | 1kHz
    * 2        | 98Hz     | 1kHz
    * 3        | 42Hz     | 1kHz
    * 4        | 20Hz     | 1kHz
    * 5        | 10Hz     | 1kHz
    * 6        | 5Hz      | 1kHz
    * 7        | Reserved | Reserved
    */
    bool set_digital_low_pass_filter_config(uint8_t config);
    /*
    * Return the interrupt configuration
    */
    uint8_t get_interrupt_configuration(void);
    /*
    * Set the interrupt configuration
    *
    * Bit | Function
    * 7   - Logical level for INT output pin: 1 = active low, 0 = active high
    * 6   - Drive type for INT output pin: 1 = open drain, 0 = push-pull
    * 5   - Latch mode: 1 = latch until interrupt is cleared, 0 = 50us pulse
    * 4   - Latch clear method: 1 = any register read, 0 = status register read only
    * 3   - 0
    * 2   - Enable interrupt when device is ready (PLL ready after changing clock source)
    * 1   - 0
    * 0   - Enable interrupt when data is available
    */
    bool set_interrupt_configuration(uint8_t config);
    /*
    * Return if any function triggered an interruption
    *
    * Bit | Function
    * 7   - 0
    * 6   - 0
    * 5   - 0
    * 4   - 0
    * 3   - 0
    * 2   - PLL ready
    * 1   - 0
    * 0   - Raw data ready
    */
    uint8_t get_interrupt_status(void);
    /*
    * Return sensor temperature value
    */
    float get_temperature(void);
    /*
    * Return sensor x-axis value
    */
    float get_x_value(void);
    /*
    * Return sensor y-axis value
    */
    float get_y_value(void);
    /*
    * Return sensor z-axis value
    */
    float get_z_value(void);
    /*
    * Read all sensor data in one burst
    */
    void get_raw_data(void);
    /*
    * Return the power control, clock source of the device
    */
    uint8_t get_power_management_configuration(void);
    /*
    * Set the power control, clock source of the device
    *
    * Bit | Function
    * 7   - Reset device and internal registers to the power-up-default settings
    * 6   - Enable low power sleep mode
    * 5   - Put gyro X in standby mode (1=standby, 0=normal)
    * 4   - Put gyro Y in standby mode (1=standby, 0=normal)
    * 3   - Put gyro Z in standby mode (1=standby, 0=normal)
    * 2   - Select device clock source
    * 1   - Select device clock source
    * 0   - Select device clock source
    *
    * Clock Select  | Clock source
    * 0             - internal oscillator
    * 1             - PLL with X Gyro reference
    * 2             - PLL with Y Gyro reference
    * 3             - PLL with Z Gyro reference
    * 4             - PLL with external 32.768kHz reference
    * 5             - PLL with external 19.2MHz reference
    * 6             - Reserved
    * 7             - Reserved
    */
    bool set_power_management_configuration(uint8_t config);

  private:
    const uint8_t _fs_sel = ITG_3205_FS_SEL_1 + ITG_3205_FS_SEL_0;
    uint8_t _id, _sample_rt_div, _dlpf_cfg;
    uint8_t _interrupt_config, _power_management;

    float _x_axis, _y_axis, _z_axis, _temperature;

    I2C &_i2c;

    /*
    * Funtion to write one byte in a specific register of the device
    */
    bool writeRegister(uint8_t address, uint8_t data);
    /*
    * Funtion to read one or more bytes from the registers, following the
    * datasheet, the internal pointer of the register is incremented in each read,
    * therefore is only needed the first register address.
    */
    bool readRegister(uint8_t address, uint8_t *data, uint8_t length = 1);
};
