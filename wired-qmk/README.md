# Wired QMK build (per-layer colour on the existing AVR hardware)

This branch is for the **current wired Corne** — Keebmaker Pro Micro C
(atmega32u4, AVR). ZMK cannot run on AVR, so this is a **Vial-QMK** build whose
only custom behaviour is changing the underglow colour per layer. Use it to try
the colour feature today, before committing to the wireless nice!nano + ZMK
build on `main`.

Colours: base = dim white, lower = blue, raise = green, adjust = red.

## Why so little code?

Because you use Vial, your real layout lives in the keyboard's EEPROM (and in
`corne.vil`) — not in the firmware. So this build only adds
`layer_state_set_user()` (the colour hook) on top of a normal Vial build. The
4-layer keymap in `keymap.c` is just a fallback default if the EEPROM is ever
reset; your Vial layout overrides it at runtime.

## Build

You must use the **Vial-QMK** fork (mainline QMK has no `VIAL_ENABLE`).

```bash
# 1. Get Vial-QMK
git clone https://github.com/vial-kb/vial-qmk.git
cd vial-qmk
make git-submodule

# 2. Start from the working crkbd `vial` keymap (it already has a valid
#    vial.json, Vial UID, and unlock combo), then drop in our colour code.
cp -r keyboards/crkbd/keymaps/vial keyboards/crkbd/keymaps/roba-wired
cp /path/to/this/repo/wired-qmk/keymap.c keyboards/crkbd/keymaps/roba-wired/keymap.c

# 3. Make sure these are set in that keymap's rules.mk
#    (see wired-qmk/rules.mk — keep the VIAL_ENABLE line that's already there):
#    VIAL_ENABLE = yes
#    RGBLIGHT_ENABLE = yes
#    OLED_ENABLE = no
#    LTO_ENABLE = yes

# 4. Compile (Pro Micro = crkbd/rev1)
qmk compile -kb crkbd/rev1 -km roba-wired
```

If `crkbd/rev1` errors, check the exact target on Keebmaker's default keymap
page; some kits use a specific revision string.

## Flash

Produces a `.hex` (one image; flash it to **both** halves).

```bash
qmk flash -kb crkbd/rev1 -km roba-wired
```

Or use **QMK Toolbox**: load the `.hex`, put one half into the bootloader
(short the `RST`+`GND` pads twice, or press reset), flash, then repeat for the
other half.

## After flashing

- If the layout looks wrong/reset, open **Vial** and re-import `corne.vil` — the
  colour hook is in firmware and works regardless of which layout is loaded.
- Hold **lower + raise** together to reach the adjust layer (red).

## Flash budget caveat

atmega32u4 has ~28 KB usable flash. Vial + RGB underglow + this colour hook
fits (it's what your current build already is, plus a few bytes). **OLED is off
on purpose** — adding it on top of Vial likely overflows, which is why your
generic Vial flash already dropped the screens. If you want OLEDs back, that's a
strong reason to move to the nice!nano/ZMK build on `main`, where there's room.
