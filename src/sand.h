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

#include "fl/fill.h"
#pragma once

#include <FastLED.h>
#include "settings.h"
#include "led_helpers.h"
#include "effects.h"
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <math.h>
#include <limits.h>
#include <EEPROM.h>

MPU6050 mpu;

uint8_t mpuFifoBuf[45];

typedef struct {
  int delay_x;
  int delay_y;
  int dir_x;
  int dir_y;
} SandContext;

typedef struct {
  int16_t acc_x_offset, acc_y_offset, acc_z_offset;
  int16_t gyro_x_offset, gyro_y_offset, gyro_z_offset;
} CalibrationOffsets;

void mpu_init() {
  mpu.initialize();
  mpu.dmpInitialize();
  mpu.setDMPEnabled(true);

  CalibrationOffsets offsets;
  EEPROM.get(2, offsets);
  mpu.setXAccelOffset(offsets.acc_x_offset);
  mpu.setYAccelOffset(offsets.acc_y_offset);
  mpu.setZAccelOffset(offsets.acc_z_offset);
  mpu.setXGyroOffset(offsets.gyro_x_offset);
  mpu.setYGyroOffset(offsets.gyro_y_offset);
  mpu.setZGyroOffset(offsets.gyro_z_offset);
}

void mpu_calibrate() {
  mpu.CalibrateAccel();
  mpu.CalibrateGyro();
  CalibrationOffsets offsets;
  offsets.gyro_x_offset = mpu.getXGyroOffset();
  offsets.gyro_y_offset = mpu.getYGyroOffset();
  offsets.gyro_z_offset = mpu.getZGyroOffset();
  offsets.acc_x_offset = mpu.getXAccelOffset();
  offsets.acc_y_offset = mpu.getYAccelOffset();
  offsets.acc_z_offset = mpu.getZAccelOffset();
  EEPROM.put(2, offsets);
}

void sand_init() {
  fl::fill_solid(leds, LED_NUM, CRGB::Black);

  // draw initial sand particles
  draw_square((LED_COLUMNS - 4) / 2, (LED_ROWS - 4) / 2, 4, CRGB::Yellow);
}

// make a step of the cellular automaton over axis Y
// positive direction -- step down; negative direction -- step up
void sand_step_y(int direction) {
  if (direction == 0) return;
  if (direction > 0) {
    // iterate over cells downside up and from right to left
    // we don't care about the lowest row as the cells there are not going anywhere
    for (int y = LED_ROWS - 2; y >= 0; y--) {
      for (int x = LED_COLUMNS - 1; x >= 0; x--) {
        CRGB *cell = &leds[XY1(x,y)];
        if (*cell) {
          CRGB *bottom_cell = &leds[XY1(x,y+1)];
          if (!(*bottom_cell)) {
            *bottom_cell = *cell;
            *cell = 0;
          } else {
            if (x != 0) {
              CRGB *bottom_left_cell = &leds[XY1(x-1,y+1)];
              if (!(*bottom_left_cell)) {
                *bottom_left_cell = *cell;
                *cell = 0;
                continue;
              }
            }
            if (x != LED_COLUMNS - 1) {
              CRGB *bottom_right_cell = &leds[XY1(x+1,y+1)];
              if (!(*bottom_right_cell)) {
                *bottom_right_cell = *cell;
                *cell = 0;
              }
            }
          }
        }
      }
    }
  } else {
    // iterate over cells upside down and from right to left
    for (int y = 1; y <= LED_ROWS - 1; y++) {
      for (int x = LED_COLUMNS - 1; x >= 0; x--) {
        CRGB *cell = &leds[XY1(x,y)];
        if (*cell) {
          CRGB *bottom_cell = &leds[XY1(x,y-1)];
          if (!(*bottom_cell)) {
            *bottom_cell = *cell;
            *cell = 0;
          } else {
            if (x != 0) {
              CRGB *bottom_right_cell = &leds[XY1(x-1,y-1)];
              if (!(*bottom_right_cell)) {
                *bottom_right_cell = *cell;
                *cell = 0;
                continue;
              }
            }
            if (x != LED_COLUMNS - 1) {
              CRGB *bottom_left_cell = &leds[XY1(x+1,y-1)];
              if (!(*bottom_left_cell)) {
                *bottom_left_cell = *cell;
                *cell = 0;
              }
            }
          }
        }
      }
    }
  }
}

// make a step of the cellular automaton over axis X
// positive direction -- step right; negative -- step left
void sand_step_x(int direction) {
  if (direction == 0) return;
  if (direction > 0) {
    // iterate over cells from right to left and downside up
    // we don't care about the rightest row as the cells there are not going anywhere
    for (int x = LED_COLUMNS - 2; x >= 0; x--) {
      for (int y = LED_ROWS - 1; y >= 0; y--) {
        CRGB *cell = &leds[XY1(x,y)];
        if (*cell) {
          CRGB *right_cell = &leds[XY1(x+1,y)];
          if (!(*right_cell)) {
            *right_cell = *cell;
            *cell = 0;
          } else {
            if (y != LED_ROWS - 1) {
              CRGB *bottom_right_cell = &leds[XY1(x+1,y+1)];
              if (!(*bottom_right_cell)) {
                *bottom_right_cell = *cell;
                *cell = 0;
                continue;
              }
            }
            if (y != 0) {
              CRGB *upper_right_cell = &leds[XY1(x+1,y-1)];
              if (!(*upper_right_cell)) {
                *upper_right_cell = *cell;
                *cell = 0;
              }
            }
          }
        }
      }
    }
  } else {
    // iterate over cells from left to right and downside up
    for (int x = 1; x <= LED_COLUMNS - 1; x++) {
      for (int y = LED_ROWS - 1; y >= 0; y--) {
        CRGB *cell = &leds[XY1(x,y)];
        if (*cell) {
          CRGB *left_cell = &leds[XY1(x-1,y)];
          if (!(*left_cell)) {
            *left_cell = *cell;
            *cell = 0;
          } else {
            if (y != 0) {
              CRGB *upper_left_cell = &leds[XY1(x-1,y-1)];
              if (!(*upper_left_cell)) {
                *upper_left_cell = *cell;
                *cell = 0;
                continue;
              }
            }
            if (y != LED_ROWS - 1) {
              CRGB *bottom_left_cell = &leds[XY1(x-1,y+1)];
              if (!(*bottom_left_cell)) {
                *bottom_left_cell = *cell;
                *cell = 0;
              }
            }
          }
        }
      }
    }
  }
}

void sand_get_delays(SandContext *ctx) {
  if (mpu.dmpGetCurrentFIFOPacket(mpuFifoBuf)) {
      Quaternion q;
      VectorFloat gravity;

      mpu.dmpGetQuaternion(&q, mpuFifoBuf);
      mpu.dmpGetGravity(&gravity, &q);

      gravity.x = (float)ceil(gravity.x * 100.0f) / 100.0f;
      gravity.y = (float)ceil(gravity.y * 100.0f) / 100.0f;
      // x and y of the gravity vector and of the LED matrix are swapped
      float buf = gravity.x;
      gravity.x = gravity.y;
      gravity.y = buf;

      int del_x, del_y;
      if (gravity.x == 0) {
        del_x = INT_MAX;
        ctx->dir_x = 0;
      } else {
        del_x = ceil(40 / gravity.x);
        ctx->dir_x = (del_x > 0) ? -1 : 1;
      }

      if (gravity.y == 0) {
        del_y = INT_MAX;
        ctx->dir_y = 0;
      } else {
        del_y = ceil(40 / gravity.y);
        ctx->dir_y = (del_y > 0) ? -1 : 1;
      }

      // 100 ms threshold of changing the speed of cells
      if (abs(del_x - ctx->delay_x) >= 100) {
        ctx->delay_x = abs(del_x);
      }

      if (abs(del_y - ctx->delay_y) >= 100) {
        ctx->delay_y = abs(del_y);
      }
      /*
      Serial.print(gravity.x);
      Serial.print(',');
      Serial.print(gravity.y);
      Serial.print(' ');
      Serial.print(del_x);
      Serial.print(',');
      Serial.println(del_y);
      */
    }
}

void sand_play_animation(SandContext *ctx) {
  static unsigned long mpu_last_read = 0;
  static unsigned long x_ticked = 0;
  static unsigned long y_ticked = 0;

  unsigned long now = millis();
  if (now - mpu_last_read >= 11 || mpu_last_read == 0) {
    sand_get_delays(ctx);
    mpu_last_read = now;
  }
  if (now - x_ticked >= ctx->delay_x) {
    sand_step_x(ctx->dir_x);
    x_ticked = now;
  }
  if (now - y_ticked >= ctx->delay_y) {
    sand_step_y(ctx->dir_y);
    y_ticked = now;
  }

  Serial.print(ctx->delay_x);
  Serial.print(',');
  Serial.println(ctx->delay_y);
}
