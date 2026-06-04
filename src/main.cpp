#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"
#include "drivers/leds/leds.h"
#include "drivers/pin_def.h"

int main()
{
    stdio_init_all();
    leds_init(LED_PIO, LED_SM, LED_PIN);

    for (;;)
    {
        leds_set_single(0, 255, 255, 255);
        leds_query_status();
        leds_update_all();
        leds_query_status();

        leds_set_single(0, 10, 10, 10);
        leds_set_single(5, 5, 10, 10);
        leds_set_single(10, 10, 10, 10);
        leds_query_status();
        leds_update_all();
        leds_query_status();

        // uint32_t led_data[1];

        // // Turn on the all LEDs to be a certain colour
        // uint8_t red = 10;
        // uint8_t green = 0;
        // uint8_t blue = 10;
        // led_data[0] = (red << 24) | (green << 16) | (blue << 8);

        // int i;
        // for (i = 0; i < 12; i++)
        // {
        //     pio_sm_put_blocking(LED_PIO, LED_SM, led_data[0]);
        // }

        // sleep_ms(500);

        // // Set all LEDs off
        // uint8_t red2 = 0;
        // uint8_t green2 = 0;
        // uint8_t blue2 = 0;
        // led_data[0] = (red2 << 24) | (green2 << 16) | (blue2 << 8);
        // // led_data[0] = 0; // turning off LED

        // for (i = 0; i < 12; i++)
        // {
        //     pio_sm_put_blocking(LED_PIO, LED_SM, led_data[0]);
        // }
        sleep_ms(500);
    }

    return 0;
}
