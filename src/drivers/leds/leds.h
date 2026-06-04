#pragma once 

/// Turn on all LEDs to one colour 
// void leds_turn_on_all();

/**
 * @brief Initialise PIO to control the LED chain. By default, all LEDs off (0, 0, 0).
 * @param pio Programmable Input/Output (PIO) block 
 * @param sm State machine (sm) number within PIO block
 * @param pin LED pin
 * @param led_total_num Total number of LEDs on the board
 */
void leds_init(PIO pio, uint sm, uint pin);

// Clear all LEDs to (0, 0, 0). Both setting and updating all LEDs. 
void leds_clear_all();