#pragma once 

/// Turn on all LEDs to one colour 
// void leds_turn_on_all();

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
* @param red Red value. Set from 0 - 255
* @param green Green value. Set from 0 - 255
* @param blue Blue value. Set from 0 - 255
*/
void set_single_led(uint led_index, uint8_t red, uint8_t green, uint8_t blue);

// Query the status of all LEDs. If not updated, print set colours and current colours of all LEDs
void query_status_leds();

// Clear all LEDs to (0, 0, 0). Both setting and updating all LEDs
void clear_all_leds();

// Update all LEDs to set colours
void update_all_leds();