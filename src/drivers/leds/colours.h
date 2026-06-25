#pragma once

/// TODO: Find a more elegant way to store RGB colours and names

/// rgb colour representation that stores a colour as 3 seperate r, g, b values
struct rgb_colour
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

/// Matches numbers to colours aligning with rgb_colour_table indexes so indexes can be referred to as the colour they represent. 
enum LedColour
{
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    BLUE,
    PURPLE,
    PINK,
    WHITE,
    OFF
};

/// Table of various colours in rgb
/// Includes RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, PINK, WHITE, OFF
static const rgb_colour rgb_colour_table[] =
    {
        {10, 0, 0},   // Red
        {10, 3, 0},   // Orange
        {10, 10, 0},  // Yellow
        {0, 10, 0},   // Green
        {0, 0, 10},   // Blue
        {5, 0, 10},   // Purple
        {10, 0, 5},   // Pink
        {10, 10, 10}, // White
        {0, 0, 0}     // Off
};

/// Table of various colour names
/// Includes RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, PINK, WHITE, OFF
static const char *colour_names[] =
{
    "RED",
    "ORANGE",
    "YELLOW",
    "GREEN",
    "BLUE",
    "PURPLE",
    "PINK",
    "WHITE",
    "OFF"
};

const LedColour rainbow[7] =
{
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    BLUE,
    PURPLE,
    PINK
};