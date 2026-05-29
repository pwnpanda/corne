#pragma once

// Append these to the config.h from the crkbd `vial` keymap (keep that file's
// Vial UID + unlock-combo lines — don't replace them).

// Underglow: static colour only. Drop every RGBLIGHT animation to save flash;
// the per-layer colour hook in keymap.c only needs a solid colour.
#undef RGBLIGHT_ANIMATIONS
#undef RGBLIGHT_EFFECT_BREATHING
#undef RGBLIGHT_EFFECT_RAINBOW_MOOD
#undef RGBLIGHT_EFFECT_RAINBOW_SWIRL
#undef RGBLIGHT_EFFECT_SNAKE
#undef RGBLIGHT_EFFECT_KNIGHT
#undef RGBLIGHT_EFFECT_CHRISTMAS
#undef RGBLIGHT_EFFECT_STATIC_GRADIENT
#undef RGBLIGHT_EFFECT_RGB_TEST
#undef RGBLIGHT_EFFECT_ALTERNATING
#undef RGBLIGHT_EFFECT_TWINKLE

// Small flash savers.
#define LAYER_STATE_8BIT
#define NO_MUSIC_MODE
