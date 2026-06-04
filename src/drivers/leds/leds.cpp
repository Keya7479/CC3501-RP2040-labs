// LED control

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "leds.h"
#include "colours.h"
#include "drivers/logging/logging.h"
#include "drivers/pin_def.h"

#define RED_TEXT "\033[31m"
#define GREEN_TEXT "\033[32m"
#define WHITE_TEXT "\033[0m"

static uint32_t leds_set_data[MAX_NUM_LED];     // Colours to be set to LEDs
static uint32_t leds_current_data[MAX_NUM_LED]; // Colours currently on LEDs
static bool is_leds_updated;

void init_leds()
{
    uint pio_program_offset = pio_add_program(LED_PIO, &ws2812_program);
    ws2812_program_init(LED_PIO, LED_SM, pio_program_offset, MAX_NUM_LED, 800000, false);
    // Clear all then briefly flash green to indicate LEDs are initialised
    clear_all_leds();
    set_all_leds(GREEN);
    update_all_leds();
    sleep_ms(1000);
    clear_all_leds();
    log(LogLevel::INFORMATION, "LED SETUP", "completed initialisation");
}

void set_single_led(uint led_index, LedColour colour_name)
{
    // Check for invalid index (start index = 0, hence 0-11 = 12 leds)
    if (led_index >= MAX_NUM_LED)
    {
        log(LogLevel::ERROR, "LED CONFIG", "incorrect LED index: index < total number of leds");
        return;
    }
    rgb_colour colour = colour_to_rgb(colour_name);
    leds_set_data[led_index] = (colour.r << 24) | (colour.g << 16) | (colour.b << 8);
    is_leds_updated = false;
}

void set_all_leds(LedColour colour_name)
{
    rgb_colour colour = colour_to_rgb(colour_name);
    for (uint i = 0; i < MAX_NUM_LED; i++)
    {
        leds_set_data[i] = (colour.r << 24) | (colour.g << 16) | (colour.b << 8);
    }
    is_leds_updated = false;
}

void clear_all_leds()
{
    rgb_colour colour = colour_to_rgb(OFF);
    for (uint i = 0; i < MAX_NUM_LED; i++)
    {
        leds_set_data[i] = (colour.r << 24) | (colour.g << 16) | (colour.b << 8);
        pio_sm_put_blocking(LED_PIO, LED_SM, leds_set_data[i]);
    }
    update_all_leds();
    sleep_ms(1);
}

void query_status_leds()
{
    if (is_leds_updated == false)
    {
        log(LogLevel::INFORMATION, "LED STATUS", RED_TEXT "colours not updated " WHITE_TEXT "printing values...");
        printf("------------------------------------------------------------\n");
        for (uint i = 0; i < MAX_NUM_LED; i++)
        {
            // Unpack each colour
            uint set_r = (leds_set_data[i] >> 24) & 0xFF;
            uint set_g = (leds_set_data[i] >> 16) & 0xFF;
            uint set_b = (leds_set_data[i] >> 8) & 0xFF;

            uint current_r = (leds_current_data[i] >> 24) & 0xFF;
            uint current_g = (leds_current_data[i] >> 16) & 0xFF;
            uint current_b = (leds_current_data[i] >> 8) & 0xFF;

            const char *set_colour_name = rgb_to_colour(set_r, set_g, set_b);
            const char *current_colour_name = rgb_to_colour(current_r, current_g, current_b);

            // If not updated, print red
            if (set_r != current_r || set_g != current_g || set_b != current_b)
            {
                printf(RED_TEXT "LED %2u: Set %-9s || Current %-9s" WHITE_TEXT "\n", i, set_colour_name, current_colour_name);
            }
            else
            {
                printf("LED %2u: Set %-9s || Current %-9s\n", i, set_colour_name, current_colour_name);
            }
        }
        printf("------------------------------------------------------------\n");
    }
    else
    {
        log(LogLevel::INFORMATION, "LED STATUS", GREEN_TEXT "colours up to date" WHITE_TEXT);
    }
}

void update_all_leds()
{
    for (uint i = 0; i < MAX_NUM_LED; i++)
    {
        pio_sm_put_blocking(LED_PIO, LED_SM, leds_set_data[i]);
        leds_current_data[i] = leds_set_data[i];
    }

    sleep_ms(1);
    is_leds_updated = true;
}

static rgb_colour colour_to_rgb(LedColour colour_name)
{
    return rgb_colour_table[colour_name];
}

static const char *rgb_to_colour(uint r, uint g, uint b)
{
    for (uint i = 0; i < sizeof(rgb_colour_table) / sizeof(rgb_colour_table[0]); i++)
    {
        if (rgb_colour_table[i].r == r &&
            rgb_colour_table[i].g == g &&
            rgb_colour_table[i].b == b)
        {
            return colour_names[i];
        }
    }

    return "NAME ERROR";
}
