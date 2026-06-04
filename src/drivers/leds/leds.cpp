// LED control

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "leds.h"
#include "drivers/logging/logging.h"
#include "drivers/pin_def.h"

// leds_init will set these static variables from global consts defined in main.cpp
// avoiding having to repeat these variables in arguments for other functions in leds.cpp
static PIO led_pio;
static uint led_sm;
static uint led_pin;
static uint32_t leds_current_data[MAX_NUM_LED]; // Colours to be set to LEDs
static uint32_t led_current_data[MAX_NUM_LED];  // Colours currently on LEDs
static bool is_leds_updated;

void leds_init(PIO pio, uint sm, uint pin)
{
    led_pio = pio;
    led_sm = sm;
    led_pin = pin;

    uint pio_program_offset = pio_add_program(led_pio, &ws2812_program);
    ws2812_program_init(led_pio, led_sm, pio_program_offset, led_pin, 800000, false);

    leds_clear_all();
    is_leds_updated = true;
}
void leds_clear_all()
{
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;

    for (uint i = 0; i < MAX_NUM_LED; i++)
    {
        leds_current_data[i] = (red << 24) | (green << 16) | (blue << 8);
        pio_sm_put_blocking(led_pio, led_sm, leds_current_data[i]);
        led_current_data[i] = (red << 24) | (green << 16) | (blue << 8);
    }

    sleep_ms(1);
}
}
}
