#include <stdio.h>
#include "pico/stdlib.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/leds/leds.h"
#include "drivers/accelerometer/accel.h"
#include "drivers/hardware_def.h"
#include <stdlib.h>
#include <cmath>

/// TODO: Expand this function to work across all LEDs

void run_spirit_level(int16_t *raw_data, float *g_data, float allowable_error_margin)
{
    uint8_t POS_X = 2;
    uint8_t NEG_X = 9;
    uint8_t NEG_Y = 5;
    uint8_t POS_Y = 11;

    read_raw_axis_accel(ACCEL_OUT_X_L, raw_data, 2);
    convert_raw_data_to_g(raw_data, 2, g_data);
    printf("RAW: X-axis: %d, Y-axis: %d\n", raw_data[0], raw_data[1]);
    printf("G: X-axis: %.3f g, Y-axis: %.3f g\n", g_data[0], g_data[1]);

    clear_all_leds();

    if (fabsf(g_data[0]) < allowable_error_margin && fabsf(g_data[1]) < allowable_error_margin)
    {
        set_all_leds(GREEN);
    }
    else
    {
        if (fabsf(g_data[0]) < allowable_error_margin)
        {
            set_single_led(POS_X, GREEN);
            set_single_led(NEG_X, GREEN);
        }
        else if (g_data[0] > allowable_error_margin)
        {
            set_single_led(POS_X, RED);
        }
        else if (g_data[0] < -allowable_error_margin)
        {
            set_single_led(NEG_X, RED);
        }

        if (fabsf(g_data[1]) < allowable_error_margin)
        {
            set_single_led(POS_Y, GREEN);
            set_single_led(NEG_Y, GREEN);
        }
        else if (g_data[1] > allowable_error_margin)
        {
            set_single_led(POS_Y, RED);
        }
        else if (g_data[1] < -allowable_error_margin)
        {
            set_single_led(NEG_Y, RED);
        }
    }
    update_all_leds();
}