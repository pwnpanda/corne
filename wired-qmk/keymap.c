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
// No tri-layer: layers are reached directly via TO()/MO() (incl. the imported
// Vial layout's TO(3)). Forcing tri-layer would clear layer 3 the instant
// TO(3) set it, which is why layer 3 looked dead.
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
// Both OLEDs in PORTRAIT (text reads down the long edge), same rotation so the
// slave isn't upside-down. Flip to OLED_ROTATION_90 if it ever mounts inverted.
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

// IMPORTANT: rendering uses ONLY oled_write_char(). In rotated mode the
// line-write and cursor-positioning helpers pad/index using the un-rotated
// grid, overrun the framebuffer, and crash. The display is 5 chars x 16 rows;
// we paint all 80 cells every frame, so it self-clears and the cursor cycles.
#define OLED_COLS 5
#define OLED_ROWS 16

static void str_append(char *dst, const char *src) {
    while (*dst) dst++;
    while (*src) *dst++ = *src;
    *dst = '\0';
}

static uint8_t slen(const char *s) {
    uint8_t n = 0;
    while (*s++) n++;
    return n;
}

// Map a basic keycode to a printable char.
static const char code_to_name[60] PROGMEM = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', ' ', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

// Last 4 printable keys (oldest -> newest) and the locks string live in bss,
// not on the stack.
static char last_keys[4] = {' ', ' ', ' ', ' '};
static char locks[16];

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
            (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) {
            keycode &= 0xFF;
        }
        if (keycode < 60) {
            last_keys[0] = last_keys[1];
            last_keys[1] = last_keys[2];
            last_keys[2] = last_keys[3];
            last_keys[3] = pgm_read_byte(&code_to_name[keycode]);
        }
    }
    return true;
}

static char char_at(const char *s, uint8_t len, uint8_t i) {
    return i < len ? s[i] : ' ';
}

bool oled_task_user(void) {
    // Throttle to ~20 Hz. Repainting all 80 cells every scan cycle (plus the
    // blocking I2C flush) starved the matrix scan / split comms, causing the
    // keyboard to lag then freeze after a few keystrokes. Skip most cycles.
    static uint16_t last_draw = 0;
    if (timer_elapsed(last_draw) < 50) {
        return false;
    }
    last_draw = timer_read();

    if (is_keyboard_master()) {
        const char *layer;
        switch (get_highest_layer(layer_state)) {
            case 1:  layer = "NUMBER"; break;
            case 2:  layer = "SYMBOL"; break;
            case 3:  layer = "MISC";   break;
            default: layer = "BASE";   break;
        }
        locks[0] = '\0';
        led_t led = host_keyboard_led_state();
        if (led.caps_lock)                  str_append(locks, "Caps");
        if (led.num_lock)   { if (locks[0]) str_append(locks, "-"); str_append(locks, "Num"); }
        if (led.scroll_lock){ if (locks[0]) str_append(locks, "-"); str_append(locks, "Scrl"); }

        uint8_t ll = slen(layer);
        uint8_t lc = slen(locks);
        uint8_t rows = ll;
        if (4  > rows) rows = 4;
        if (lc > rows) rows = lc;
        uint8_t top = (OLED_ROWS - rows) / 2;   // vertical centering

        for (uint8_t y = 0; y < OLED_ROWS; y++) {
            uint8_t r = (y >= top) ? (uint8_t)(y - top) : 255;
            oled_write_char(char_at(layer, ll, r), false);
            oled_write_char(' ', false);
            oled_write_char(char_at(last_keys, 4, r), false);
            oled_write_char(' ', false);
            oled_write_char(char_at(locks, lc, r), false);
        }
    } else {
        const char *s = "corne";
        uint8_t top = (OLED_ROWS - 5) / 2;
        for (uint8_t y = 0; y < OLED_ROWS; y++) {
            uint8_t r = (y >= top) ? (uint8_t)(y - top) : 255;
            oled_write_char(char_at(s, 5, r), false);
            oled_write_char(' ', false);
            oled_write_char(' ', false);
            oled_write_char(' ', false);
            oled_write_char(' ', false);
        }
    }
    return false;
}
#endif
