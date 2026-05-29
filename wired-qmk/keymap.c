/* Corne Choc v2.5 (Keebmaker) — wired QMK keymap for atmega32u4 Pro Micro C.
 *
 * Purpose of this branch: test the per-layer underglow-colour feature on the
 * EXISTING wired hardware, which is AVR and cannot run ZMK. The wireless ZMK
 * build lives on `main`.
 *
 * The layout below is only a fallback default. Your real layout lives in the
 * keyboard's Vial EEPROM (and in corne.vil) and is unaffected by this file —
 * the only thing that matters here is layer_state_set_user(), which colours the
 * underglow per layer. Build against Vial-QMK so Vial GUI editing still works.
 *
 * Layers: 0 base / 1 number / 2 symbol / 3 misc.
 */

#include QMK_KEYBOARD_H

// HSV in QMK is 0-255 per channel. Hues: red 0, green 85, blue 170.
#define HSV_BASE   22,  255, 255  // vivid orange = hsv(31deg, 100%, 100%) on QMK's 0-255 scale
#define HSV_LOWER  170, 255, 150 // blue
#define HSV_RAISE  85,  255, 150 // green
#define HSV_ADJUST 0,   255, 150 // red

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
        KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,        KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_BSPC,
        KC_LCTL, KC_A,    KC_R,    KC_S,    KC_T,    KC_G,        KC_M,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,        KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH, KC_ESC,
                                   KC_LGUI, MO(1),   KC_SPC,      KC_ENT,  MO(2),   KC_RALT
    ),
    [1] = LAYOUT_split_3x6_3(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
        KC_LCTL, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_TRNS, KC_TRNS,
        KC_LSFT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_TRNS, KC_TRNS,
                                   KC_LGUI, KC_TRNS, KC_SPC,      KC_ENT,  MO(3),   KC_RALT
    ),
    [2] = LAYOUT_split_3x6_3(
        KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,     KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,
        KC_LCTL, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS, KC_GRV,
        KC_LSFT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
                                   KC_LGUI, MO(3),   KC_SPC,      KC_ENT,  KC_TRNS, KC_RALT
    ),
    [3] = LAYOUT_split_3x6_3(
        QK_BOOT, RGB_TOG, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, RGB_RMOD,RGB_HUD, RGB_SAD, RGB_VAD, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                   KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS
    ),
};

void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_sethsv_noeeprom(HSV_BASE);
}

// Colour follows the highest active layer. _noeeprom avoids flash wear.
// No tri-layer here: layers are reached directly via TO()/MO() (including your
// imported Vial layout's TO(3)). Forcing tri-layer would clear layer 3 the
// instant TO(3) set it, which is why layer 3 looked dead.
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case 1:  rgblight_sethsv_noeeprom(HSV_LOWER);  break;
        case 2:  rgblight_sethsv_noeeprom(HSV_RAISE);  break;
        case 3:  rgblight_sethsv_noeeprom(HSV_ADJUST); break;
        default: rgblight_sethsv_noeeprom(HSV_BASE);   break;
    }
    return state;
}

#ifdef OLED_ENABLE
// The OLED is portrait (~5 chars wide), so lines stack top-to-bottom. The
// master shows, stacked: layer name, Caps state, last key pressed. We render
// ASCII (lib/glcdfont.c has no logo glyphs, which is why the slave's default
// logo render showed "snow"); returning false skips that render.

// Map a basic keycode to a printable char for the "last key" line.
static const char code_to_name[60] PROGMEM = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', ' ', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};
static uint8_t last_kc = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        // Strip mod-tap / layer-tap down to the tapped keycode.
        if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
            (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) {
            keycode &= 0xFF;
        }
        if (keycode < 60) last_kc = keycode;
    }
    return true;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_write_ln_P(PSTR("Layer"), false);
        switch (get_highest_layer(layer_state)) {
            case 1:  oled_write_ln_P(PSTR("Number"), false); break;
            case 2:  oled_write_ln_P(PSTR("Symbol"), false); break;
            case 3:  oled_write_ln_P(PSTR("Misc"),   false); break;
            default: oled_write_ln_P(PSTR("Base"),   false); break;
        }
        oled_write_ln_P(PSTR(""), false);
        oled_write_ln_P(PSTR("Caps"), false);
        oled_write_ln_P(host_keyboard_led_state().caps_lock ? PSTR("ON") : PSTR("off"), false);
        oled_write_ln_P(PSTR(""), false);
        oled_write_ln_P(PSTR("Key"), false);
        oled_write_char(pgm_read_byte(&code_to_name[last_kc]), false);
    } else {
        oled_write_P(PSTR("corne\nwired"), false);
    }
    return false;
}
#endif
