#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"
#include "drivers/leds/leds.h"
#include "drivers/leds/colours.h"
#include "drivers/pin_def.h"
#include <stdlib.h>

#define START_INDEX 0 // Universal start index
#define RAINBOW_END_INDEX 6
#define RAINBOW_LENGTH 7

static LedColour rainbow[RAINBOW_LENGTH] =
    {
        RED,
        ORANGE,
        YELLOW,
        GREEN,
        BLUE,
        PURPLE,
        PINK};

/**
 * @brief Flash rainbow sequence, cycles all leds at the same time through the rainbow (duration: RAINBOW_LENGTH * time_interval)
 * @param time_interval time between each colour change
 */
void flash_leds_rainbow(uint time_interval)
{
    for (uint i = START_INDEX; i <= RAINBOW_END_INDEX; i++)
    {
        set_all_leds(rainbow[i]);
        update_all_leds();
        sleep_ms(time_interval);
    }
}

/**
 * @brief Rainbow wave sequence, cycles all leds through the rainbow once (duration: RAINBOW_LENGTH * time_interval)
 * @param time_interval time between each colour change
 */
void wave_leds_rainbow(uint time_interval)
{
    for (uint offset = START_INDEX; offset <= RAINBOW_LENGTH; offset++)
    {
        for (uint led_index = START_INDEX; led_index < MAX_NUM_LED; led_index++)
        {
            uint colour = (led_index + offset) % RAINBOW_LENGTH;
            set_single_led(led_index, rainbow[colour]);
        }

        update_all_leds();
        sleep_ms(time_interval);
    }
}

void exit_wave_leds_rainbow(uint time_interval)
{
    for (uint off_count = START_INDEX; off_count <= MAX_NUM_LED; off_count++)
    {
        for (uint led_index = START_INDEX; led_index < MAX_NUM_LED; led_index++)
        {
            // Lil guy first appears, trailing rainbow
            if (led_index == off_count - 7)
            {
                set_single_led(led_index, WHITE);
            }
            // Turn off LEDs in sequence
            else if (led_index < off_count)
            {
                set_single_led(led_index, OFF);
            }
            // Continue Rainbow sequence
            else
            {
                uint colour = (led_index + off_count) % RAINBOW_LENGTH;
                set_single_led(led_index, rainbow[colour]);
            }
        }

        update_all_leds();
        sleep_ms(time_interval);
    }
}

/**
 * @brief Steps lil guy (single white LED) from current_location to target_location
 * @param current_location current location
 * @param target_location target location
 * @param time_interval time between each single led step
 */
void step_lil_guy(int current_location, int target_location, uint time_interval)
{
    int direction = target_location > current_location ? 1 : -1;
    while (current_location != target_location)
    {
        int next_location = current_location + direction;
        set_single_led(current_location, OFF);
        set_single_led(next_location, WHITE);
        update_all_leds();
        sleep_ms(time_interval);
        current_location = next_location;
    }
}

/**
 * @brief Blink lil guy (single LED) with time_interval between each flash, then waiting 2 * time_interval
 * @param current_location current location
 * @param colour current colour
 * @param blink_colour colour of off blink
 * @param num_blinks number of blinks
 * @param time_interval time between each single led step
 */
void blink_lil_guy(uint current_location, LedColour colour, LedColour blink_colour, uint num_blinks, uint time_interval)
{
    for (uint i = START_INDEX; i < num_blinks; i++)
    {
        set_single_led(current_location, blink_colour);
        update_all_leds();
        sleep_ms(time_interval);
        set_single_led(current_location, colour);
        update_all_leds();
        sleep_ms(time_interval);
    }
    sleep_ms(2 * time_interval);
}

/// Lil guy (single white LED) moves around board progressively faster
void confuse_lil_guy()
{
    step_lil_guy(4, 11, 100);
    sleep_ms(500);
    blink_lil_guy(11, WHITE, OFF, 3, 100);
    step_lil_guy(11, 0, 100);
    sleep_ms(500);
    blink_lil_guy(0, WHITE, OFF, 5, 100);
    step_lil_guy(0, 11, 95);
    sleep_ms(100);
    blink_lil_guy(11, WHITE, OFF, 3, 50);
    step_lil_guy(11, 0, 80);
    sleep_ms(50);
    blink_lil_guy(0, WHITE, OFF, 2, 50);
    step_lil_guy(0, 11, 60);
    sleep_ms(50);
    step_lil_guy(11, 0, 50);
    step_lil_guy(0, 11, 45);
    step_lil_guy(11, 0, 40);
    step_lil_guy(0, 11, 35);
    step_lil_guy(11, 0, 30);
    step_lil_guy(0, 11, 25);
}
/// Lil guy (single white LED) smashes into wall at led index 11
void crash_lil_guy()
{
    blink_lil_guy(11, WHITE, RED, 2, 50);
    set_single_led(11, OFF);
    set_single_led(10, RED);
    set_single_led(9, RED);
    update_all_leds();
    sleep_ms(100);

    set_single_led(10, OFF);
    update_all_leds();
    sleep_ms(100);
    step_lil_guy(9, 8, 100);
    blink_lil_guy(8, WHITE, RED, 5, 50);
    sleep_ms(1000);
    blink_lil_guy(8, WHITE, OFF, 2, 200);
    sleep_ms(1000);
    step_lil_guy(8, 5, 500);
    sleep_ms(1000);
}

/// Animate a lil guy (single white LED)
void animate_lil_guy()
{
    // Lil guy appears
    clear_all_leds();
    set_single_led(5, WHITE);
    update_all_leds();
    sleep_ms(2000);
    blink_lil_guy(5, WHITE, OFF, 1, 500);

    // Lil guy looks left
    step_lil_guy(5, 7, 500);
    sleep_ms(1000);
    blink_lil_guy(7, WHITE, OFF, 1, 250);

    // Lil guy looks right
    step_lil_guy(7, 4, 500);
    sleep_ms(1000);
    blink_lil_guy(4, WHITE, OFF, 2, 150);

    confuse_lil_guy();
    crash_lil_guy();

    // But lil guy is all alone :(
    sleep_ms(1000);
    set_single_led(5, BLUE);
    update_all_leds();
    sleep_ms(5000);

    // Lil guy leaves
    blink_lil_guy(5, WHITE, BLUE, 3, 500);
    sleep_ms(500);
    step_lil_guy(5, 0, 500);
    sleep_ms(1000);
    blink_lil_guy(0, WHITE, OFF, 2, 500);
    sleep_ms(1000);
    step_lil_guy(0, 1, 500);
    sleep_ms(1000);
    blink_lil_guy(1, WHITE, OFF, 1, 500);
    sleep_ms(1000);
    set_single_led(1, BLUE);
    update_all_leds();
    sleep_ms(5000);
    blink_lil_guy(1, WHITE, BLUE, 3, 500);
    sleep_ms(500);
    set_single_led(1, WHITE);
    update_all_leds();
    sleep_ms(500);
    step_lil_guy(1, 0, 500);
    clear_all_leds();
    sleep_ms(5000);
}

int main()
{
    stdio_init_all();
    init_leds();

    for (;;)
    {
        flash_leds_rainbow(250);
        wave_leds_rainbow(250);
        exit_wave_leds_rainbow(250);
        animate_lil_guy();
    }

    return 0;
}
