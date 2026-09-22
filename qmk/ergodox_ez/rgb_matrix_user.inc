// The only RGB effect this keymap builds — config.h compiles out every stock
// animation, and keyboard_post_init_user() in keymap.c selects this one.
//
// Every layer rests on its own color, so the active one is obvious at a
// glance: BASE a soft warm white, GAME a rainbow drifting slowly left to
// right, the others a pastel two-hue gradient each. On BASE the same
// rainbow drifts underneath the white: a pressed key shows the rainbow's
// color at its spot, then fades back. On GAME a pressed key flashes
// brighter in its own rainbow color.
RGB_MATRIX_EFFECT(LAYER_GLOW)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#    include "action_layer.h"

#    define LAYER_GLOW_VAL 110        // brightness of the resting colors
#    define LAYER_GLOW_REVEAL_VAL 230 // brightness of a key's rainbow flash
#    define LAYER_GLOW_FADE_SHIFT 2   // flash fades over 256 << 2 ms, ~1 s
#    define LAYER_GLOW_DRIFT_SHIFT 6  // rainbow moves 1 hue step per 64 ms: a cycle every ~16 s

// Hue at the left and right edge of the board (ignored when `rainbow` is
// set: the layer rests on the drifting rainbow instead), saturation, and
// whether pressed keys flash the rainbow at full brightness — indexed by
// layer in the order of keymap.c's `enum layers` (not visible from here).
// QMK hues run 0-255: red 0, orange 21, yellow 43, green 85, cyan 128,
// blue 170, purple 191, pink 213.
static const struct {
    uint8_t h_left, h_right, s;
    bool    rainbow, reveal;
} layer_glow[] = {
    {28, 28, 40, false, true},      // BASE:  warm white
    {0, 0, 255, true, true},        // GAME:  rainbow
    {185, 225, 150, false, false},  // SYM:   lavender -> pink
    {80, 120, 150, false, false},   // NUM:   mint -> aqua
    {140, 175, 150, false, false},  // MOUSE: sky -> periwinkle
};

static bool LAYER_GLOW(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t layer = get_highest_layer(layer_state);
    if (layer >= ARRAY_SIZE(layer_glow)) layer = 0;
    int16_t h_left = layer_glow[layer].h_left;
    int16_t h_span = layer_glow[layer].h_right - h_left;
    uint8_t drift  = g_rgb_timer >> LAYER_GLOW_DRIFT_SHIFT;

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        uint8_t x   = g_led_config.point[i].x; // 0 (left edge) .. 224 (right edge)
        uint8_t hue = layer_glow[layer].rainbow ? x - drift : h_left + h_span * x / 224;
        rgb_t   rgb = rgb_matrix_hsv_to_rgb((hsv_t){hue, layer_glow[layer].s, LAYER_GLOW_VAL});

        if (layer_glow[layer].reveal) {
            // Most recent press of this key, in ms ago — same search as
            // QMK's effect_runner_reactive().
            uint16_t tick = UINT16_MAX;
            for (int8_t j = g_last_hit_tracker.count - 1; j >= 0; j--) {
                if (g_last_hit_tracker.index[j] == i) {
                    tick = g_last_hit_tracker.tick[j];
                    break;
                }
            }
            uint16_t fade = tick >> LAYER_GLOW_FADE_SHIFT;
            if (fade < 255) {
                rgb_t   rainbow = rgb_matrix_hsv_to_rgb((hsv_t){x - drift, 255, LAYER_GLOW_REVEAL_VAL});
                uint8_t amount  = 255 - fade;
                rgb.r           = lerp8by8(rgb.r, rainbow.r, amount);
                rgb.g           = lerp8by8(rgb.g, rainbow.g, amount);
                rgb.b           = lerp8by8(rgb.b, rainbow.b, amount);
            }
        }
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
