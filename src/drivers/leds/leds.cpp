// LED control

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "leds.h"
#include "drivers/logging/logging.h"
#include "drivers/pin_def.h"

#define RED_TEXT "\033[31m"
#define GREEN_TEXT "\033[32m"
#define WHITE_TEXT "\033[0m"

static PIO led_pio;
static uint led_sm;
static uint led_pin;
static uint32_t leds_set_data[MAX_NUM_LED];     // Colours to be set to LEDs
static uint32_t leds_current_data[MAX_NUM_LED]; // Colours currently on LEDs
static bool is_leds_updated;

void init_leds(PIO pio, uint sm, uint pin)
{
    led_pio = pio;
    led_sm = sm;
    led_pin = pin;

    uint pio_program_offset = pio_add_program(led_pio, &ws2812_program);
    ws2812_program_init(led_pio, led_sm, pio_program_offset, led_pin, 800000, false);

    clear_all_leds();
    is_leds_updated = true;
    log(LogLevel::INFORMATION, "LED SETUP", "initiated");
}

void set_single_led(uint led_index, uint8_t red, uint8_t green, uint8_t blue)
{
    // Check for invalid index (start index = 0, hence 0-11 = 12 leds)
    if (led_index >= MAX_NUM_LED)
    {
        log(LogLevel::ERROR, "LED CONFIG", "incorrect LED index: index < total number of leds");
        return;
    }

    leds_set_data[led_index] = (red << 24) | (green << 16) | (blue << 8);
    is_leds_updated = false;
}

void clear_all_leds()
{
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;

    for (uint i = 0; i < MAX_NUM_LED; i++)
    {
        leds_set_data[i] = (red << 24) | (green << 16) | (blue << 8);
        pio_sm_put_blocking(led_pio, led_sm, leds_set_data[i]);
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
            uint set_red = (leds_set_data[i] >> 24) & 0xFF;
            uint set_green = (leds_set_data[i] >> 24) & 0xFF;
            uint set_blue = (leds_set_data[i] >> 24) & 0xFF;

            uint current_red = (leds_current_data[i] >> 24) & 0xFF;
            uint current_green = (leds_current_data[i] >> 24) & 0xFF;
            uint current_blue = (leds_current_data[i] >> 24) & 0xFF;

            // If not updated, print red
            if (set_red != current_red || set_green != current_green || set_blue != current_blue)
            {
                printf(RED_TEXT "LED %2u: Set RGB(%3u, %3u, %3u) || Current RGB(%3u, %3u, %3u)" WHITE_TEXT "\n", i,
                       set_red, set_green, set_blue, current_red, current_green, current_blue);
            }
            else
            {
                printf("LED %2u: Set RGB(%3u, %3u, %3u) || Current RGB(%3u, %3u, %3u)\n", i,
                       set_red, set_green, set_blue, current_red, current_green, current_blue);
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
        is_leds_updated = true;
    }
    sleep_ms(1);
    log(LogLevel::INFORMATION, "LED UPDATE", "colours changed");
}
