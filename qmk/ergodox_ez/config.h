#pragma once

// Identity — required for Keymapp/live-training to recognize this board.
// Matches the Oryx layout this keymap is derived from: xbvqK/nlyNZG.
#define SERIAL_NUMBER "xbvqK/nlyNZG"
#define USB_SUSPEND_WAKEUP_DELAY 0

// 8 layers max (we use 5: BASE/GAME/SYM/NUM/MOUSE). Saves flash + RAM.
#define LAYER_STATE_8BIT

// keyboards/zsa/ergodox_ez/m32u4/info.json declares 20ms; the Oryx export's
// 40ms is a blanket default that adds real latency against a 175ms
// quick-tap window. Step back up toward 40 if chatter appears after flashing.
#undef DEBOUNCE
#define DEBOUNCE 20

// Tap-hold tuning — ported from config/totem.keymap's hml/hmr/&lt behaviors.
// The Oryx export used TAPPING_TERM 180, which is too short to hold a
// home-row mod reliably; 280 matches the Totem. It also lengthens the hold
// on the Space and Tab thumb keys.
//   tapping-term-ms 280        -> TAPPING_TERM
//   quick-tap-ms 175           -> QUICK_TAP_TERM
//   require-prior-idle-ms 50   -> FLOW_TAP_TERM
//   flavor "balanced"          -> PERMISSIVE_HOLD
//   hold-trigger-key-positions -> CHORDAL_HOLD (chordal_hold_layout in keymap.c)
#undef TAPPING_TERM
#define TAPPING_TERM 280
#define QUICK_TAP_TERM 175
#define PERMISSIVE_HOLD
#define PERMISSIVE_HOLD_PER_KEY // thumb tap-holds + number row are tap-preferred instead
#define CHORDAL_HOLD
#define FLOW_TAP_TERM 50

// Combos — COMBO_TERM_PER_COMBO and COMBO_SHOULD_TRIGGER are load-bearing:
// without them QMK silently ignores get_combo_term() and
// combo_should_trigger() in keymap.c, and combos would fire on every layer.
#define COMBO_TERM 50
#define COMBO_TERM_PER_COMBO
#define COMBO_SHOULD_TRIGGER
#define EXTRA_SHORT_COMBOS // every combo here is exactly 2 keys

#define CAPS_WORD_IDLE_TIMEOUT 3000

// Mouse tuning — kept verbatim from the Oryx export; the MOUSE layer uses it.
#undef MOUSEKEY_INTERVAL
#define MOUSEKEY_INTERVAL 25

#undef MOUSEKEY_MAX_SPEED
#define MOUSEKEY_MAX_SPEED 6

#undef MOUSEKEY_TIME_TO_MAX
#define MOUSEKEY_TIME_TO_MAX 1

#undef MOUSEKEY_WHEEL_MAX_SPEED
#define MOUSEKEY_WHEEL_MAX_SPEED 5

#undef MOUSEKEY_WHEEL_TIME_TO_MAX
#define MOUSEKEY_WHEEL_TIME_TO_MAX 80

// RGB — a single custom effect (rgb_matrix_user.inc): per-layer colors (GAME
// a drifting rainbow), and on every other layer that rainbow revealed under
// pressed keys.
// RGB_MATRIX_KEYPRESSES feeds it the per-key press tracker. Every stock
// animation the board's config.h turns on is compiled out below, which also
// frees the flash they took.
#define RGB_MATRIX_KEYPRESSES
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CUSTOM_LAYER_GLOW

#undef ENABLE_RGB_MATRIX_ALPHAS_MODS
#undef ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#undef ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
#undef ENABLE_RGB_MATRIX_BREATHING
#undef ENABLE_RGB_MATRIX_BAND_SAT
#undef ENABLE_RGB_MATRIX_BAND_VAL
#undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
#undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
#undef ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
#undef ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
#undef ENABLE_RGB_MATRIX_CYCLE_ALL
#undef ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#undef ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
#undef ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN
#undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#undef ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
#undef ENABLE_RGB_MATRIX_CYCLE_SPIRAL
#undef ENABLE_RGB_MATRIX_DUAL_BEACON
#undef ENABLE_RGB_MATRIX_RAINBOW_BEACON
#undef ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#undef ENABLE_RGB_MATRIX_RAINDROPS
#undef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#undef ENABLE_RGB_MATRIX_HUE_BREATHING
#undef ENABLE_RGB_MATRIX_HUE_PENDULUM
#undef ENABLE_RGB_MATRIX_HUE_WAVE
#undef ENABLE_RGB_MATRIX_PIXEL_RAIN
#undef ENABLE_RGB_MATRIX_PIXEL_FLOW
#undef ENABLE_RGB_MATRIX_PIXEL_FRACTAL
#undef ENABLE_RGB_MATRIX_TYPING_HEATMAP
#undef ENABLE_RGB_MATRIX_DIGITAL_RAIN
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#undef ENABLE_RGB_MATRIX_SPLASH
#undef ENABLE_RGB_MATRIX_MULTISPLASH
#undef ENABLE_RGB_MATRIX_SOLID_SPLASH
#undef ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
