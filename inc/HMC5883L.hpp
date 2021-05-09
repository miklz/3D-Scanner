#pragma once

#define HMC5883_DEFAULT_ADDRESS           0x1E

// Registers
#define HMC5883_CONFIG_REGISTER_A         0x00
#define HMC5883_CONFIG_REGISTER_B         0x01
#define HMC5883_MODE_REGISTER             0x02
#define HMC5883_DATA_OUTPUT_X_MSB         0x03
#define HMC5883_DATA_OUTPUT_X_LSB         0x04
#define HMC5883_DATA_OUTPUT_Z_MSB         0x05
#define HMC5883_DATA_OUTPUT_Z_LSB         0x06
#define HMC5883_DATA_OUTPUT_Y_MSB         0x07
#define HMC5883_DATA_OUTPUT_Y_LSB         0x08
#define HMC5883_STATUR_REGISTER           0x09
#define HMC5883_IDENTIFICATION_REGISTER_A 0x0A
#define HMC5883_IDENTIFICATION_REGISTER_B 0x0B
#define HMC5883_IDENTIFICATION_REGISTER_C 0x0C

// Bits of Register A configuration
#define HMC5883_MA_1  (1 << 6)
#define HMC5883_MA_0  (1 << 5)
#define HMC5883_DO_2  (1 << 4)
#define HMC5883_DO_1  (1 << 3)
#define HMC5883_DO_0  (1 << 2)
#define HMC5883_MS_1  (1 << 1)
#define HMC5883_MS_0  (1 << 0)

// Bits of Register A configuration
#define HMC5883_GN_2  (1 << 7)
#define HMC5883_GN_1  (1 << 6)
#define HMC5883_GN_0  (1 << 5)

// Bits of mode register
#define HMC5883_HS    (1 << 7)
#define HMC5883_MD_1  (1 << 1)
#define HMC5883_MD_0  (1 << 0)

class HMC5883L {
  public:
    HMC5883L(I2C &i2c);

    /*
    * This register is for setting the data rate output and
    * measurement configuration.
    *
    * Bit | Function
    * 7   - 0
    * 6   - Number of samples averaged per measurement output
    * 5   - Number of samples averaged per measurement output
    * 4   - Data output rate bits
    * 3   - Data output rate bits
    * 2   - Data output rate bits
    * 1   - Measurement mode
    * 0   - Measurement mode
    *
    * MA = Number of samples averaged per measurement output
    * MA  | Samples averaged
    * 0   - 1 (default)
    * 1   - 2
    * 2   - 4
    * 3   - 8
    *
    * DOB = Data output bits
    * DOB | Typical Data Output Rate (Hz)
    * 0   - 0.75
    * 1   - 1.5
    * 2   - 3
    * 3   - 7.5
    * 4   - 15 (default)
    * 5   - 30
    * 6   - 75
    * 7   - Reserved
    *
    * MS = Measurement Mode
    * MS  | Description
    * 0   - Normal mode (default). The positive and negative pins are left
    *       floating and high impedance.
    * 1   - Positive bias configuration for X, Y, and Z axes. A positive current
    *       is forced across the resistive load for all three axes.
    * 2   - Negative bias configuration for X, Y, and Z axes. A negative current
    *       is forced across the resistive load for all three axes.
    * 3   - Reserved.
    */
    bool set_register_a_configuration(uint8_t configuration);
    /*
    * Return the current configuration of the A register
    */
    uint8_t get_register_a_configuration();
    /*
    * This register is for setting the device gain.
    *
    * Bit | Function
    * 7   - Gain
    * 6   - Gain
    * 5   - Gain
    * 4   - 0
    * 3   - 0
    * 2   - 0
    * 1   - 0
    * 0   - 0
    *
    * Gain bits  | Recommended Sensor Field Range | Gain (LSB/Gauss) | Digital Resolution (mG/LSB)
    * 0          -        +/- 0.88 Ga             -       1370       -          0.73
    * 1          -        +/- 1.3 Ga              -  1090(default)   -          0.92
    * 2          -        +/- 1.9 Ga              -        820       -          1.22
    * 3          -        +/- 2.5 Ga              -        660       -          1.52
    * 4          -        +/- 4.0 Ga              -        440       -          2.27
    * 5          -        +/- 4.7 Ga              -        390       -          2.56
    * 6          -        +/- 5.6 Ga              -        330       -          3.03
    * 7          -        +/- 8.1 Ga              -        230       -          4.35
    *
    * Note: The first measurement after a gain change uses the previous configuration.
    * The new gain setting is effective from the second measurement and on.
    *
    */
    bool set_register_b_configuration(uint8_t configuration);
    /*
    * Return the current configuration of the B register
    */
    uint8_t get_register_b_configuration();
    /*
    * This register is used to select the operating mode of the device.
    *
    * Bit | Function
    * 7   - High speed I2C (3400kHz)
    * 6   - 0
    * 5   - 0
    * 4   - 0
    * 3   - 0
    * 2   - 0
    * 1   - Mode select
    * 0   - Mode select
    *
    * Mode select bits  | Operating mode
    * 0                 - Continuous-Measurement mode.
    * 1                 - Single-Measurement mode (default).
    * 2                 - Idle Mode.
    * 3                 - Idle Mode.
    *
    */
    bool set_mode_register(uint8_t mode);
    /*
    * Return the mode that the device is operating on.
    */
    uint8_t get_mode_register(void);
    /*
    * Return the field in x
    */
    float get_x_value(void);
    /*
    * Return the field in y
    */
    float get_y_value(void);
    /*
    * Return the field in z
    */
    float get_z_value(void);
    /*
    * Colect new data of the axes
    */
    void get_raw_data(void);
    /*
    * Return the device status
    *
    * Bit | Description
    * 7   - 0
    * 6   - 0
    * 5   - 0
    * 4   - 0
    * 3   - 0
    * 2   - 0
    * 1   - Lock
    * 0   - Ready
    *
    * The Lock bit is set when:
    * 1. Some but not all six data output registers have been read.
    * 2. Mode register has been read.
    * This bit is cleared only when:
    * 1. All six bytes (data bytes) have been read.
    * 2. The mode register is changed.
    * 3. The measurement configuration is changed.
    * 4. Power is reset.
    *
    * The ready bit is set when data is written to all six data registers.
    * It's cleared when device initiates a write to the data output and after
    * one or more of the data registers are written to.
    * This bit can be monitored with the external interrupt pin: DRDY.
    */
    uint8_t read_status_register(void);

  private:
    uint8_t _a_register_config, _b_register_config, _mode;
    float _x_axis, _y_axis, _z_axis, _digital_resolution;

    I2C &_i2c;

    bool writeRegister(uint8_t address, uint8_t data);
    bool readRegister(uint8_t address, uint8_t *data, uint8_t length = 1);
};
