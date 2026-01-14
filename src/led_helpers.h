/*
Copyright 2025 rotlir

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <FastLED.h>
#include <avr/pgmspace.h>
#include "sprites.h"
#include "settings.h"

CRGB leds[LED_NUM];

void led_to_XY(int num, int *coords) {
  int x = LED_COLUMNS - num / LED_ROWS;
  int modulo = num % LED_ROWS;
  int y;
  if (modulo > 0) x--;
  if (x % 2 == 0) { // goes down
    if (modulo == 0) {
      y = LED_ROWS - 1;
    } else {
      y = modulo - 1;
    }
  } else {          // goes up
    if (modulo == 0) {
      y = 0;
    } else {
      y = LED_ROWS - modulo;
    }
  }
  coords[0] = x;
  coords[1] = y;
}

// convert coordinates beginning from the lower left corner
int XY(int x, int y) {
  int column = LED_COLUMNS - x - 1;
  if (column % 2 == 0) { // goes up
    return column * LED_ROWS + y;
  } else {               // goes down
    return column * LED_ROWS + (LED_ROWS - y) - 1;
  }
}

// convert coordinates beginning from the upper left corner
int XY1(int x, int y) {
  int column = LED_COLUMNS - x - 1;
  if (column % 2 == 0) {
    return column * LED_ROWS + (LED_ROWS - y) - 1;
  } else {
    return column * LED_ROWS + y;
  }
}

void draw_sprite(Sprite sprite, CRGB color = CRGB::Red, bool render = true, CRGB *color_map = NULL) {
  for (int y = 0; y < LED_COLUMNS; y++) {
    for (int x = 0; x < LED_ROWS; x++) {
      uint8_t v = pgm_read_byte(&sprite[y][x]);
      if (v) {
        if (color_map != NULL) {
          leds[XY1(x, y)] = color_map[v];
        } else {
          leds[XY1(x, y)] = color;
        }
      } else {
        leds[XY1(x, y)] = CRGB::Black;
      }
    }
  }
  if (render) FastLED.show();
}


// shift the image on the screen one pixel to the left
void shift_image(bool render = true) {
  for (int x = 1; x < LED_COLUMNS; x++) {
    for (int y = 0; y < LED_ROWS; y++) {
      leds[XY1(x-1, y)] = leds[XY1(x, y)];
      if (x == LED_COLUMNS - 1) {
        leds[XY1(x, y)] = CRGB::Black;
      }
      if (render) FastLED.show();
    }
  }
}

void draw_square(int x, int y, int size, CRGB color = CRGB::Green) {
  for (int i = y; i < y + size; i++) {
    for (int j = x; j < x + size; j++) {
      leds[XY1(j, i)] = color;
    }
  }
}
