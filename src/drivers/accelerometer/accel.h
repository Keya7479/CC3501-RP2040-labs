#pragma once

// Initialise the i2c bus to communicate with accelerometer (400Hz sampling rate)
// Attempt to read the WHO_AM_I register and confirm that the returned value is correct
void init_accel();

/**
 * @brief Write to a register on the accelerometer
 * @param reg Register to write to (address of register)
 * @param data Data to write to the register
 * @return True if write was successful, false otherwise
 */
bool write_reg_accel(uint8_t reg, uint8_t data);

/**
 * @brief Read a register on the accelerometer. Register does not need multi-byte read enabled, if length > 1 then auto-increment will be enabled within this function
 * and will step through registers until length bytes are read
 * @param reg Register to read (address of register)
 * @param data Pointer to a buffer into which the data will be placed
 * @param length Number of bytes to read
 * @return True if read was successful, false otherwise
 */
bool read_accel(uint8_t reg, uint8_t *data, size_t length);

/**
 * @brief Combine low and high byte of axis data into a single 16-bit signed integer
 * @param data Pointer to a buffer with the low and high byte of an axis's data
 */
int16_t combine_axis_data(uint8_t *data);