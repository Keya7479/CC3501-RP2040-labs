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
 * @brief Read a register on the accelerometer. Auto-increment is not automatically enabled
 * @param reg Register to read (address of register)
 * @param data Pointer to a buffer into which the data will be placed
 * @param length Number of bytes to read 
 * @return True if read was successful, false otherwise
 */
bool read_accel(uint8_t reg, uint8_t *data, size_t length);

/**
 * @brief Read from axis registers on the accelerometer. Auto-increment is automatically enabled
 * 
 * Read starts from specified address and auto-increments until length of bytes are read.
 * Expects to read an axis's data and will combine the low and high bytes and right-shift 
 * 
 * @param register_address Register to read (address of register), if length > 1 then auto-increment will be enabled
 * @param data Pointer to a buffer into which the data will be placed, must be at least length bytes long
 * @param number_of_axises Number of axises to read 
 * @return True if read was successful, false otherwise
 */
bool read_raw_axis_accel(uint8_t register_address, int16_t *data, size_t number_of_axises);
