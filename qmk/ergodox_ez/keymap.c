// ErgoDox EZ Glow port of ../../config/totem.keymap — same layers, same
// home-row-mod behaviour and combos as the TOTEM, using the ErgoDox's extra
// keys (number row, arrows, outer/side columns) for what the Totem lacks.
// Each LAYOUT_ergodox_pretty() row below combines both halves on one line,
// left-to-right, matching the original Oryx export's argument order; the
// thumb cluster follows as three groups (top pair, inner pair, bottom
// triple), left half then right half within each group.

#include QMK_KEYBOARD_H

enum layers {
    _BASE = 0,
    _GAME,
    _SYM,
    _NUM,
    _ADJ,
    _FUN,
};

enum combo_names {
    CB_CAPS = 0,
    CB_UNDS,
    CB_HOME,
    CB_END,
};

// Tap = digit, hold = F-key. LT(0, kc) is QMK's documented idiom for a
// layer-tap with no practical layer effect (layer 0 is always active), used
// here purely as a carrier for the tap-hold timing machinery;
// process_record_user() below intercepts it before layer 0 would "activate".
#define NUMF(kc) LT(0, kc)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_ergodox_pretty(
        KC_ESC,   NUMF(KC_1), NUMF(KC_2), NUMF(KC_3), NUMF(KC_4), NUMF(KC_5), KC_LSFT,                                CW_TOGG,  NUMF(KC_6), NUMF(KC_7), NUMF(KC_8), NUMF(KC_9), NUMF(KC_0), KC_EQL,
        KC_TAB,   KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_HOME,                                KC_PGUP,  KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_DEL,
        KC_LCTL,  MT(MOD_LGUI, KC_A), MT(MOD_LALT, KC_S), MT(MOD_LCTL, KC_D), MT(MOD_LSFT, KC_F), KC_G,                            KC_H,     MT(MOD_RSFT, KC_J), MT(MOD_RCTL, KC_K), MT(MOD_RALT, KC_L), MT(MOD_RGUI, KC_SCLN), TG(_GAME),
        MO(_FUN), KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_END,                                 KC_PGDN,  KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    MO(_ADJ),
        LCTL(KC_LSFT), LCTL(KC_LALT), LALT(KC_LSFT), KC_LALT, MT(MOD_LGUI, KC_ESC),                                                        KC_LEFT,    KC_UP,      KC_DOWN,    KC_RGHT,    KC_RGUI,

                                          KC_AUDIO_VOL_DOWN, KC_AUDIO_VOL_UP,    KC_PSCR,          LSFT(KC_PSCR),
                                                    KC_AUDIO_MUTE,               KC_CAPS,
                        MT(MOD_LGUI, KC_ESC), LT(_SYM, KC_SPC), KC_ENT,          KC_BSPC, LT(_NUM, KC_TAB), KC_DEL
    ),

    [_GAME] = LAYOUT_ergodox_pretty(
        KC_ESC,   KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_LSFT,                                KC_TRNS,  KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_EQL,
        KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                                KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_LCTL,  KC_A,       KC_S,       KC_D,       KC_F,       KC_G,                                                          KC_H,     KC_J,       KC_K,       KC_L,       KC_SCLN,    TG(_GAME),
        KC_LSFT,  KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_TRNS,                                KC_TRNS,  KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    KC_TRNS,
        LCTL(KC_LSFT), LCTL(KC_LALT), LALT(KC_LSFT), KC_LALT, KC_LGUI,                                                                     KC_LEFT,    KC_UP,      KC_DOWN,    KC_RGHT,    KC_RGUI,

                                          KC_TRNS,            KC_TRNS,            KC_TRNS,          KC_TRNS,
                                                    KC_TRNS,                     KC_TRNS,
                        KC_TRNS, KC_SPC, KC_TRNS,             KC_TAB, KC_BSPC, KC_TRNS
    ),

    [_SYM] = LAYOUT_ergodox_pretty(
        KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                                KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,  KC_GRV,     KC_LT,      KC_GT,      KC_MINS,    KC_PIPE,    KC_TRNS,                                KC_TRNS,  KC_CIRC,    KC_LCBR,    KC_RCBR,    KC_DLR,     KC_AMPR,    KC_TRNS,
        KC_TRNS,  KC_EXLM,    KC_ASTR,    KC_SLSH,    KC_EQL,     KC_QUES,                                                       KC_HASH,  KC_LPRN,    KC_RPRN,    KC_SCLN,    KC_DQT,     KC_TRNS,
        KC_TRNS,  KC_TILD,    KC_PLUS,    KC_COLN,    KC_UNDS,    KC_PERC,    KC_TRNS,                                KC_TRNS,  KC_AT,      KC_LBRC,    KC_RBRC,    KC_BSLS,    KC_QUOT,    KC_TRNS,
        KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                                                                             KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,

                                          KC_TRNS,            KC_TRNS,            KC_TRNS,          KC_TRNS,
                                                    KC_TRNS,                     KC_TRNS,
                        KC_TRNS, KC_TRNS, KC_TRNS,             KC_TRNS, KC_TRNS, KC_TRNS
    ),

    [_NUM] = LAYOUT_ergodox_pretty(
        KC_TRNS,  KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_TRNS,                                KC_TRNS,  KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_TRNS,
        KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                                KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,  KC_LGUI,    KC_LALT,    KC_LCTL,    KC_LSFT,    KC_MINS,                                                       KC_PGUP,  KC_RSFT,    KC_RCTL,    KC_RALT,    KC_RGUI,    KC_TRNS,
        MO(_FUN), KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                                KC_TRNS,  KC_PGDN,    KC_LEFT,    KC_UP,      KC_DOWN,    KC_RGHT,    KC_TRNS,
        KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                                                                             KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,

                                          KC_TRNS,            KC_TRNS,            KC_TRNS,          KC_TRNS,
                                                    KC_TRNS,                     KC_TRNS,
                        KC_TRNS, KC_TRNS, KC_TRNS,             KC_TRNS, KC_TRNS, KC_TRNS
    ),

    [_ADJ] = LAYOUT_ergodox_pretty(
        KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                                KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,  RGB_TOG,    RGB_MOD,    RGB_VAI,    RGB_VAD,    RGB_HUI,    KC_TRNS,                                KC_TRNS,  KC_AUDIO_MUTE, KC_BRIGHTNESS_DOWN, KC_BRIGHTNESS_UP, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,  KC_TRNS,    LALT(LCTL(KC_MINS)), KC_TRNS, LCTL(LSFT(KC_MINS)), KC_TRNS,                                        KC_MEDIA_PLAY_PAUSE, KC_MEDIA_PREV_TRACK, KC_AUDIO_VOL_UP, KC_AUDIO_VOL_DOWN, KC_MEDIA_NEXT_TRACK, KC_TRNS,
        KC_TRNS,  QK_BOOT,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                                KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                                                                             KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,

                                          KC_TRNS,            KC_TRNS,            KC_TRNS,          KC_TRNS,
                                                    KC_TRNS,                     KC_TRNS,
                        KC_TRNS, KC_TRNS, KC_TRNS,             KC_TRNS, KC_TRNS, KC_TRNS
    ),

    [_FUN] = LAYOUT_ergodox_pretty(
        KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                                KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,  KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_TRNS,                                KC_TRNS,  KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_TRNS,
        KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                                                       KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_F11,
        KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                                KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_F12,
        KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                                                                             KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,

                                          KC_TRNS,            KC_TRNS,            KC_TRNS,          KC_TRNS,
                                                    KC_TRNS,                     KC_TRNS,
                        KC_TRNS, KC_TRNS, KC_TRNS,             KC_TRNS, KC_TRNS, KC_TRNS
    ),
};

// Chordal Hold's per-key handedness map — the QMK equivalent of the Totem's
// hold-trigger-key-positions bilateral guard. '*' (thumbs) may settle as
// held in a chord with either hand, matching THUMBS being listed in both
// hml's and hmr's trigger sets on the Totem. Same row/thumb grouping and
// argument order as the keymaps above.
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT_ergodox_pretty(
    'L', 'L', 'L', 'L', 'L', 'L', 'L',                                        'R', 'R', 'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L', 'L', 'L',                                        'R', 'R', 'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L', 'L',                                                       'R', 'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L', 'L', 'L',                                        'R', 'R', 'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L',                                                                 'R', 'R', 'R', 'R', 'R',

                             '*', '*',    '*', '*',
                                  '*',    '*',
                    '*', '*', '*',        '*', '*', '*'
);

// The Totem's home-row mods are `balanced` (~= PERMISSIVE_HOLD) but its
// thumb `&lt` keys stay `tap-preferred`, because with `balanced`, rolling
// from Space into the next word's first letter falsely resolves as a hold
// and fires SYM. Opt the two thumb LT keys out of PERMISSIVE_HOLD and Flow
// Tap to reproduce that.
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(_SYM, KC_SPC):
        case LT(_NUM, KC_TAB):
            return false; // tap-preferred, protects the Space-roll case
    }
    if (keycode >= NUMF(KC_1) && keycode <= NUMF(KC_0)) {
        return false; // tap-preferred, so fast digit rolls stay digits
    }
    return true; // balanced, for the home row mods
}

// The Totem's &lt has no require-prior-idle-ms — only hml/hmr do. KC_SPC is
// in QMK's default is_flow_tap_key() set, so without this override SYM would
// be unreachable for FLOW_TAP_TERM after every letter.
uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t *record, uint16_t prev_keycode) {
    switch (keycode) {
        case LT(_SYM, KC_SPC):
        case LT(_NUM, KC_TAB):
            return 0;
    }
    if (is_flow_tap_key(keycode) && is_flow_tap_key(prev_keycode)) {
        return FLOW_TAP_TERM;
    }
    return 0;
}

// Tracks idle time for the caps-word combo's prior-idle guard, below. Only
// updated for keys that are NOT part of a combo: process_combo() runs
// before process_record_user() and returns false for combo-participating
// presses, so this only advances on genuinely idle-then-press activity.
static uint16_t last_key_time = 0;

// Tap = digit, hold = F-key, on both number-row halves. Replaces the
// original Oryx export's ten copy-pasted DUAL_FUNC_n blocks (which also
// used dummy layer numbers up to 15, out of range under LAYER_STATE_8BIT).
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

const uint16_t PROGMEM combo_caps[] = {MT(MOD_LSFT, KC_F), MT(MOD_RSFT, KC_J), COMBO_END};
const uint16_t PROGMEM combo_unds[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM combo_home[] = {MT(MOD_RSFT, KC_J), MT(MOD_RCTL, KC_K), COMBO_END};
const uint16_t PROGMEM combo_end[]  = {MT(MOD_RCTL, KC_K), MT(MOD_RALT, KC_L), COMBO_END};

combo_t key_combos[] = {
    [CB_CAPS] = COMBO(combo_caps, CW_TOGG),
    [CB_UNDS] = COMBO(combo_unds, KC_UNDS),
    [CB_HOME] = COMBO(combo_home, KC_HOME),
    [CB_END]  = COMBO(combo_end,  KC_END),
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
    if (layer_state_is(_GAME)) {
        return false;
    }
    if (combo_index == CB_CAPS && timer_elapsed(last_key_time) < 100) {
        return false;
    }
    return true;
}
