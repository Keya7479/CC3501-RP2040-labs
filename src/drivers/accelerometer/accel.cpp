#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/hardware_def.h"
#include "drivers/logging/logging.h"

bool write_register_accel(uint8_t reg, uint8_t data)
{
    // need two bytes to write to i2c bus: register address and data
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = data;
    int bytes_written = i2c_write_blocking(ACCEL_I2C_INSTANCE, ACCEL_I2C_ADDRESS, buf, 2, false);
    if (bytes_written != 2)
    {
        log(LogLevel::ERROR, "ACCELEROMETER COMMUNICATION", "Failed to write to register.");
        return false;
    }
    return true;
}

bool read_register_accel(uint8_t reg, uint8_t *data, size_t length)
{
    // tell the device which address to read
    if (1 != i2c_write_blocking(ACCEL_I2C_INSTANCE, ACCEL_I2C_ADDRESS, &reg, 1, true))
    {
        log(LogLevel::ERROR, "ACCELEROMETER COMMUNICATION", "Failed to select register address for reading.");
        return false;
    }

    // read the data
    if (length != i2c_read_blocking(ACCEL_I2C_INSTANCE, ACCEL_I2C_ADDRESS, data, length, false))
    {
        log(LogLevel::ERROR, "ACCELEROMETER COMMUNICATION", "Failed to read data.");
        return false;
    }
    return true;
}

bool read_raw_axis_accel(uint8_t reg, int16_t *data, size_t number_of_axises)
{
    // enable auto-increment for multi-byte reads which will read until number_of_axises are read
    // to do this, the MSB of the register address must be set to 1
    uint8_t read_reg = reg | 0x80;                 // MSB = 1 (1000 0000)
    size_t number_of_bytes = number_of_axises * 2; // 2 bytes required per axis (high and low)
    uint8_t uncombined_data[number_of_bytes];

    if (!read_register_accel(read_reg, uncombined_data, number_of_bytes))
    {
        return false;
    }

    // combine low and high bytes into a 16-bit signed integer
    for (size_t i = 0; i < number_of_axises; i++)
    {
        data[i] = (int16_t)(uncombined_data[i * 2] | (uncombined_data[i * 2 + 1] << 8)) >> 6;
    }
    return true;
}

void init_accel()
{
    i2c_init(ACCEL_I2C_INSTANCE, 400 * 1000); // communication speed 400kHz
    gpio_set_function(ACCEL_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(ACCEL_SCL_PIN, GPIO_FUNC_I2C);

    // read the value of the WHO_AM_I register and confirm that the returned value is correct
    uint8_t who_am_i = 0;
    read_register_accel(ACCEL_WHO_AM_I_ADDRESS, &who_am_i, 1);
    if (who_am_i != ACCEL_WHO_AM_I)
    {
        log(LogLevel::ERROR, "ACCELEROMETER COMMUNICATION", "WHO_AM_I register returned unexpected value");
        return;
    }

    log(LogLevel::INFORMATION, "ACCELEROMETER SETUP", "completed initialisation");

    // configure for 400Hz sampling rate (default accel range appropriate)
    /// TODO: make sampling rate customisable 
    write_register_accel(ACCEL_CTRL_REG_1, 0b01110111);
}

void convert_raw_data_to_g(int16_t *raw_data, size_t length_of_raw_data, float *g_data)
{
    /// TODO: change conversion factor to be customisable based on accel operating mode
    float conversion_factor = 0.004f; 
    for (size_t i = 0; i < length_of_raw_data; i++)
    {
        g_data[i] = raw_data[i] * conversion_factor;
    }
}
