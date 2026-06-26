#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/hardware_def.h"
#include "drivers/logging/logging.h"

bool write_reg_accel(uint8_t reg, uint8_t data)
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

bool read_accel(uint8_t reg, uint8_t *data, size_t length)
{
    // tell the device which address to read
    if (1 != i2c_write_blocking(ACCEL_I2C_INSTANCE, ACCEL_I2C_ADDRESS, &reg, 1, true))
    {
        log(LogLevel::ERROR, "ACCELEROMETER COMMUNICATION", "Failed to select register address for reading.");
        return false;
    }

    // read the data
    int bytes_read = i2c_read_blocking(ACCEL_I2C_INSTANCE, ACCEL_I2C_ADDRESS, data, length, false);
    if (bytes_read != length)
    {
        log(LogLevel::ERROR, "ACCELEROMETER COMMUNICATION", "Failed to read data.");
        return false;
    }
    // uint8_t raw_data[2];
    // int16_t data = (int16_t)(raw_data[0] | (raw_data[1] << 8)) >> 6;
    return true;
}

void init_accel()
{
    i2c_init(ACCEL_I2C_INSTANCE, 400 * 1000); // communication speed 400kHz
    gpio_set_function(ACCEL_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(ACCEL_SCL_PIN, GPIO_FUNC_I2C);

    // shouldn't need to pull up internal resistors as pcb has external pull-ups
    // gpio_pull_up(ACCEL_SDA_PIN);
    // gpio_pull_up(ACCEL_SCL_PIN);

    // read the value of the WHO_AM_I register and confirm that the returned value is correct
    uint8_t who_am_i = 0;
    if (!read_accel(ACCEL_WHO_AM_I_ADDRESS, &who_am_i, 1))
    {
        log(LogLevel::ERROR, "ACCELEROMETER COMMUNICATION", "WHO_AM_I register returned unexpected value");
    }
    else
    {
        log(LogLevel::INFORMATION, "ACCELEROMETER SETUP", "completed initialisation");
    }

    // configure for 400Hz sampling rate (default accel range appropriate)
    write_reg_accel(ACCEL_CTRL_REG_1, 0b01110111);
}

// // accelerometer has 10-bit readings in “normal mode”, use left justified number extension to get 16-bit signed integer
// uint16_t convert_data_accel(uint8_t *raw_data)
// {
//     uint8_t raw_data[2];
//     int16_t data = (int16_t)(raw_data[0] | (raw_data[1] << 8)) >> 6;
//     return data;
// }
