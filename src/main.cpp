#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"
#include "drivers/leds/leds.h"
#include "drivers/leds_animation/leds_animation.h"
#include "drivers/accelerometer/accel.h"
#include "drivers/hardware_def.h"
#include <stdlib.h>

#define MAX_NUM_MODES 3

volatile bool is_button_pressed = false;
volatile uint32_t last_button_press_us = 0;


enum Mode
{
    MODE_STANDBY,
    MODE_LED_ANIMATE,
    MODE_SPIRIT_LEVEL
};

void button_callback(uint gpio, uint32_t event_mask)
{
    // check for 'button bounce'
    uint32_t current_time_us = time_us_32();
    if (current_time_us - last_button_press_us > 10000) // 10000us = 10ms
    {
        last_button_press_us = current_time_us;
        // log(LogLevel::INFORMATION, "BUTTON STATUS", "pressed");
        is_button_pressed = true;
    }
}

int main()
{
    stdio_init_all();
    gpio_init(BUTTON_PIN);
    init_leds();
    init_accel();

    // Set to STANDBY mode
    int current_mode = MODE_STANDBY;
    log(LogLevel::INFORMATION, "MODE STATUS", "in standby mode");
    set_all_leds(GREEN);
    update_all_leds();

    for (;;)
    {
        // if button pressed, change mode
        gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_RISE, true, &button_callback);
        if (is_button_pressed)
        {
            // Update mode
            if (current_mode < MAX_NUM_MODES - 1)
            {
                current_mode++;
            }
            else
            {
                current_mode = MODE_STANDBY;
            }

            // Change to updated mode
            clear_all_leds();
            switch (current_mode)
            {
            case 0:
                // standby
                log(LogLevel::INFORMATION, "MODE STATUS", "standby mode");
                set_all_leds(GREEN);
                update_all_leds();
                break;

            case 1:
                // LED animation
                log(LogLevel::INFORMATION, "MODE STATUS", "LED animation mode");
                wave_leds_rainbow(250);
                break;

            case 2:
                // Spirit level
                log(LogLevel::INFORMATION, "MODE STATUS", "spirit level mode");
                uint8_t data[6];
                for (;;)
                {
                    read_accel(ACCEL_OUT_X_L, data, 6);
                    printf("X-axis: %d, Y-axis: %d, Z-axis: %d\n", combine_axis_data(data), combine_axis_data(data + 2), combine_axis_data(data + 4));
                    sleep_ms(1000);
                }
            }

            is_button_pressed = false;
        }
    }

    return 0;
}
