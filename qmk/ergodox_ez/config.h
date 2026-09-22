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

#define RGB_MATRIX_STARTUP_SPD 60
