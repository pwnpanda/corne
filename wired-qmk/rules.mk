# Merge these into the rules.mk you get from the crkbd `vial` keymap.

# ---- Layout source: pick ONE -------------------------------------------------
# Option A (default): keep Vial GUI editing. Tightest fit on AVR; the trims
# below are what make the OLED fit alongside it.
VIA_ENABLE = yes
VIAL_ENABLE = yes
# Option B: drop Vial for plain QMK. Frees ~5-10 KB (OLED fits with room to
# spare), but you then edit keymap.c + recompile to change the layout. To use
# it, set BOTH lines above to `no` and ignore the crkbd vial keymap's
# vial.json / UID / unlock-combo.

# ---- Lighting: underglow only, no per-key matrix -----------------------------
RGBLIGHT_ENABLE = yes
RGB_MATRIX_ENABLE = no

# ---- Displays: the goal of these trims ---------------------------------------
OLED_ENABLE = yes

# ---- Flash trims (atmega32u4 has ~28 KB usable) ------------------------------
MOUSEKEY_ENABLE = no   # mouse keys unused
NKRO_ENABLE = no       # 6KRO is enough; saves flash
MAGIC_ENABLE = no      # QMK "magic" keycodes unused (EE_HANDS is unaffected)
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
LTO_ENABLE = yes       # biggest single saving (~20%)
