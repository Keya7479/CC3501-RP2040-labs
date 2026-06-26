#pragma once

// Initialise the i2c bus to communicate with accelerometer (400Hz sampling rate)
// Attempt to read the WHO_AM_I register and confirm that the returned value is correct
void init_accel();

/**
 * @brief write to a register on the accelerometer
 * @param reg register to write to (address of register)
 * @param data data to write to the register
 * @return true if write was successful, false otherwise
 */
bool write_reg_accel(uint8_t reg, uint8_t data);

/**
 * @brief read a register on the accelerometer
 * @param reg register to read (address of register)
 * @param data pointer to a buffer into which the data will be placed
 * @param length number of bytes to read
 * @return true if read was successful, false otherwise
 */
bool read_accel(uint8_t reg, uint8_t *data, size_t length);