// ErgoDox EZ Glow — the Oryx layout xbvqK/nlyNZG, with home-row mods, a
// bilateral chordal-hold guard and combos added to the base layer only.
//
// Layers [1]-[4] are carried over verbatim from the Oryx source export; the
// only change to them is that their numeric indices now have names. The
// GAME layer in particular is untouched: no mods, and combo_should_trigger()
// keeps every combo off it.
//
// Each LAYOUT_ergodox_pretty() row below combines both halves on one line,
// left-to-right, matching the Oryx export's argument order; the thumb
// cluster follows as three groups (top pair, inner pair, bottom triple),
// left half then right half within each group.
//
// The three status LEDs (top right) show the highest active layer, using
// Oryx's pattern — see layer_state_set_user() at the end of this file.

#include QMK_KEYBOARD_H

#ifndef ZSA_SAFE_RANGE
#define ZSA_SAFE_RANGE SAFE_RANGE
#endif

enum custom_keycodes {
  RGB_SLD = ZSA_SAFE_RANGE,
};

enum layers {
    _BASE = 0,
    _GAME,   // TG(_GAME) on the right home-row outer key
    _SYM,    // held via the left thumb's LT(_SYM, KC_SPC)
    _NUM,    // TG(_NUM), also held via LT(_NUM, KC_N)
    _MOUSE,  // TG(_MOUSE)
};

enum combo_names {
    CB_CAPS = 0,
    CB_DQUO,
    CB_QUOT,
    CB_HOME,
    CB_END,
    CB_COLN,
    CB_UNDS,
    CB_MINS,
    CB_QUES,
};

// Tap = digit, hold = F-key — the Oryx export's ten copy-pasted DUAL_FUNC_n
// blocks, collapsed. LT(0, kc) is QMK's documented idiom for a layer-tap
// with no practical layer effect (layer 0 is always active), used purely as
// a carrier for the tap-hold timing machinery; process_record_user() below
// intercepts it before layer 0 would "activate". The export's placeholder
// hold-layers (6/7/9/13/15) were unreachable under LAYER_STATE_8BIT anyway.
#define NUMF(kc) LT(0, kc)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Note the deliberate 5/6 duplication across the halves on the number
    // row — the left half is 1-6, the right half is 5-0. That is how the
    // Oryx layout is; it is not a transcription slip.
    [_BASE] = LAYOUT_ergodox_pretty(
        KC_ESCAPE,      NUMF(KC_1),         NUMF(KC_2),         NUMF(KC_3),         NUMF(KC_4),           NUMF(KC_5), NUMF(KC_6),              NUMF(KC_5),     NUMF(KC_6),         NUMF(KC_7),         NUMF(KC_8),         NUMF(KC_9),            NUMF(KC_0), KC_TRANSPARENT,
        KC_TAB,         KC_Q,               KC_W,               KC_E,               KC_R,                 KC_T,       KC_HOME,                 KC_PAGE_UP,     KC_Y,               KC_U,               KC_I,               KC_O,                  KC_P,       TG(_NUM),
        KC_LEFT_CTRL,   MT(MOD_LGUI, KC_A), MT(MOD_LALT, KC_S), MT(MOD_LCTL, KC_D), MT(MOD_LSFT, KC_F),   KC_G,                                                KC_H,               MT(MOD_RSFT, KC_J), MT(MOD_RCTL, KC_K), MT(MOD_RALT, KC_L),    MT(MOD_RGUI, KC_SCLN), TG(_GAME),
        KC_LEFT_SHIFT,  KC_Z,               KC_X,               KC_C,               KC_V,                 KC_B,       KC_END,                  KC_PGDN,        LT(_NUM, KC_N),     KC_M,               KC_COMMA,           KC_DOT,                KC_SLASH,   TG(_MOUSE),
        LCTL(KC_LEFT_SHIFT), LCTL(KC_LEFT_ALT), LALT(KC_LEFT_SHIFT), KC_LEFT_ALT,   MT(MOD_LGUI, KC_ESCAPE),                                                                       KC_LEFT,            KC_UP,              KC_DOWN,               KC_RIGHT,   KC_LEFT_GUI,

                                                                                                          KC_AUDIO_VOL_DOWN, KC_AUDIO_VOL_UP,  KC_PSCR,        LSFT(KC_PSCR),
                                                                                                                             KC_AUDIO_MUTE,    KC_CAPS,
                                                                 LT(_SYM, KC_SPACE), MT(MOD_LCTL, KC_TAB), KC_ENTER,                           KC_DELETE,      KC_BSPC,            KC_RIGHT_SHIFT
    ),

    // Gaming layer — deliberately left exactly as exported. No home-row
    // mods; combos are gated off it in combo_should_trigger().
    [_GAME] = LAYOUT_ergodox_pretty(
        KC_TRANSPARENT, KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           KC_6,                                   KC_5,           KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           KC_TRANSPARENT,
        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                         KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
        // The home row is pinned to plain letters here. The Oryx export left
        // it KC_TRANSPARENT, which fell through to a base layer that had
        // plain KC_A..KC_SCLN — now that base has mod-taps, transparent would
        // silently give the GAME layer home-row mods. Same reason the export
        // already pins KC_N below instead of inheriting LT(_NUM, KC_N).
        KC_TRANSPARENT, KC_A,           KC_S,           KC_D,           KC_F,           KC_TRANSPARENT,                                                         KC_TRANSPARENT, KC_J,           KC_K,           KC_L,           KC_SCLN,        KC_TRANSPARENT,
        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                         KC_TRANSPARENT, KC_N,           KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_SPACE,                                                                               KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,

                                                                                        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                                        KC_TRANSPARENT, KC_TRANSPARENT,
                                                                    LT(_SYM, KC_SPACE), KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
    ),

    [_SYM] = LAYOUT_ergodox_pretty(
        KC_TRANSPARENT, KC_F1,          KC_F2,          KC_F3,          KC_F4,          KC_F5,          KC_F6,                                  KC_F5,          KC_F6,          KC_F7,          KC_F8,          KC_F9,          KC_F10,         KC_F11,
        KC_TRANSPARENT, KC_GRAVE,       KC_LABK,        KC_RABK,        KC_MINUS,       KC_PIPE,        KC_TRANSPARENT,                         KC_TRANSPARENT, KC_CIRC,        KC_LCBR,        KC_RCBR,        KC_DLR,         KC_AMPR,        KC_F12,
        KC_TRANSPARENT, KC_EXLM,        KC_ASTR,        KC_SLASH,       KC_EQUAL,       KC_AMPR,                                                                KC_HASH,        KC_LPRN,        KC_RPRN,        KC_SCLN,        KC_DQUO,        KC_TRANSPARENT,
        KC_TRANSPARENT, KC_TILD,        KC_PLUS,        KC_UNDS,        KC_COLN,        KC_PERC,        KC_TRANSPARENT,                         KC_TRANSPARENT, KC_AT,          KC_LBRC,        KC_RBRC,        KC_BSLS,        KC_QUOTE,       KC_TRANSPARENT,
        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                                                         KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,

                                                                                        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                                        KC_TRANSPARENT, KC_TRANSPARENT,
                                                                        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
    ),

    [_NUM] = LAYOUT_ergodox_pretty(
        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,       KC_TRANSPARENT, KC_TRANSPARENT,       KC_TRANSPARENT, KC_TRANSPARENT,             KC_TRANSPARENT, KC_TRANSPARENT, KC_7,           KC_8,           KC_9,           KC_TRANSPARENT, KC_TRANSPARENT,
        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,       KC_TRANSPARENT, KC_TRANSPARENT,       KC_TRANSPARENT, KC_TRANSPARENT,             KC_TRANSPARENT, KC_TRANSPARENT, KC_4,           KC_5,           KC_6,           KC_TRANSPARENT, KC_TRANSPARENT,
        KC_TRANSPARENT, KC_TRANSPARENT, LALT(LCTL(KC_MINUS)), KC_TRANSPARENT, LCTL(LSFT(KC_MINUS)), KC_TRANSPARENT,                                             KC_TRANSPARENT, KC_1,           KC_2,           KC_3,           KC_TRANSPARENT, KC_TRANSPARENT,
        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,       KC_TRANSPARENT, KC_TRANSPARENT,       KC_TRANSPARENT, KC_TRANSPARENT,             KC_TRANSPARENT, KC_TRANSPARENT, KC_COMMA,       KC_0,           KC_TRANSPARENT, KC_TRANSPARENT, TO(_BASE),
        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,       KC_TRANSPARENT, KC_TRANSPARENT,                                                                   KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,

                                                                                        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                                        KC_TRANSPARENT, KC_TRANSPARENT,
                                                                        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
    ),

    [_MOUSE] = LAYOUT_ergodox_pretty(
        KC_TRANSPARENT, KC_TRANSPARENT,   KC_TRANSPARENT,     KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                   KC_TRANSPARENT, KC_TRANSPARENT,      KC_TRANSPARENT,      KC_TRANSPARENT, KC_TRANSPARENT,   KC_TRANSPARENT,     KC_TRANSPARENT,
        KC_TRANSPARENT, RGB_SPI,          KC_TRANSPARENT,     KC_MS_UP,       KC_TRANSPARENT, RGB_SAI,        KC_TRANSPARENT,                   KC_TRANSPARENT, KC_MEDIA_PREV_TRACK, KC_MEDIA_NEXT_TRACK, KC_MS_WH_UP,    KC_TRANSPARENT,   KC_TRANSPARENT,     TO(_BASE),
        KC_TRANSPARENT, RGB_SPD,          KC_MS_LEFT,         KC_MS_DOWN,     KC_MS_RIGHT,    RGB_SAD,                                                          KC_MEDIA_PLAY_PAUSE, KC_MS_WH_LEFT,       KC_MS_WH_DOWN,  KC_MS_WH_RIGHT,   KC_BRIGHTNESS_UP,   KC_TRANSPARENT,
        KC_TRANSPARENT, RGB_MODE_FORWARD, TOGGLE_LAYER_COLOR, RGB_VAD,        RGB_VAI,        RGB_TOG,        KC_TRANSPARENT,                   KC_TRANSPARENT, KC_TRANSPARENT,      KC_MS_ACCEL0,        KC_MS_ACCEL1,   KC_MS_ACCEL2,     KC_BRIGHTNESS_DOWN, KC_TRANSPARENT,
        KC_TRANSPARENT, RGB_HUD,          RGB_HUI,            RGB_SLD,        KC_TRANSPARENT,                                                                   KC_TRANSPARENT,      KC_TRANSPARENT,      KC_TRANSPARENT, KC_TRANSPARENT,   KC_TRANSPARENT,

                                                                                        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                                        KC_TRANSPARENT, KC_TRANSPARENT,
                                                                        KC_MS_BTN1,     KC_MS_BTN2,     KC_MS_BTN3,     KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
    ),
};

// Chordal Hold's per-key handedness map — the QMK equivalent of the Totem's
// hold-trigger-key-positions bilateral guard (config/totem.keymap). A
// home-row mod only settles as held when chorded with the OTHER hand; a
// same-hand roll resolves as a tap. '*' (thumbs) may settle as held with
// either hand, matching THUMBS being listed in both hml's and hmr's trigger
// sets on the Totem. Same row/thumb grouping as the keymaps above.
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT_ergodox_pretty(
    'L', 'L', 'L', 'L', 'L', 'L', 'L',                                        'R', 'R', 'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L', 'L', 'L',                                        'R', 'R', 'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L', 'L',                                                       'R', 'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L', 'L', 'L',                                        'R', 'R', 'R', 'R', 'R', 'R', 'R',
    // The 5th entry is MT(MOD_LGUI, KC_ESCAPE) — the main Super key, and a
    // thumb key in all but name. It is '*' (exempt from the bilateral guard)
    // on purpose: it is chorded with the LEFT hand constantly (Super+1..6,
    // Super+Q/W/E, Super+Shift+...), and 'L' here made every one of those
    // settle as a tap, so Super only registered if held past TAPPING_TERM.
    // The Totem has the same key on a thumb, where plain &mt has no
    // hold-trigger-key-positions guard at all.
    'L', 'L', 'L', 'L', '*',                                                                 'R', 'R', 'R', 'R', 'R',

                             '*', '*',    '*', '*',
                                  '*',    '*',
                    '*', '*', '*',        '*', '*', '*'
);

// Home-row mods are `balanced` (PERMISSIVE_HOLD). The thumb tap-holds stay
// tap-preferred: with `balanced`, rolling from Space into the next word's
// first letter falsely resolves as a hold and fires SYM. The number row is
// tap-preferred for the same reason — fast digit rolls must stay digits.
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(_SYM, KC_SPACE):
        case MT(MOD_LCTL, KC_TAB):
            return false;
    }
    if (keycode >= NUMF(KC_1) && keycode <= NUMF(KC_0)) {
        return false;
    }
    return true; // balanced, for the home row mods
}

// Only the home-row mods want require-prior-idle. KC_SPC and KC_TAB are both
// in QMK's default is_flow_tap_key() set, so without this override SYM would
// be unreachable for FLOW_TAP_TERM after every letter.
uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t *record, uint16_t prev_keycode) {
    switch (keycode) {
        case LT(_SYM, KC_SPACE):
        case MT(MOD_LCTL, KC_TAB):
            return 0;
    }
    if (is_flow_tap_key(keycode) && is_flow_tap_key(prev_keycode)) {
        return FLOW_TAP_TERM;
    }
    return 0;
}

// Tracks idle time for the caps-word combo's prior-idle guard, below. Only
// advances for keys that are NOT part of a combo: process_combo() runs
// before process_record_user() and returns false for combo-participating
// presses, so this only moves on genuinely idle-then-press activity.
static uint16_t last_key_time = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        last_key_time = timer_read();
    }

    if (keycode >= NUMF(KC_1) && keycode <= NUMF(KC_0)) {
        if (record->tap.count) {
            return true; // tap: let QMK send the digit itself (incl. Shift+1 = !)
        }
        uint8_t  i   = (keycode & 0xFF) - KC_1;
        uint16_t out = KC_F1 + i;
        if (record->event.pressed) {
            register_code(out);
        } else {
            unregister_code(out);
        }
        return false;
    }
    return true;
}

// The Totem's nine base-layer combos, on the same letter pairs. Home-row
// keys must be referenced by their MT() keycode, not the bare letter.
const uint16_t PROGMEM combo_caps[] = {MT(MOD_LSFT, KC_F), MT(MOD_RSFT, KC_J), COMBO_END};
const uint16_t PROGMEM combo_dquo[] = {MT(MOD_LALT, KC_S), MT(MOD_LCTL, KC_D), COMBO_END};
const uint16_t PROGMEM combo_quot[] = {MT(MOD_LCTL, KC_D), MT(MOD_LSFT, KC_F), COMBO_END};
const uint16_t PROGMEM combo_home[] = {MT(MOD_RSFT, KC_J), MT(MOD_RCTL, KC_K), COMBO_END};
const uint16_t PROGMEM combo_end[]  = {MT(MOD_RCTL, KC_K), MT(MOD_RALT, KC_L), COMBO_END};
const uint16_t PROGMEM combo_coln[] = {MT(MOD_RALT, KC_L), MT(MOD_RGUI, KC_SCLN), COMBO_END};
const uint16_t PROGMEM combo_unds[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM combo_mins[] = {KC_C, KC_V, COMBO_END};
const uint16_t PROGMEM combo_ques[] = {KC_DOT, KC_SLASH, COMBO_END};

combo_t key_combos[] = {
    [CB_CAPS] = COMBO(combo_caps, CW_TOGG),
    [CB_DQUO] = COMBO(combo_dquo, KC_DQUO),
    [CB_QUOT] = COMBO(combo_quot, KC_QUOTE),
    [CB_HOME] = COMBO(combo_home, KC_HOME),
    [CB_END]  = COMBO(combo_end,  KC_END),
    [CB_COLN] = COMBO(combo_coln, KC_COLN),
    [CB_UNDS] = COMBO(combo_unds, KC_UNDS),
    [CB_MINS] = COMBO(combo_mins, KC_MINUS),
    [CB_QUES] = COMBO(combo_ques, KC_QUES),
};

uint16_t get_combo_term(uint16_t combo_index, combo_t *combo) {
    switch (combo_index) {
        case CB_CAPS:
            return 50;
        default:
            return 35;
    }
}

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    // Base layer only — the equivalent of the Totem's `layers = <BASE>`.
    // This is what keeps every combo off the GAME layer.
    if (get_highest_layer(layer_state) != _BASE) {
        return false;
    }
    // require-prior-idle-ms = <100> on the Totem: stops F+J firing mid-word.
    if (combo_index == CB_CAPS && timer_elapsed(last_key_time) < 100) {
        return false;
    }
    return true;
}

// --- Per-layer RGB, carried over verbatim from the Oryx export ---

extern rgb_config_t rgb_matrix_config;

RGB hsv_to_rgb_with_value(HSV hsv) {
  RGB rgb = hsv_to_rgb( hsv );
  float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
  return (RGB){ f * rgb.r, f * rgb.g, f * rgb.b };
}

void keyboard_post_init_user(void) {
  rgb_matrix_enable();
}

// Status LEDs: one unique pattern per layer, following Oryx's convention
// (BASE off, GAME 1, SYM 2, NUM 3, MOUSE 1+2). bit0 = LED1 .. bit2 = LED3.
static const uint8_t layer_leds[] = {
    [_BASE] = 0, [_GAME] = 1, [_SYM] = 2, [_NUM] = 4, [_MOUSE] = 1 | 2,
};

layer_state_t layer_state_set_user(layer_state_t state) {
  uint8_t leds = layer_leds[get_highest_layer(state)];
  leds & 1 ? ergodox_right_led_1_on() : ergodox_right_led_1_off();
  leds & 2 ? ergodox_right_led_2_on() : ergodox_right_led_2_off();
  leds & 4 ? ergodox_right_led_3_on() : ergodox_right_led_3_off();
  return state;
}

const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3] = {
    [_SYM] = { {31,255,255}, {31,255,255}, {31,255,255}, {31,255,255}, {31,255,255}, {209,255,255}, {41,255,255}, {41,255,255}, {209,255,255}, {209,255,255}, {209,255,255}, {41,255,255}, {41,255,255}, {209,255,255}, {209,255,255}, {209,255,255}, {41,255,255}, {41,255,255}, {209,255,255}, {209,255,255}, {0,183,238}, {0,183,238}, {0,183,238}, {0,183,238}, {31,255,255}, {31,255,255}, {31,255,255}, {31,255,255}, {31,255,255}, {209,255,255}, {209,255,255}, {74,255,255}, {74,255,255}, {209,255,255}, {209,255,255}, {209,255,255}, {209,255,255}, {209,255,255}, {209,255,255}, {209,255,255}, {209,255,255}, {209,255,255}, {209,255,255}, {209,255,255}, {209,255,255}, {209,255,255}, {209,255,255}, {209,255,255} },

    [_NUM] = { {98,255,255}, {31,255,255}, {31,255,255}, {31,255,255}, {98,255,255}, {98,255,255}, {31,255,255}, {31,255,255}, {31,255,255}, {98,255,255}, {98,255,255}, {31,255,255}, {31,255,255}, {31,255,255}, {98,255,255}, {10,225,255}, {98,255,255}, {31,255,255}, {98,255,255}, {98,255,255}, {0,0,255}, {0,0,255}, {0,0,255}, {0,0,255}, {98,255,255}, {98,255,255}, {98,255,255}, {98,255,255}, {98,255,255}, {98,255,255}, {98,255,255}, {98,255,255}, {98,255,255}, {98,255,255}, {98,255,255}, {0,183,238}, {98,255,255}, {0,183,238}, {98,255,255}, {98,255,255}, {98,255,255}, {98,255,255}, {98,255,255}, {98,255,255}, {98,255,255}, {98,255,255}, {98,255,255}, {98,255,255} },

    [_MOUSE] = { {30,96,255}, {30,96,255}, {30,96,255}, {30,96,255}, {30,96,255}, {169,120,255}, {169,120,255}, {31,255,255}, {195,255,255}, {195,255,255}, {169,120,255}, {31,255,255}, {31,255,255}, {31,255,255}, {146,224,255}, {195,255,255}, {15,166,195}, {15,166,195}, {15,166,195}, {146,224,255}, {10,225,255}, {10,225,255}, {10,225,255}, {10,225,255}, {30,96,255}, {30,96,255}, {30,96,255}, {30,96,255}, {30,96,255}, {195,255,255}, {195,255,255}, {10,225,255}, {195,255,255}, {195,255,255}, {195,255,255}, {10,225,255}, {10,225,255}, {10,225,255}, {195,255,255}, {195,255,255}, {195,255,255}, {195,255,255}, {195,255,255}, {195,255,255}, {195,255,255}, {195,255,255}, {195,255,255}, {195,255,255} },

};

void set_layer_color(int layer) {
  for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
    HSV hsv = {
      .h = pgm_read_byte(&ledmap[layer][i][0]),
      .s = pgm_read_byte(&ledmap[layer][i][1]),
      .v = pgm_read_byte(&ledmap[layer][i][2]),
    };
    if (!hsv.h && !hsv.s && !hsv.v) {
        rgb_matrix_set_color( i, 0, 0, 0 );
    } else {
        RGB rgb = hsv_to_rgb_with_value(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
  }
}

bool rgb_matrix_indicators_user(void) {
  if (rawhid_state.rgb_control) {
      return false;
  }
  if (!keyboard_config.disable_layer_led) {
    switch (biton32(layer_state)) {
      case _SYM:
        set_layer_color(_SYM);
        break;
      case _NUM:
        set_layer_color(_NUM);
        break;
      case _MOUSE:
        set_layer_color(_MOUSE);
        break;
     default:
        if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
          rgb_matrix_set_color_all(0, 0, 0);
        }
    }
  } else {
    if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
      rgb_matrix_set_color_all(0, 0, 0);
    }
  }

  return true;
}
