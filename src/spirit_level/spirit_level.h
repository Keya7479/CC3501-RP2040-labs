#pragma once

/**
 * @brief Run  digital spirit level to visually show when gravity is aligned along particular X and Y axises
 * 
 * Turns green when axis side is aligned, turns red otherwise
 * @param raw_data Pointer to a buffer into which the raw data will be placed
 * @param g_data Pointer to a buffer into which the converted data in g's (gravity) will be placed
 * @param allowable_error_margin Margin of error allowed by the spirit level 
 */
void run_spirit_level(int16_t *raw_data, float *g_data, float allowable_error_margin);