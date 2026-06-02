#pragma once 

/// Turn on all LEDs to one colour 
// void leds_turn_on_all();

/**
 * @brief Initialise PIO to control the LED chain
 * @param pio Programmable Input/Output (PIO) block 
 * @param sm State machine (sm) number within PIO block
 * @param pin LED pin
 * @param led_total_num Total number of LEDs on the board
 */
void leds_init(PIO pio, uint sm, uint pin, uint led_total_num);

