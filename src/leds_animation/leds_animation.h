#include "drivers/leds/colours.h"
#pragma once

/// TODO: Clean this up, fix animation

/**
 * @brief Flash rainbow sequence, cycles all leds at the same time through the rainbow (duration: RAINBOW_LENGTH * time_interval)
 * @param time_interval time between each colour change
 */
void flash_leds_rainbow(uint time_interval);

/**
 * @brief Rainbow wave sequence, cycles all leds through the rainbow once (duration: RAINBOW_LENGTH * time_interval)
 * @param time_interval time between each colour change
 */
void wave_leds_rainbow(uint time_interval);

/**
 * @brief Exit rainbow wave sequence, with lil guy (single white LED) following behind
 * @param time_interval time between each colour change
 */
void exit_wave_leds_rainbow(uint time_interval);

/**
 * @brief Steps lil guy (single white LED) from current_location to target_location
 * @param current_location current location
 * @param target_location target location
 * @param time_interval time between each single led step
 */
void step_lil_guy(int current_location, int target_location, uint time_interval);

/**
 * @brief Blink lil guy (single LED) with time_interval between each flash, then waiting 2 * time_interval
 * @param current_location current location
 * @param colour current colour
 * @param blink_colour colour of off blink
 * @param num_blinks number of blinks
 * @param time_interval time between each blink
 */
void blink_lil_guy(uint current_location, LedColour colour, LedColour blink_colour, uint num_blinks, uint time_interval);

/// Lil guy (single white LED) moves around board progressively faster
void confuse_lil_guy();

/// Exit sequence for lil guy (single white led)
void exit_lil_guy();

/// Lil guy (single white LED) smashes into wall at led index 11
void crash_lil_guy();

/// Animate a lil guy (single white LED)
void animate_lil_guy();