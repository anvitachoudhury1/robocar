#include <Arduino.h>
#include "config.h"
#include "heart_control.h"
#include "network.h"
#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

uint8_t bigHeart[8][12] = {
    {0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0},
    {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0}};
// Blank frame (for blink effect)
uint8_t blank[8][12] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

static bool heartState = false;
void heartInit()
{
    matrix.begin();

    heartState = false;
}
// Turn the onboard LED on/off (updates the persistent state).
void heartOn()
{
    matrix.renderBitmap(bigHeart, 8, 12);
    heartState = true;
}

void heartOff()
{
    matrix.renderBitmap(blank, 8, 12);
    heartState = false;
}

// Current persistent LED state (the state blink restores to).
bool heartIsOn() { return heartState; }
