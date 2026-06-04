#include "colours.h"
#pragma once

/**
 * @brief Initialise PIO to control the LED chain. By default, all LEDs off (0, 0, 0)
 * @param pio Programmable Input/Output (PIO) block
 * @param sm State machine (sm) number within PIO block
 * @param pin LED pin
 * @param led_total_num Total number of LEDs on the board
 */
void init_leds(PIO pio, uint sm, uint pin);

/**
 * @brief Set a single LED colour (RGB)
 * @param led_index Index of desired LED to set
 * @param colour_name name of colour (RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, PINK, WHITE, OFF)
 */
void set_single_led(uint led_index, LedColour colour_name);

/**
 * @brief Set all LED colours to one colour (RGB)
 * @param colour_name name of colour (RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, PINK, WHITE, OFF)
 */
void set_all_leds(LedColour colour_name);

// Query the status of all LEDs. If not updated, print set colours and current colours of all LEDs
void query_status_leds();

// Clear all LEDs to (0, 0, 0). Both setting and updating all LEDs
void clear_all_leds();

// Update all LEDs to set colours
void update_all_leds();

// Convert colour name to rgb value
static rgb_colour colour_to_rgb(LedColour colour_name);