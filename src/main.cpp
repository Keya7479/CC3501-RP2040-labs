#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"
#include "drivers/leds/leds.h"
#include "drivers/accelerometer/accel.h"
#include "spirit_level/spirit_level.h"
#include "leds_animation/leds_animation.h"
#include "drivers/hardware_def.h"
#include <stdlib.h>
#include <cmath>

#define MAX_NUM_MODES 3

volatile bool is_button_pressed = false;
volatile uint32_t last_button_press_us = 0;
volatile bool is_mode_updated = false;

/// TODO: move functions into other file

enum Mode
{
    MODE_STANDBY,
    MODE_LED_ANIMATE,
    MODE_SPIRIT_LEVEL
};

// sleep until total_time_ms but can be interupted if button is pressed
void sleep_ms_interuptable(uint16_t total_time_ms)
{
    for (uint16_t current_time_ms = 0; current_time_ms < total_time_ms; current_time_ms += 10)
    {
        if (is_button_pressed)
        {
            return;
        }
        sleep_ms(10);
    }
    return;
}

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

uint8_t new_mode(uint8_t current_mode)
{
    // Update mode
    if (current_mode < MAX_NUM_MODES - 1)
    {
        return current_mode + 1;
    }
    else
    {
        return MODE_STANDBY;
    }
}

void print_mode_change_log(const char *mode)
{
    if (!is_mode_updated)
    {
        log(LogLevel::INFORMATION, "MODE STATUS", mode);
    }
    is_mode_updated = true;
}

int main()
{
    stdio_init_all();
    gpio_init(BUTTON_PIN);
    init_leds();
    init_accel();
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_RISE, true, &button_callback);

    // Set to STANDBY mode
    /// TODO remove this repeated code 
    uint8_t current_mode = MODE_STANDBY;
    print_mode_change_log("standby mode");
    set_all_leds(GREEN);
    update_all_leds();

    for (;;)
    {
        if (is_button_pressed)
        {
            clear_all_leds();
            current_mode = new_mode(current_mode);
            is_mode_updated = false;
            is_button_pressed = false;
        }

        // Change to updated mode
        switch (current_mode)
        {
        case MODE_STANDBY:
            print_mode_change_log("standby mode");
            set_all_leds(GREEN);
            update_all_leds();
            break;

        case MODE_LED_ANIMATE:
            print_mode_change_log("LED animation mode");
            wave_leds_rainbow(250);
            break;

        case MODE_SPIRIT_LEVEL:
            print_mode_change_log("spirit level mode");
            int16_t raw_data[2];
            float g_data[2];
            run_spirit_level(raw_data, g_data, 0.05f);
            sleep_ms_interuptable(1000);
        }
    }

    return 0;
}
