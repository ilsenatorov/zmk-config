/*
 * Types the battery level of each half as text, e.g. "L87 R64", so the right
 * half's level is readable without the LED widget or the host's battery
 * indicator (which only ever sees the left half, see config/totem.conf).
 * "R?" means the central has no level for the peripheral: it is
 * disconnected, or hasn't reported yet since connecting.
 */

#define DT_DRV_COMPAT zmk_behavior_battery_type

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>

#include <dt-bindings/zmk/keys.h>
#include <zmk/behavior.h>
#include <zmk/behavior_queue.h>
#include <zmk/battery.h>

#if IS_ENABLED(CONFIG_ZMK_SPLIT_BLE_CENTRAL_BATTERY_LEVEL_FETCHING)
#include <zmk/split/central.h>
#endif

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

struct behavior_battery_type_config {
    uint32_t tap_ms;
    uint32_t wait_ms;
};

static const uint32_t digits[] = {N0, N1, N2, N3, N4, N5, N6, N7, N8, N9};

static void type_key(const struct behavior_battery_type_config *cfg,
                     const struct zmk_behavior_binding_event *event, uint32_t keycode) {
    const struct zmk_behavior_binding kp = {
        .behavior_dev = DEVICE_DT_NAME(DT_NODELABEL(kp)),
        .param1 = keycode,
    };
    zmk_behavior_queue_add(event, kp, true, cfg->tap_ms);
    zmk_behavior_queue_add(event, kp, false, cfg->wait_ms);
}

static void type_level(const struct behavior_battery_type_config *cfg,
                       const struct zmk_behavior_binding_event *event, uint8_t level) {
    if (level >= 100) {
        type_key(cfg, event, digits[level / 100]);
    }
    if (level >= 10) {
        type_key(cfg, event, digits[(level / 10) % 10]);
    }
    type_key(cfg, event, digits[level % 10]);
}

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    const struct behavior_battery_type_config *cfg = dev->config;

    type_key(cfg, &event, LS(L));
#if IS_ENABLED(CONFIG_ZMK_BATTERY_REPORTING)
    type_level(cfg, &event, zmk_battery_state_of_charge());
#else
    type_key(cfg, &event, QMARK);
#endif

#if IS_ENABLED(CONFIG_ZMK_SPLIT_BLE_CENTRAL_BATTERY_LEVEL_FETCHING)
    for (uint8_t source = 0; source < ZMK_SPLIT_CENTRAL_PERIPHERAL_COUNT; source++) {
        uint8_t level = 0;
        type_key(cfg, &event, SPACE);
        type_key(cfg, &event, LS(R));
        // ZMK stores 0 until the first report, and resets to 0 on disconnect.
        if (zmk_split_central_get_peripheral_battery_level(source, &level) == 0 && level > 0) {
            type_level(cfg, &event, level);
        } else {
            type_key(cfg, &event, QMARK);
        }
    }
#endif

    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_battery_type_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
    // Only the central knows both levels, and owns the HID connection.
    .locality = BEHAVIOR_LOCALITY_CENTRAL,
#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
    .get_parameter_metadata = zmk_behavior_get_empty_param_metadata,
#endif
};

#define BATTERY_TYPE_INST(n)                                                                       \
    static const struct behavior_battery_type_config behavior_battery_type_config_##n = {         \
        .tap_ms = DT_INST_PROP(n, tap_ms),                                                         \
        .wait_ms = DT_INST_PROP(n, wait_ms),                                                       \
    };                                                                                             \
    BEHAVIOR_DT_INST_DEFINE(n, NULL, NULL, NULL, &behavior_battery_type_config_##n, POST_KERNEL,  \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                                   \
                            &behavior_battery_type_driver_api);

DT_INST_FOREACH_STATUS_OKAY(BATTERY_TYPE_INST)
