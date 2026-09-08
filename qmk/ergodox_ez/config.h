#pragma once

// Identity — required for Keymapp/live-training to recognize this board.
#define SERIAL_NUMBER "xbvqK/nlyNZG"
#define USB_SUSPEND_WAKEUP_DELAY 0

// 8 layers max (we use 6: BASE/GAME/SYM/NUM/ADJ/FUN). Saves flash + RAM.
#define LAYER_STATE_8BIT

// keyboards/zsa/ergodox_ez/m32u4/info.json declares 20ms; the Oryx export's
// 40ms is a blanket default that adds real latency against a 175ms
// quick-tap window. Step back up toward 40 if chatter appears after flashing.
#undef DEBOUNCE
#define DEBOUNCE 20

// Tap-hold tuning — ported from totem.keymap's hml/hmr/&lt behaviors.
//   tapping-term-ms 280        -> TAPPING_TERM
//   quick-tap-ms 175           -> QUICK_TAP_TERM
//   require-prior-idle-ms 150  -> FLOW_TAP_TERM
//   flavor "balanced"          -> PERMISSIVE_HOLD
//   hold-trigger-key-positions -> CHORDAL_HOLD (chordal_hold_layout in keymap.c)
#undef TAPPING_TERM
#define TAPPING_TERM 280
#define QUICK_TAP_TERM 175
#define PERMISSIVE_HOLD
#define PERMISSIVE_HOLD_PER_KEY // thumb LTs + number row are tap-preferred instead
#define CHORDAL_HOLD
#define FLOW_TAP_TERM 150

// Combos — both required for get_combo_term()/combo_should_trigger() in
// keymap.c to actually be called; without them QMK silently ignores both.
#define COMBO_TERM 50
#define COMBO_TERM_PER_COMBO
#define COMBO_SHOULD_TRIGGER
#define EXTRA_SHORT_COMBOS // every combo here is exactly 2 keys

#define CAPS_WORD_IDLE_TIMEOUT 3000
