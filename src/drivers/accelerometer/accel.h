#pragma once

// Initialise the i2c bus to communicate with accelerometer (400Hz sampling rate)
void init_accel();

/**
 * @brief write to a register on the accelerometer 
 * @param reg register to read (address of register)
 * @param data pointer to a buffer into which the data will be placed
 */
bool write_reg_accel(uint8_t reg, uint8_t data);
