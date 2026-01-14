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

#include "settings.h"
#pragma once

#include <FastLED.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>

#include "sprites.h"
#include "led_helpers.h"

uint16_t counter;

void counter_read() {
  EEPROM.get(0, counter);
}

void counter_update() {
  counter++;
  if (counter >= 1700) counter = 0;
  EEPROM.put(0, counter);
}

void counter_screen() {
  fl::fill_solid(leds, LED_NUM, CRGB::Black);
  const Symbol *digits[10] = {&ZERO, &ONE, &TWO, &THREE, &FOUR, &FIVE, &SIX, &SEVEN, &EIGHT, &NINE};

  uint16_t pixels = counter / 100;

  uint8_t first_digit = (counter % 100) / 10;
  uint8_t second_digit = (counter % 100) % 10;

  const Symbol *digit_1 = digits[first_digit];
  const Symbol *digit_2 = digits[second_digit];
  for (int y = 0; y <= 4; y++) {
    for (int x = 0; x <= 2; x++) {
      uint8_t pix_1 = pgm_read_byte(&(*digit_1)[y][x]);
      if (pix_1) leds[XY1(x,y)] = CRGB::Red;

      uint8_t pix_2 = pgm_read_byte(&(*digit_2)[y][x]);
      if (pix_2) leds[XY1(x + 5, y)] = CRGB::Red; // shift the digit 5 pixels right
    }
  }

  uint8_t filled_rows = pixels / LED_COLUMNS;
  uint8_t pix_in_last_row = pixels % LED_COLUMNS;
  uint8_t last_row = LED_ROWS - 2 + filled_rows;

  for (int y = LED_ROWS - 2; y < last_row; y++) {
    for (int x = 0; x <= LED_COLUMNS - 1; x++) {
      leds[XY1(x,y)] = CRGB::DeepPink;
    }
  }

  for (int x = 0; x < pix_in_last_row; x++) {
    leds[XY1(x, last_row)] = CRGB::DeepPink;
  }

  FastLED.show();
}

void counter_update_animation() {
  for (int i = 5; i >= 0; i--) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(50);
  }

  draw_sprite(HEART);

  for (int i = 1; i <= 5; i++) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(50);
  }

  delay(450);

  for (int i = 5; i >= 0; i--) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(50);
  }

  FastLED.setBrightness(5);
}
