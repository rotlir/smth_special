#define DEBUG 0

#include <FastLED.h>

#define EB_NO_CALLBACK
#define EB_HOLD_TIME 3000
#include <EncButton.h>

#include "settings.h"
#include "sprites.h"
#include "led_helpers.h"
#include "effects.h"
#include "animation.h"
#include "sand.h"
#include "counter.h"

SandContext sand_ctx;

enum State {ANIMATION, SAND, COUNTER};

enum ButtonState {NONE, CLICKED, HELD};

Button btn(BTN_PIN);

enum ButtonState get_btn_state() {
  btn.tick();
  if (btn.click()) return CLICKED;
  if (btn.hold()) return HELD;
  return NONE;
}

void setup() {
  #if DEBUG == 1
  delay(5000);
  Serial.begin(115200);
  #endif

  Wire.begin();
  mpu_init();
  FastLED.addLeds<LED_CHIP, LED_PIN, LED_COLOR_ORDER>(leds, LED_NUM);
  FastLED.setBrightness(5);
  if (!digitalRead(BTN_PIN)) mpu_calibrate();
  
  make_anim();
  
  counter_read();
}

void loop() {
  static bool state_changed = true;
  static enum State state = ANIMATION;

  int del = 0;

  switch (state) {
    case ANIMATION:
      if (state_changed) {
        state_changed = false;
        animation.effects[animation.index]->initialized = false;
        animation.index = 0;
      }
      del = play_animation(&animation);
      break;
    case SAND:
      if (state_changed) {
        state_changed = false;
        sand_ctx = {0,};
        sand_ctx.delay_x = -1000;
        sand_ctx.delay_y = -1000;
        sand_init();
        FastLED.show();
      }
      sand_play_animation(&sand_ctx);
      FastLED.show();
      break;
    case COUNTER:
      if (state_changed) {
        state_changed = false;
        counter_screen();
      }
      break;
  }

  bool single_run = (del == 0);
  
  unsigned long tmr = millis();
  while (single_run || millis() - tmr < del) {
    enum ButtonState btn_state = get_btn_state();
    if (btn_state == CLICKED) {
      FastLED.setBrightness(5);
      if (state == COUNTER) state = ANIMATION;
      else state = (enum State)(state + 1);
      state_changed = true;
    }

    if (btn_state == HELD) {
      Serial.println("held");
      counter_update();
      counter_update_animation();
      state_changed = true;
    }
    if (single_run) break;
  }
}
