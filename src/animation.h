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

#include "effects.h"

Animation animation;

void make_anim() {
  // 0
  static Sprite *notes_frames[] = {&NOTES, &NOTES1, &NOTES, &NOTES1, &NOTES, &NOTES1, &NOTES, &NOTES1};
  static Effect notes_effect = {
    .wait = 500,
    .start = animation_start,
    .update = animation_update
  };
  static CRGB notes_color_map[] = {CRGB::Black, CRGB::Green, CRGB::Purple};
  static AnimationDescriptor notes_desc = {
    .initialized = &notes_effect.initialized,
    .wait = &notes_effect.wait,
    .num = 8,
    .frames = notes_frames,
    .color_map = notes_color_map
  };
  notes_effect.ctx = &notes_desc;

  // 1
  static Effect we = {
    .wait = 0,
    .start = draw_sprite_start,
    .update = draw_sprite_update
  };
  static SpriteDescriptor we_desc = {
    .initialized = &we.initialized,
    .wait = &we.wait,
    .sprite = &WE
  };
  we.ctx = &we_desc;

  // 2
  static Effect fade_in_100 = {
    .wait = 0,
    .start = fade_start,
    .update = fade_update
  };
  static FadeDescriptor fade_in_100_desc = {
    .initialized = &fade_in_100.initialized,
    .wait = &fade_in_100.wait,
    .time = 100,
    .direction = 1
  };
  fade_in_100.ctx = &fade_in_100_desc;

  // 3
  static Effect fade_out_200 = {
    .wait = 0,
    .start = fade_start,
    .update = fade_update
  };
  static FadeDescriptor fade_out_200_desc = {
    .initialized = &fade_out_200.initialized,
    .wait = &fade_out_200.wait,
    .time = 200,
    .direction = -1
  };
  fade_out_200.ctx = &fade_out_200_desc;

  // 4
  static Effect fe = {
    .wait = 0,
    .start = draw_sprite_start,
    .update = draw_sprite_update
  };
  static SpriteDescriptor fe_desc = {
    .initialized = &fe.initialized,
    .wait = &fe.wait,
    .sprite = &FE
  };
  fe.ctx = &fe_desc;

  // 5
  // fade_in_100

  // 6
  static Sprite *FELL[] = {&FE, &LL};
  static Effect fell = {
    .wait = 50,
    .start = running_line_start,
    .update = running_line_update
  };
  static RunningLineDescriptor fell_desc = {
    .initialized = &fell.initialized,
    .wait = &fell.wait,
    .sprites = FELL,
    .num = 2,
    .color = CRGB::Blue,
    .gap = true
  };
  fell.ctx = &fell_desc;

  // 7
  static Effect fade_out_100 = {
    .wait = 0,
    .start = fade_start,
    .update = fade_update
  };
  static FadeDescriptor fade_out_100_desc = {
    .initialized = &fade_out_100.initialized,
    .wait = &fade_out_100.wait,
    .time = 100,
    .direction = -1
  };
  fade_out_100.ctx = &fade_out_100_desc;

  // 8
  static Effect in = {
    .wait = 0,
    .start = draw_sprite_start,
    .update = draw_sprite_update
  };
  static SpriteDescriptor in_desc = {
    .initialized = &in.initialized,
    .wait = &in.wait,
    .sprite = &IN
  };
  in.ctx = &in_desc;

  // 9
  // fade_in_100

  // 10
  // fade_out_200

  // 11
  static Effect lo = {
    .wait = 0,
    .start = draw_sprite_start,
    .update = draw_sprite_update
  };
  static SpriteDescriptor lo_desc = {
    .initialized = &lo.initialized,
    .wait = &lo.wait,
    .sprite = &LO,
    .color = CRGB::Red,
    .render = false
  };
  lo.ctx = &lo_desc;

  // 12
  // fade_in_100

  // 13
  static Sprite *LOVE[] = {&LO, &VE};
  static Effect love = {
    .wait = 50,
    .start = running_line_start,
    .update = running_line_update
  };
  static RunningLineDescriptor love_desc = {
    .initialized = &love.initialized,
    .wait = &love.wait,
    .sprites = LOVE,
    .num = 2,
    .color = CRGB::Red,
    .gap = true
  };
  love.ctx = &love_desc;

  // 14
  // fade_out_100

  // 15
  // in

  // 16
  // fade_in_100

  // 17
  // fade_out_200

  // 18
  static Effect oc = {
    .wait = 0,
    .start = draw_sprite_start,
    .update = draw_sprite_update
  };
  static SpriteDescriptor oc_desc = {
    .initialized = &oc.initialized,
    .wait = &oc.wait,
    .sprite = &OC,
    .color = CRGB::Orange,
    .render = false
  };
  oc.ctx = &oc_desc;

  // 19
  // fade_in_100

  // 20
  static Sprite *OCTOBER[] = {&OC, &TO, &BE, &R};
  static Effect october = {
    .wait = 40,
    .start = running_line_start,
    .update = running_line_update
  };
  static RunningLineDescriptor october_desc = {
    .initialized = &october.initialized,
    .wait = &october.wait,
    .sprites = OCTOBER,
    .num = 4,
    .color = CRGB::Orange,
    .gap = true
  };
  october.ctx = &october_desc;

  // 21
  static Effect heart_transition = {
    .wait = 0,
    .start = fill_transition_start,
    .update = fill_transition_update
  };
  static FillTransitionDescriptor heart_transition_desc = {
    .initialized = &heart_transition.initialized,
    .wait = &heart_transition.wait,
    .sprite = &HEART
  };
  heart_transition.ctx = &heart_transition_desc;

  // 22
  static Effect fade_out_300 = {
    .wait = 0,
    .start = fade_start,
    .update = fade_update
  };
  static FadeDescriptor fade_out_300_desc = {
    .initialized = &fade_out_300.initialized,
    .wait = &fade_out_300.wait,
    .time = 300,
    .direction = -1
  };
  fade_out_300.ctx = &fade_out_300_desc;

  // 23
  static Effect th = {
    .wait = 0,
    .start = draw_sprite_start,
    .update = draw_sprite_update
  };
  static SpriteDescriptor th_desc = {
    .initialized = &th.initialized,
    .wait = &th.wait,
    .sprite = &TH,
    .color = CRGB::Blue,
    .render = false
  };
  th.ctx = &th_desc;

  // 24
  // fade_in_100

  // 25
  static Sprite *THATS[] = {&TH, &AT, &S};
  static Effect thats = {
    .wait = 50,
    .start = running_line_start,
    .update = running_line_update
  };
  static RunningLineDescriptor thats_desc = {
    .initialized = &thats.initialized,
    .wait = &thats.wait,
    .sprites = THATS,
    .num = 3,
    .color = CRGB::Blue,
    .gap = true
  };
  thats.ctx = &thats_desc;

  // 26
  static Effect fade_out_50 = {
    .wait = 0,
    .start = fade_start,
    .update = fade_update
  };
  static FadeDescriptor fade_out_50_desc = {
    .initialized = &fade_out_50.initialized,
    .wait = &fade_out_50.wait,
    .time = 50,
    .direction = -1
  };
  fade_out_50.ctx = &fade_out_50_desc;

  // 27
  static Effect wh = {
    .wait = 0,
    .start = draw_sprite_start,
    .update = draw_sprite_update
  };
  static SpriteDescriptor wh_desc = {
    .initialized = &wh.initialized,
    .wait = &wh.wait,
    .sprite = &WH,
    .color = CRGB::Blue,
    .render = false
  };
  wh.ctx = &wh_desc;

  // 28
  static Effect fade_in_50 = {
    .wait = 0,
    .start = fade_start,
    .update = fade_update
  };
  static FadeDescriptor fade_in_50_desc = {
    .initialized = &fade_in_50.initialized,
    .wait = &fade_in_50.wait,
    .time = 50,
    .direction = 1
  };
  fade_in_50.ctx = &fade_in_50_desc;

  // 29
  static Sprite *WHY[] = {&WH, &HY};
  static Effect why = {
    .wait = 50,
    .start = running_line_start,
    .update = running_line_update
  };
  static RunningLineDescriptor why_desc = {
    .initialized = &why.initialized,
    .wait = &why.wait,
    .sprites = WHY,
    .num = 2,
    .color = CRGB::Blue,
    .gap = false
  };
  why.ctx = &why_desc;

  // 30
  // fade_out_50

  // 31
  static Effect i = {
    .wait = 0,
    .start = draw_sprite_start,
    .update = draw_sprite_update
  };
  static SpriteDescriptor i_desc = {
    .initialized = &i.initialized,
    .wait = &i.wait,
    .sprite = &I,
    .color = CRGB::Blue,
    .render = false
  };
  i.ctx = &i_desc;

  // 32
  static Effect fade_in_200 = {
    .wait = 0,
    .start = fade_start,
    .update = fade_update
  };
  static FadeDescriptor fade_in_200_desc = {
    .initialized = &fade_in_200.initialized,
    .wait = &fade_in_200.wait,
    .time = 200,
    .direction = 1
  };
  fade_in_200.ctx = &fade_in_200_desc;

  // 33
  // fade_out_200

  // 34
  // lo

  // 35
  // fade_in_100

  // 36
  // love

  // 37
  // fade_out_50

  // 38
  static Effect fa = {
    .wait = 0,
    .start = draw_sprite_start,
    .update = draw_sprite_update
  };
  static SpriteDescriptor fa_desc = {
    .initialized = &fa.initialized,
    .wait = &fa.wait,
    .sprite = &FA,
    .color = CRGB::Orange,
    .render = false
  };
  fa.ctx = &fa_desc;

  // 39
  // fade_in_50

  // 40
  static Sprite *FALL[] = {&FA, &LL};
  static Effect fall = {
    .wait = 50,
    .start = running_line_start,
    .update = running_line_update
  };
  static RunningLineDescriptor fall_desc = {
    .initialized = &fall.initialized,
    .wait = &fall.wait,
    .sprites = FALL,
    .num = 2,
    .color = CRGB::Orange,
    .gap = true
  };
  fall.ctx = &fall_desc;

  // 41
  static Effect square_transition = {
    .wait = 100,
    .start = square_transition_start,
    .update = square_transition_update
  };
  static SquareTransitionDescriptor square_transition_desc = {
    .initialized = &square_transition.initialized,
    .wait = &square_transition.wait
  };
  square_transition.ctx = &square_transition_desc;

  // 42
  static CRGB leaf_color_map[] = {CRGB::Black, CRGB::SaddleBrown, CRGB::NavajoWhite, CRGB::Green};
  static Sprite *leaf_frames[] = {&LEAF_ANIM_1, &LEAF_ANIM_2, &LEAF_ANIM_3, &LEAF};
  static Effect leaf_effect = {
    .wait = 100,
    .start = animation_start,
    .update = animation_update
  };
  static AnimationDescriptor leaf_desc = {
    .initialized = &leaf_effect.initialized,
    .wait = &leaf_effect.wait,
    .num = 4,
    .frames = leaf_frames,
    .color_map = leaf_color_map
  };
  leaf_effect.ctx = &leaf_desc;

  // 43
  static Effect delay_1000 = {
    .wait = 0,
    .start = delay_start,
    .update = delay_update
  };
  static DelayDescriptor delay_1000_desc = {
    .initialized = &delay_1000.initialized,
    .wait = &delay_1000.wait,
    .del = 1000
  };
  delay_1000.ctx = &delay_1000_desc;

  // 44
  // fade_out_200


  // --------------------------------

  static Effect *effects[] = {
    &notes_effect,
    &we,
    &fade_in_100,
    &fade_out_200,
    &fe,
    &fade_in_100,
    &fell,
    &fade_out_100,
    &in,
    &fade_in_100,
    &fade_out_200,
    &lo,
    &fade_in_100,
    &love,
    &fade_out_100,
    &in,
    &fade_in_100,
    &fade_out_200,
    &oc,
    &fade_in_100,
    &october,
    &heart_transition,
    &fade_out_300,
    &th,
    &fade_in_100,
    &thats,
    &fade_out_50,
    &wh,
    &fade_in_50,
    &why,
    &fade_out_50,
    &i,
    &fade_in_200,
    &fade_out_200,
    &lo,
    &fade_in_100,
    &love,
    &fade_out_50,
    &fa,
    &fade_in_50,
    &fall,
    &square_transition,
    &leaf_effect,
    &delay_1000,
    &fade_out_200
    };
  animation.effects = effects;
  animation.num = 45;
}
