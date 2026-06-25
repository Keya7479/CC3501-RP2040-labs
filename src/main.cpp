#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"
#include "drivers/leds/leds.h"
#include "drivers/leds_animation/leds_animation.h"
#include "drivers/pin_def.h"
#include <stdlib.h>

#define MAX_NUM_MODES 2

volatile bool is_button_pressed = false;
volatile uint32_t last_button_press_us = 0;

enum Mode
{
    MODE_STANDBY,
    MODE_LED_ANIMATE
};

void button_callback(uint gpio, uint32_t event_mask)
{
    // check for 'button bounce'
    uint32_t current_time_us = time_us_32();
    if (current_time_us - last_button_press_us > 10000) // 10000us = 10ms
    {
        last_button_press_us = current_time_us;
        log(LogLevel::INFORMATION, "BUTTON STATUS", "pressed");
        is_button_pressed = true;
    }

    // if (event_mask & GPIO_IRQ_EDGE_RISE)
    // {
    //     printf("rising edge on GPIO %u\n", gpio);
    //     printf("___________________\n");
    // }

    // if (event_mask & GPIO_IRQ_EDGE_FALL)
    // {
    //     printf("falling edge on GPIO %u\n", gpio);
    //     printf("___________________\n");
    // }
}

int main()
{
    stdio_init_all();
    init_leds();
    gpio_init(BUTTON_PIN);
    int current_mode = MODE_STANDBY;

    for (;;)
    {
        gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_RISE, true, &button_callback);
        if (is_button_pressed)
        {
            if (current_mode < MAX_NUM_MODES - 1)
            {
                current_mode++;
            }
            else
            {
                current_mode = MODE_STANDBY;
            }

            switch (current_mode)
            {
            case 0:
                // standby
                set_all_leds(GREEN);
                update_all_leds();
                break;

            case 1:
                // LED animation
                wave_leds_rainbow(250);
                break;
            }
            is_button_pressed = false;
        }
    }

    return 0;
}
