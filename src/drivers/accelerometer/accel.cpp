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

void init_accel()
{
    i2c_init(ACCEL_I2C_INSTANCE, 400 * 1000); // communication speed 400kHz
    gpio_set_function(ACCEL_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(ACCEL_SCL_PIN, GPIO_FUNC_I2C);

    // shouldn't need to pull up internal resistors as pcb has external pull-ups
    // gpio_pull_up(ACCEL_SDA_PIN);
    // gpio_pull_up(ACCEL_SCL_PIN);

    // read the value of the WHO_AM_I register and confirm that the returned value is correct
    uint8_t reg = ACCEL_WHO_AM_I_ADDRESS;
    uint8_t who_am_i = 0;

    i2c_write_blocking(ACCEL_I2C_INSTANCE, ACCEL_I2C_ADDRESS, &reg, 1, true);
    int bytes_read = i2c_read_blocking(ACCEL_I2C_INSTANCE, ACCEL_I2C_ADDRESS, &who_am_i, 1, false);

    if (bytes_read != 1)
    {
        log(LogLevel::ERROR, "ACCELEROMETER COMMUNICATION", "Failed to read LIS3DH WHO_AM_I register.");
    }
    else
    {
        if (who_am_i != ACCEL_WHO_AM_I)
        {
            log(LogLevel::ERROR, "ACCELEROMETER COMMUNICATION", "WHO_AM_I register returned unexpected value");
        }
        else
        {
            log(LogLevel::INFORMATION, "ACCELEROMETER SETUP", "completed initialisation");
        }
    }

    write_reg_accel(ACCEL_CTRL_REG_1, 0b01110111); // configure for 400Hz sampling rate
}


// // “reg” is the register to read
// // “data” is a pointer to a buffer into which the data will be placed
// // “length” is the number of bytes to read
// bool read_accel(uint8_t reg, uint8_t *data, size_t length)
// {
//     // tell the device which address we want to read
//     if (1 != i2c_write_blocking(ACCEL_I2C_INSTANCE, ACCEL_I2C_ADDRESS, &reg, 1, true))
//     {
//         // You need to pass the pointer to the register address because
//         // i2c_write_blocking expects a pointer to a buffer of data.
//         log(LogLevel::ERROR, "ACCELEROMETER COMMUNICATION", "Failed to select register address for reading.");
//         return false;
//     }

//     // now read the data
//     int bytes_read = i2c_read_blocking(ACCEL_I2C_INSTANCE, ACCEL_I2C_ADDRESS, data, length, false);
//     if (bytes_read != length)
//     {
//         log(LogLevel::ERROR, "ACCELEROMETER COMMUNICATION", "Failed to read data.");
//         return false;
//     }
//     return true;
// }
