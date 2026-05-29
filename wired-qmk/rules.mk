# Additions for the per-layer-colour wired build. Merge these into the rules.mk
# you get from the crkbd `vial` keymap (see README) — do not lose VIAL_ENABLE.
VIA_ENABLE = yes
VIAL_ENABLE = yes
RGBLIGHT_ENABLE = yes
OLED_ENABLE = no
# LTO keeps the firmware inside the atmega32u4's ~28 KB flash alongside Vial.
LTO_ENABLE = yes
