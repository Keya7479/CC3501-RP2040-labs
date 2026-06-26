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
    // enable auto-increment for multi-byte reads which will read until length bytes have been read
    // to do this, the MSB of the register address must be set to 1
    uint8_t read_reg = reg;
    if (length > 1)
    {
        read_reg |= 0x80; // MSB = 1 (1000 0000) 
    }

    // tell the device which address to read
    if (1 != i2c_write_blocking(ACCEL_I2C_INSTANCE, ACCEL_I2C_ADDRESS, &read_reg, 1, true))
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

void init_accel()
{
    i2c_init(ACCEL_I2C_INSTANCE, 400 * 1000); // communication speed 400kHz
    gpio_set_function(ACCEL_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(ACCEL_SCL_PIN, GPIO_FUNC_I2C);

    // read the value of the WHO_AM_I register and confirm that the returned value is correct
    uint8_t who_am_i = 0;
    read_accel(ACCEL_WHO_AM_I_ADDRESS, &who_am_i, 1);
    if (who_am_i != ACCEL_WHO_AM_I)
    {
        log(LogLevel::ERROR, "ACCELEROMETER COMMUNICATION", "WHO_AM_I register returned unexpected value");
        return;
    }

    log(LogLevel::INFORMATION, "ACCELEROMETER SETUP", "completed initialisation");

    // configure for 400Hz sampling rate (default accel range appropriate)
    write_reg_accel(ACCEL_CTRL_REG_1, 0b01110111);
}

int16_t combine_axis_data(uint8_t *data)
{
    return (int16_t)(data[0] | (data[1] << 8)) >> 6;
}
