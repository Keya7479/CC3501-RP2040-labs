#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/adc.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"
#include "drivers/leds/leds.h"
#include "drivers/accelerometer/accel.h"
#include "drivers/microphone/microphone.h"
#include "spirit_level/spirit_level.h"
#include "leds_animation/leds_animation.h"
#include "drivers/hardware_def.h"

#define MAX_NUM_MODES 4
#define MIC_SAMPLE_SIZE 1024

volatile bool is_button_pressed = false;
volatile uint32_t last_button_press_us = 0;
volatile bool is_mode_updated = false;

/// TODO: move functions into other file

enum Mode
{
    MODE_STANDBY,
    MODE_LED_ANIMATE,
    MODE_SPIRIT_LEVEL,
    MODE_MIC_OUTPUT
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
    init_mic();
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
            break;

        case MODE_MIC_OUTPUT:
            print_mode_change_log("mic output mode");
            uint16_t mic_data[MIC_SAMPLE_SIZE];
            read_mic(mic_data, MIC_SAMPLE_SIZE);

            // starting min and max values, set as opposites initially
            uint16_t min_mic_value = ADC_RESOLUTION;
            uint16_t max_mic_value = 0;
            int32_t sum = 0;

            // Loop through all samples read and determine min and max values and total sum
            for (size_t i = 0; i < MIC_SAMPLE_SIZE; i++)
            {
                uint16_t current_sample = mic_data[i];
                sum += mic_data[i];

                if (current_sample < min_mic_value)
                {
                    min_mic_value = current_sample;
                }

                if (current_sample > max_mic_value)
                {
                    max_mic_value = current_sample;
                }
            }

            uint16_t mean = sum / 1024;
            uint16_t peak_to_peak = max_mic_value - min_mic_value;

            printf("Mean: %u || Peak to peak: %u\n", mean, peak_to_peak);
            sleep_ms_interuptable(1000);
            // ADC resolution = 12-bit = 4095
            // DC biases at 0.5 * 3.3V, hence expected mean to be at 0.5 * ADC reolution = 2048
            break;
        }
    }

    return 0;
}
