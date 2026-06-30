#pragma once

// Initialise ADC and clock for the microphone
void init_mic();

/**
 * @brief Read a number of samples from FIFO buffer of the microphone 
 * @param data Pointer to a buffer into which the sample data will be placed
 * @param number_of_samples Number of samples to be read
 */
void read_mic(uint16_t *data, size_t number_of_samples);