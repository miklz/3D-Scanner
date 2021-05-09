#pragma once

#include <cstdint>

#define ADXL345_DEFAULT_ADDRESS       0x53

// Registers
#define ADXL345_DEVICE_ID             0x00
#define ADXL345_TAP_THRESHOLD         0x1D
#define ADXL345_X_AXIS_OFFSET         0x1E
#define ADXL345_Y_AXIS_OFFSET         0x1F
#define ADXL345_Z_AXIS_OFFSET         0x20
#define ADXL345_TAP_DURATION          0x21
#define ADXL345_TAP_LATENCY           0x22
#define ADXL345_TAP_WINDOW            0x23
#define ADXL345_ACTIVITY_THRESHOLD    0x24
#define ADXL345_INACTIVITY_THRESHOLD  0x25
#define ADXL345_INACTIVITY_TIME       0x26
#define ADXL345_AXIS_EN_CTRL_ACT_INA  0x27 // Axis enable control for activity and inactivity detection
#define ADXL345_FREE_FALL_THRESHOLD   0x28
#define ADXL345_FREE_FALL_TIME        0x29
#define ADXL345_AXIS_CTRL_SNG_DBL_TAP 0x2A // Axis control for single tap/double tap
#define ADXL345_TAP_SOURCE            0x2B
#define ADXL345_DATA_RATE_PWR_CTRL    0x2C // Data rate and power mode control
#define ADXL345_PWR_SAVING_FEAT_CTRL  0x2D // Power-saving features control
#define ADXL345_INTERRUPT_ENABLE_CTRL 0x2E
#define ADXL345_INTERRUPT_MAP_CTRL    0x2F
#define ADXL345_INTERRUPT_SOURCE      0x30
#define ADXL345_DATA_FORMAT_CTRL      0x31
#define ADXL345_DATA_X0               0x32
#define ADXL345_DATA_X1               0x33
#define ADXL345_DATA_Y0               0x34
#define ADXL345_DATA_Y1               0x35
#define ADXL345_DATA_Z0               0x36
#define ADXL345_DATA_Z1               0x37
#define ADXL345_FIFO_CTRL             0x38
#define ADXL345_FIFO_STATUS           0x39

// Interruption on axes Parameters
#define ADXL345_ACT_AC_DC   (1 << 7)
#define ADXL345_ACT_X_EN    (1 << 6)
#define ADXL345_ACT_Y_EN    (1 << 5)
#define ADXL345_ACT_Z_EN    (1 << 4)
#define ADXL345_INACT_AC_DC (1 << 3)
#define ADXL345_INACT_X_EN  (1 << 2)
#define ADXL345_INACT_Y_EN  (1 << 1)
#define ADXL345_INACT_Z_EN  (1 << 0)

// Tap axes Parameters
#define ADXL345_SUPPRESS    (1 << 3)
#define ADXL345_TAP_X_EN    (1 << 2)
#define ADXL345_TAP_Y_EN    (1 << 1)
#define ADXL345_TAP_Z_EN    (1 << 0)

// Tap axes Status bits
#define ADXL345_ACT_X_SRC   (1 << 6)
#define ADXL345_ACT_Y_SRC   (1 << 5)
#define ADXL345_ACT_Z_SRC   (1 << 4)
#define ADXL345_ASLEEP      (1 << 3)
#define ADXL345_TAP_X_SRC   (1 << 2)
#define ADXL345_TAP_Y_SRC   (1 << 1)
#define ADXL345_TAP_Z_SRC   (1 << 0)

// Data rate and power mode control Parameters
#define ADXL345_LOW_POWER   (1 << 4)
#define ADXL345_RATE_3      (1 << 3)
#define ADXL345_RATE_2      (1 << 2)
#define ADXL345_RATE_1      (1 << 1)
#define ADXL345_RATE_0      (1 << 0)

// Power Control Parameters
#define ADXL345_LINK        (1 << 5)
#define ADXL345_AUTO_SLEEP  (1 << 4)
#define ADXL345_MEASURE     (1 << 3)
#define ADXL345_SLEEP       (1 << 2)
#define ADXL345_WAKEUP_1    (1 << 1)
#define ADXL345_WAKEUP_0    (1 << 0)

// Interruption Parameters
#define ADXL345_DATA_READY  (1 << 7)
#define ADXL345_SINGLE_TAP  (1 << 6)
#define ADXL345_DOUBLE_TAP  (1 << 5)
#define ADXL345_ACTIVITY    (1 << 4)
#define ADXL345_INACTIVITY  (1 << 3)
#define ADXL345_FREE_FALL   (1 << 2)
#define ADXL345_WATERMARK   (1 << 1)
#define ADXL345_OVERRUN     (1 << 0)

// Data Format configuration
#define ADXL345_SELF_TEST   (1 << 7)
#define ADXL345_SPI         (1 << 6)
#define ADXL345_INT_INVERT  (1 << 5)
#define ADXL345_FULL_RES    (1 << 3)
#define ADXL345_JUSTIFY     (1 << 2)
#define ADXL345_RANGE_1     (1 << 1)
#define ADXL345_RANGE_0     (1 << 0)

// Fifo configuration
#define ADXL345_FIFO_MODE_1 (1 << 7)
#define ADXL345_FIFO_MODE_0 (1 << 6)
#define ADXL345_TRIGGER     (1 << 5)
#define ADXL345_SAMPLES_4   (1 << 4)
#define ADXL345_SAMPLES_3   (1 << 3)
#define ADXL345_SAMPLES_2   (1 << 2)
#define ADXL345_SAMPLES_1   (1 << 1)
#define ADXL345_SAMPLES_0   (1 << 0)

// Fifo status
#define ADXL345_FIFO_TRIG   (1 << 7)
#define ADXL345_ENTRIES     0x1F

class ADXL345 {
  public:
    ADXL345(I2C &i2c);

    /*
    * Returns the device address, this address cannot be changed through software
    */
    uint8_t get_device_id(void);
    /*
    * Returns the threshold value stored in the register to activate the tap
    * interruption. The value is an unsigned byte and the scale factor is 62.5mg/LSB.
    * The returned value is between 0 - 16g.
    */
    uint16_t get_threshold_tap(void);
    /*
    * Set the value of threshold to activate the tap interruption.
    * The value must be in mg scale from 0 to 16g but is not recommended to set as 0
    * when the interruption by single or double tap is active since this could
    * cause undesired behavior. Due to the scale the minimum value is 63mg
    * If the value is bigger than 16g than the threshold will be set as 16g.
    */
    bool set_threshold_tap(uint16_t milli_gs);
    /*
    * Returns the x offset of the x-axis
    */
    uint8_t get_offset_x(void);
    /*
    * Returns the y offset of the y-axis
    */
    uint8_t get_offset_y(void);
    /*
    * Returns the z offset of the z-axis
    */
    uint8_t get_offset_z(void);
    /*
    * Set the x offset of the x-axis
    */
    bool set_offset_x(uint8_t value);
    /*
    * Set the y offset of the y-axis
    */
    bool set_offset_y(uint8_t value);
    /*
    * Set the z offset of the z-axis
    */
    bool set_offset_z(uint8_t value);
    /*
    * Returns the duration in milliseconds of the tap to activate the interruption.
    */
    uint8_t get_tap_duration(void);
    /*
    * Set duration of tap to activate the interruption.
    * The maximum value in milliseconds is 159ms and a value of 0 disable the
    * interruption by single/double tap.
    * If the value is bigger than 159ms the tap duration will be set as 159ms.
    */
    bool set_tap_duration(uint8_t milliseconds);
    /*
    * Returns the the time value representing the wait time of the time window during
    * which a second tap event can be detected.
    */
    uint16_t get_tap_latency(void);
    /*
    * Set the time value representing the wait time of the time window during
    * which a second tap event can be detected.
    * A value of zero disables the double tap function.
    * The maximum value is 319ms, if the value is bigger than 319ms the latency
    * will be set as 319ms.
    */
    bool set_tap_latency(uint16_t milliseconds);
    /*
    * Returns the time value in milliseconds representing the amount of time
    * after the expiration of the latency time during wich a second valid tap can begin.
    */
    uint16_t get_tap_window(void);
    /*
    * Set the time value in milliseconds representing the amount of time
    * after the expiration of the latency time during wich a second valid tap can begin.
    * A value of zero disables the double tap function.
    * The maximum value is 319ms, if the value is bigger than 319ms the latency
    * will be set as 319ms.
    */
    bool set_tap_window(uint16_t milliseconds);
    /*
    * Returns the magnitude value in gs representing the threshold value to detect
    * activity.
    */
    uint16_t get_active_threshold(void);
    /*
    * Set the magnitude value in gs to detect activity.
    * A value of zero may result in undesired behavior if the activity interrupt is
    * enable.
    */
    bool set_active_threshold(uint16_t milli_gs);
    /*
    * Returns the magnitude value in gs representing the threshold value to detect
    * inactivity.
    */
    uint16_t get_inactive_threshold(void);
    /*
    * Set the magnitude value in gs to detect inactivity.
    * A value of zero may result in undesired behavior if the inactivity interrupt is
    * enable.
    */
    bool set_inactive_threshold(uint16_t milli_gs);
    /*
    * Returns the amount of time in seconds that acceleration must be less than the value
    * in _inactive_threshold.
    */
    uint8_t get_time_inactive(void);
    /*
    * Set the amount of time in seconds that acceleration must be less than the value
    * in the  ADXL345_INACTIVITY_THRESHOLD register.
    * At least one sample must be generated for the inactivity interrupt to be triggered.
    * This results in the function appearing unresponsive if the ADXL345_INACTIVITY_THRESHOLD register
    * is less than the time constant of the output data rate.
    * A value of zero results in an interrupt when the output data is less than the value
    * in the ADXL345_INACTIVITY_THRESHOLD register.
    */
    bool set_time_inactive(uint8_t seconds);
    /*
    * Returns the configuration in the ADXL345_AXIS_EN_CTRL_ACT_INA register.
    * A setting of zero selects dc-coupled operation, and a setting of 1 enables ac-coupled
    * operation.
    * In dc-coupled operation, the current acceleration magnitude is compared directly with
    * _active_threshold and _inactive_threshold to determine whether activity or inactivity is
    * detected.
    * In ac-coupled operation, the acceleration value at the start is taken as a reference value.
    * New samples of acceleration are then compared to this reference value, and if the magnitude
    * of the difference exceeds or the _active_threshold or the _inactive_threshold, the device
    * triggers an activity or an inactivity interrupt.
    */
    uint8_t get_active_inactive_ctrl(void);
    /*
    * Set the configuration in the ADXL345_AXIS_EN_CTRL_ACT_INA register.
    * Bit | Function
    * 7   - activity ac/dc
    * 6   - activity x-axis enable
    * 5   - activity y-axis enable
    * 4   - activity z-axis enable
    * 3   - inactivity ac/dc
    * 2   - inactivity x-axis enable
    * 1   - inactivity y-axis enable
    * 0   - inactivity z-axis enable
    *
    * A setting of a 1 enables x-, y-, z-axis participation in detecting activity or
    * inactivity. Setting zero excludes the selected axis from participation.
    * If all axes are are excluded, the function is disabled.
    * For activity detection if any axes exceeds the threshold the function is trigged.
    * For inactivity detection only if all axes are below the threshold the function is trigged.
    */
    bool set_active_inactive_ctrl(uint8_t value);
    /*
    * Set the free fall threshold that determines if a free fall event has occurred.
    * The recommended values are between 300mg and 600mg. A value of less than 63mg
    * may result in undesired behavior if the free fall interrupt is enabled.
    */
    bool set_free_fall_threshold(uint16_t milli_gs);
    /*
    * Set the minimum time that the values of all axes must be less than the
    * free fall threshold to generate a free fall interruption.
    * Values between 100ms and 350ms. Values less than 5ms may result in undesired
    * behavior if the free fall interrupt is enabled.
    */
    bool set_free_tall_time(uint16_t milliseconds);
    /*
    * Returns the configuration of axes able to participate in the tap detection.
    */
    uint8_t get_axes_tap_ctrl(void);
    /*
    * Set the configuration of axes able to participate in the tap detection.
    * Bit | Function
    * 7   - 0
    * 6   - 0
    * 5   - 0
    * 4   - 0
    * 3   - suppress
    * 2   - tap x-axis enable
    * 1   - tap y-axis enable
    * 0   - tap z-axis enable
    *
    * Setting the suppress bit suppresses double tap detection if acceleration
    * greater than the value in _threshold_tap is present between taps.
    * A setting of 1 in the tap-x, tap-y or tap-z enables x-, y-, or z-axis
    * participation in tap detection and zero exclude the respective axis.
    */
    bool set_axes_tap_ctrl(uint8_t value);
    /*
    * Returns the bits involved in a tap or activity event. A setting of 1
    * corresponds to involvement in the event, and a settinf of 0 corresponds to
    * no involvement.
    * When new data is available, these bits are not cleared but are overwritten by
    * the new data. This function should be called before clearing the interrupt.
    * Disabling and axis from participation clears the corresponding source bit
    * when the next activity single/double tap event occurs.
    * Bit | Function
    * 7   - 0
    * 6   - activity x-axis source
    * 5   - activity y-axis source
    * 4   - activity z-axis source
    * 3   - asleep
    * 2   - tap x-axis source
    * 1   - tap y-axis source
    * 0   - tap z-axis source
    *
    * A setting of one in the asleep bit indicates that the part is asleep,
    * and a settinf of a zero that the part is not asleep. This bit toggles only
    * if the device is configured for auto sleep.
    */
    uint8_t get_tap_source(void);
    /*
    * Returns the configuration of the power operation and bit rate of the device.
    */
    uint8_t get_data_rt_power_ctrl(void);
    /*
    * Set the configuration of the power operation and bit rate of the device.
    * Bit | Function
    * 7   - 0
    * 6   - 0
    * 5   - 0
    * 4   - low power
    * 3   - rate
    * 2   - rate
    * 1   - rate
    * 0   - rate
    *
    * A setting of zero in the low power bit selects normal operation, and a
    * set of one reduces power operation (but has higher noise).
    * Rate Code | Output Data Rate (Hz) | Bandwidth (Hz) | Idd (uA)
    * 1111      | 3200                  | 1600           | 140
    * 1110      | 1600                  | 800            | 90
    * 1101      | 800                   | 400            | 140
    * 1100      | 400                   | 200            | 140
    * 1011      | 200                   | 100            | 140
    * 1010      | 100                   | 50             | 140
    * 1001      | 50                    | 25             | 90
    * 1000      | 25                    | 12.5           | 60
    * 0111      | 12.5                  | 6.25           | 50
    * 0110      | 6.25                  | 3.13           | 45
    * 0101      | 3.13                  | 1.56           | 40
    * 0100      | 1.56                  | 0.78           | 34
    * 0011      | 0.78                  | 0.39           | 23
    * 0010      | 0.39                  | 0.20           | 23
    * 0001      | 0.20                  | 0.10           | 23
    * 0000      | 0.10                  | 0.05           | 23
    *
    * The default is 0x0A, wich translates to a 100Hz output data rate.
    * An output data rate should be selected that is appropriate for the
    * communication protocol and frequency selected.
    * Selectin too high of an output data rate with a low communication speed
    * results in samples being discarded.
    */
    bool set_data_rt_power_ctrl(uint8_t value);
    /*
    * Returns the power configuration of the device.
    */
    uint8_t get_power_ctrl(void);
    /*
    * Set the power configuration of the device.
    * Bit | Function
    * 7   - 0
    * 6   - 0
    * 5   - link
    * 4   - auto-sleep
    * 3   - measure
    * 2   - sleep
    * 1   - wakeup
    * 0   - wakeup
    *
    * A setting of one in the link bit with both activity and inactivity
    * function enabled, delays the start of the activity function until
    * inactivity is detected. After activity is detected, inactivity detection
    * begins, preventing the detection of activity.
    * When this bit is set to zero, the inactivity and activity functions are
    * concurrent.
    *
    * If the link bit is set, a setting of one in the auto-sleep bit enables
    * auto-sleep functionality. Switch to sleep mode if inactivity is detected
    * and wakes up if activity is detected.
    * Setting zero disables automatic sleep mode.
    *
    * Setting 0 in the measure bit puts the device into standby mode, and a 1
    * places the part into measurement mode.
    *
    * Zero in the Sleep bit puts the device in normal mode and one into sleep
    * mode. Sleep mode suppress ADXL345_DATA_READY interruption and stops
    * transmitting data to FIFO, the sampling rate in this mode is specified by
    * the wakeup bits.
    *
    * Wakeup Bits | Frequency (Hz)
    * 0   0       | 8
    * 0   1       | 4
    * 1   0       | 2
    * 1   1       | 1
    *
    * When clearing the link, auto-sleep or the sleep, is recommended that the
    * device be placed in standby mode and then set back to measurement mode
    * with a subsequent write.
    */
    bool set_power_ctrl(uint8_t value);
    /*
    * Returns the interruptions enabled in the device.
    */
    uint8_t get_interrupt_enable_ctrl(void);
    /*
    * Enable or disable interruptions to occur
    *
    * Bit | Interruption
    * 7   - data ready
    * 6   - single tap
    * 5   - double tap
    * 4   - activity
    * 3   - inactivity
    * 2   - free fall
    * 1   - watermark
    * 0   - overrun
    *
    * Setting 1 enable the interruption and 0 disables.
    * The data ready, watermark and overrun bits always set the
    * ADXL345_INTERRUPT_SOURCE register, this register just configures wich
    * function will set the output pin (INT1 or INT2).
    * It's recommended that interruptions be configured before enabling their
    * output.
    */
    bool set_interrupt_enable_ctrl(uint8_t value);
    /*
    * Returns the mapping of the interruptions to INT1 or to INT2 pins.
    */
    uint8_t get_interrupt_map_pin_ctrl(void);
    /*
    * Map the interruption to pin INT1 or pin INT2.
    *
    * Bit | Interruption
    * 7   - data ready
    * 6   - single tap
    * 5   - double tap
    * 4   - activity
    * 3   - inactivity
    * 2   - free fall
    * 1   - watermark
    * 0   - overrun
    *
    * Setting 0 send the interruption to pin INT1 and. And 1 sends to pin INT2.
    */
    bool set_interrupt_map_pin_ctrl(uint8_t value);
    /*
    * Returns what function caused the interruption.
    * 1 indicates that the function have triggered the interruption and 0 that
    * don't.
    *
    * Bit | Interruption
    * 7   - data ready
    * 6   - single tap
    * 5   - double tap
    * 4   - activity
    * 3   - inactivity
    * 2   - free fall
    * 1   - watermark
    * 0   - overrun
    *
    * Data ready and watermark bits may require multiple reads. Other bits, are
    * cleared by calling this function (reading ADXL345_INTERRUPT_SOURCE).
    */
    uint8_t get_interrupt_source(void);
    /*
    * Returns the data format configuration.
    */
    uint8_t get_data_format(void);
    /*
    * Set the configuration of the data present to the registers where the x, y
    * and z values are stored. All data, except that for the +/-16g range, must
    * be clipped to avoid rollover.
    *
    * Bit | Function
    * 7   - self test
    * 6   - spi
    * 5   - interrupt invert
    * 4   - 0
    * 3   - full resolution
    * 2   - justify
    * 1   - range
    * 0   - range
    *
    * A setting of 1 in the self test bit applies a self-test force to the sensor,
    * causing a shift in the output data. A value of zero disables the self-test
    * force.
    *
    * SPI = 1 sets the device to 3-wire SPI mode, and SPI=0 to 4-wire SPI mode.
    *
    * interrupt invert = 0 set the interrupts to active high, and a value of 1 to
    * active low.
    *
    * full resolution = 1 the device is in full resolution if 0 the device is in
    * 10- bit mode, and the range bits determine the maximum g range and scale
    * factor.
    *
    * justify = 1 left-justified (MSB) mode, and 0 selects right-justified mode
    * with sign extension.
    *
    * range bits | g Range
    * 0   0      - +/- 2g
    * 0   1      - +/- 4g
    * 1   0      - +/- 8g
    * 1   1      - +/- 16g
    */
    bool set_data_format(uint8_t value);
    /*
    * Returns the value of the x-axis
    */
    float get_x_data(void);
    /*
    * Returns the value of the y-axis
    */
    float get_y_data(void);
    /*
    * Returns the value of the z-axis
    */
    float get_z_data(void);
    /*
    * Returns the configuration of the FIFO
    */
    uint8_t get_fifo_ctrl(void);
    /*
    * Configures the FIFO modes.
    *
    * Bit | Function
    * 7   - fifo mode
    * 6   - fifo mode
    * 5   - trigger
    * 4   - samples
    * 3   - samples
    * 2   - samples
    * 1   - samples
    * 0   - samples
    *
    * Fifo modes |    Mode    | Function
    * 0   0      |   Bypass   | Fifo is bypassed.
    * 0   1      |    FIFO    | Stop collection when the Fifo is full.
    * 1   0      |   Stream   | Overwrite oldest values once full.
    * 1   1      |  Trigger   | Set interruption once the FiFo collects a number
    *                         | of samples predefined in the sample bits.
    *
    * When the Fifo is in the trigger mode the trigger bit maps the event to the
    * INT1 pin if is set to 0, and to INT2 pin if set to one.
    *
    * The samples determine how many samples are needed to trigger a watermark
    * interruption if the Fifo is in FIFO or Stream mode. And specifies how many
    * FIFO samples are retained in the FIFO buffer before a trigger event.
    */
    bool set_fifo_ctrl(uint8_t value);
    /*
    * Returns the FIFO status
    * Bit | Function
    * 7   - FIFO trigger
    * 6   - 0
    * 5   - entries
    * 4   - entries
    * 3   - entries
    * 2   - entries
    * 1   - entries
    * 0   - entries
    *
    * FIFO trigger = 1 a trigger event has occurred, 0 that not.
    *
    * The entries bits  specifies how many samples there are in FIFO,
    * they must be read in bursts because FIFO is cleared after any read.
    */
    uint8_t get_fifo_status(void);

    void get_raw_data(void);

  private:
    uint8_t _id, _power_ctrl, _fifo_ctrl, _axes_tap_ctrl;
    uint8_t _offset_x, _offset_y, _offset_z;
    uint16_t _threshold_tap, _tap_duration;
    uint8_t _active_inactive_ctrl, _interrupt_enable_ctrl, _interrupt_map_pin_ctrl;
    uint8_t _data_rt_power_ctrl, _data_format;
    uint16_t _active_threshold, _inactive_threshold, _tap_latency, _tap_window;
    uint8_t _free_fall_threshold, _free_fall_time, _time_inactive;

    float _gx, _gy, _gz, _scale_factor;

    I2C &_i2c;

    bool writeRegister(uint8_t address, uint8_t data);
    bool readRegister(uint8_t address, uint8_t *data, uint8_t length = 1);
};
