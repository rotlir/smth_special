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

#include "USBAPI.h"
#include "crgb.h"
#include "Arduino.h"
#pragma once

#include <avr/pgmspace.h>
#include <FastLED.h>
#include "sprites.h"
#include "led_helpers.h"
#include "settings.h"

// interface for effects
typedef struct {
  int wait; //= 0;
  void (*start)(void *);    // setup function
  bool (*update)(void *);   // update function (should return true if the effect is finished; otherwise false)
  bool initialized; //= false;
  void *ctx;                // context of the effect
} Effect;

typedef struct {
  bool *initialized;
  int *wait;
  Sprite **sprites;
  int num;
  CRGB color; //= CRGB::Blue
  bool gap;
  int i; //= 1;
  int j; //= -1;
} RunningLineDescriptor;

typedef struct {
  bool *initialized;
  int *wait;
  int time;
  int direction;  // -1 -- fade out; 1 -- fade in
  int i;
} FadeDescriptor;

typedef struct {
  bool *initialized;
  int *wait;
  // define coordinates for all squares except the one in the upper left corner
  // (the coordinate of its upper left corner does not change)
  int x2; //= 7;
  int y3; //= 7;
  int x4; //= 7;
  int y4; //= 7;
  int i;  //= 1;
} SquareTransitionDescriptor;

typedef struct {
  bool *initialized;
  int *wait;
  Sprite *sprite; //= &HEART;
  int i; //= 0;
  int direction; //= -1; // -1/1
} FillTransitionDescriptor;

typedef struct {
  bool *initialized;
  int *wait;
  unsigned del;
  unsigned long now;
} DelayDescriptor;

typedef struct {
  bool *initialized;
  int *wait;
  int num;
  Sprite **frames;
  CRGB *color_map; //= NULL;
  CRGB color; //= CRGB::Blue;
  int i; //= 0;
} AnimationDescriptor;

typedef struct {
  bool *initialized;
  int *wait;
  Sprite *sprite;
  CRGB color; //= CRGB::Blue
  bool render; //= false;
  CRGB *color_map; //= NULL;
} SpriteDescriptor;

typedef struct {
  int index; //= 0;
  int num;
  Effect **effects;
} Animation;

// returns time in ms you should wait after each tick of the animation
int play_animation(Animation *anim) {
  Effect *effect = anim->effects[anim->index];

  if (!effect->initialized) {
    effect->start(effect->ctx);
    return 0;
  }
  bool res = effect->update(effect->ctx);

  if (res) {
    if (anim->index < anim->num - 1) {
      anim->index++;
    } else {
      anim->index = 0;
    }
  }

  return (res) ? 0 : effect->wait;
}

void draw_sprite_start(void *ctx) {
  SpriteDescriptor *desc = (SpriteDescriptor *)ctx;
  *desc->initialized = true;
  if (desc->color == 0) {
    desc->color = CRGB::Blue;
  }
}

bool draw_sprite_update(void *ctx) {
  SpriteDescriptor *desc = (SpriteDescriptor *)ctx;
  draw_sprite(*desc->sprite, desc->color, desc->render, desc->color_map);
  *desc->initialized = false;
  return true;
}

void fill_transition_start(void *ctx) {
  FillTransitionDescriptor *desc = (FillTransitionDescriptor *)ctx;
  *desc->initialized = true;
  *desc->wait = 3;
  desc->direction = -1;
}

bool fill_transition_update(void *ctx) {
  FillTransitionDescriptor *desc = (FillTransitionDescriptor *)ctx;

  if (desc->direction < 0) {
    *desc->wait = 3;
    if (desc->i < LED_NUM) {
      if (desc->i % 2 == 0) {
        leds[desc->i] = CRGB::Green;
      } else {
        leds[desc->i] = CRGB::OrangeRed;
      }
      FastLED.show();
      desc->i++;
      return false;
    }
    desc->i = LED_NUM - 1;
    desc->direction = 1;
    *desc->wait = 2;
  }

  int coords[2];
  if (desc->i >= 0) {
    led_to_XY(desc->i+1, coords);
    uint8_t v = pgm_read_byte(&(*desc->sprite)[coords[1]][coords[0]]);
    if (v) {
      leds[desc->i] = CRGB::Red;
    } else {
      leds[desc->i] = CRGB::Black;
    }

    FastLED.show();
    desc->i--;
    return false;
  }
  *desc->initialized = false;
  return true;
}

void delay_start(void *ctx) {
  DelayDescriptor *desc = (DelayDescriptor *)ctx;
  *desc->initialized = true;
  *desc->wait = 0;
  desc->now = millis();
}

bool delay_update(void *ctx) {
  DelayDescriptor *desc = (DelayDescriptor *)ctx;
  if (millis() < desc->now + desc->del) {
    return false;
  }
  *desc->initialized = false;
  return true;
}

void square_transition_start(void *ctx) {
  SquareTransitionDescriptor *desc = (SquareTransitionDescriptor *)ctx;
  *desc->initialized = true;
  desc->x2 = 7;
  desc->x4 = 7;
  desc->y3 = 7;
  desc->y4 = 7;
  desc->i = 1;
}

bool square_transition_update(void *ctx) {
  SquareTransitionDescriptor *desc = (SquareTransitionDescriptor *)ctx;

  if (desc->i <= 4) {
    draw_square(0, 0, desc->i);
    draw_square(desc->x2, 0, desc->i);
    draw_square(0, desc->y3, desc->i);
    draw_square(desc->x4, desc->y4, desc->i);
    FastLED.show();
    desc->i++;
    desc->x2--;
    desc->y3--;
    desc->x4--;
    desc->y4--;
    return false;
  }
  *desc->initialized = false;
  return true;
}

void fade_start(void *ctx) {
  FadeDescriptor *desc = (FadeDescriptor *)ctx;
  *desc->initialized = true;
  if (desc->direction < 0) {
    desc->i = 5;
  } else {
    desc->i = 0;
  }
  *desc->wait = desc->time / 5;
}

bool fade_update(void *ctx) {
  FadeDescriptor *desc = (FadeDescriptor *)ctx;

  if (desc->direction > 0) {
    if (desc->i <= 5) {
      FastLED.setBrightness(desc->i);
      FastLED.show();
      desc->i++;
      return false;
    }
  } else {
    if (desc->i >= 0) {
      FastLED.setBrightness(desc->i);
      FastLED.show();
      desc->i--;
      return false;
    }
  }
  *desc->initialized = false;
  return true;
}

void running_line_start(void *ctx) {
  RunningLineDescriptor *desc = (RunningLineDescriptor *)ctx;
  *desc->initialized = true;
  desc->i = 1;
  desc->j = -1;
  if (desc->color == 0) {
    desc->color = CRGB::Blue;
  }
  draw_sprite(*(desc->sprites)[0], desc->color, false);
}

bool running_line_update(void *ctx) {
  RunningLineDescriptor *desc = (RunningLineDescriptor *)ctx;

  if (desc->i < desc->num) {
    if (desc->j < LED_COLUMNS) {
      shift_image(false);
      if (desc->j == -1 && desc->gap) {
        desc->j++;
        FastLED.show();
        return false;
      }
      for (int y = 0; y < LED_ROWS; y++) {
        uint8_t pix = pgm_read_byte(&(*(desc->sprites)[desc->i])[y][desc->j]);
        if (pix) {
          leds[XY1(LED_COLUMNS-1, y)] = desc->color;
        } else {
          leds[XY1(LED_COLUMNS-1, y)] = CRGB::Black;
        }
      }
      FastLED.show();
      desc->j++;
      if (desc->j < LED_COLUMNS) return false;
    }
    desc->j = -1;
    desc->i++;
    return false;
  }
  *desc->initialized = false;
  return true;
}

void animation_start(void *ctx) {
  AnimationDescriptor *desc = (AnimationDescriptor *)ctx;
  FastLED.setBrightness(5);
  *desc->initialized = true;
  if (desc->color == 0) {
    desc->color = CRGB::Blue;
  }
  desc->i = 0;
}

bool animation_update(void *ctx) {
  AnimationDescriptor *desc = (AnimationDescriptor *)ctx;
  if (desc->i < desc->num) {
    draw_sprite(*desc->frames[desc->i], desc->color, true, desc->color_map);
    desc->i++;
    return false;
  }
  *desc->initialized = false;
  return true;
}
