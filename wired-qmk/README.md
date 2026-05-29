# Wired QMK build (per-layer colour on the existing AVR hardware)

This branch is for the **current wired Corne** — Keebmaker Pro Micro C
(atmega32u4, AVR). ZMK cannot run on AVR, so this is a QMK build whose only
custom behaviour is changing the underglow colour per layer. Use it to try the
colour feature today, before committing to the wireless nice!nano + ZMK build
on `main`.

Colours: base = dim white, lower = blue, raise = green, adjust = red (edit the
`HSV_*` defines at the top of `keymap.c` — see "Setting the colours" below).

## Why so little code?

Your real layout lives in the keyboard's EEPROM (and in `corne.vil`), not in the
firmware. So this build only adds `layer_state_set_user()` (the colour hook).
The 4-layer keymap in `keymap.c` is just a fallback default if the EEPROM is
reset; in the Vial build your Vial layout overrides it at runtime.

## Two ways to build — pick one

The atmega32u4 has only **~28 KB** usable flash, and the OLED is what makes it
tight. You have two options:

### Option A — keep Vial (GUI editing), with trims to fit the OLED
Vial costs the most flash, so `wired-qmk/rules.mk` and `wired-qmk/config.h`
strip what you don't use (per-key RGB matrix, RGB animations, mouse keys, NKRO,
magic keycodes) to make room. It *should* fit; if the build overflows, the
compiler tells you by how much and you trim more (or switch to Option B).

### Option B — drop Vial, plain QMK (most space, no GUI)
Set `VIA_ENABLE = no` and `VIAL_ENABLE = no` in `rules.mk`. This frees ~5–10 KB,
so the OLED fits easily with headroom. The cost: no Vial GUI — you change the
layout by editing `keymap.c` and recompiling. Your full layout is already in
`keymap.c`, so it's ready to go.

## Build

Use the **Vial-QMK** fork for Option A (mainline QMK has no `VIAL_ENABLE`);
either fork works for Option B.

```bash
# 1. Get Vial-QMK
git clone https://github.com/vial-kb/vial-qmk.git
cd vial-qmk
make git-submodule

# 2. Start from the working crkbd `vial` keymap (it already has a valid
#    vial.json, Vial UID, and unlock combo), then overlay our files.
cp -r keyboards/crkbd/keymaps/vial keyboards/crkbd/keymaps/roba-wired
cp /path/to/this/repo/wired-qmk/keymap.c keyboards/crkbd/keymaps/roba-wired/keymap.c

# 3. Apply the rules.mk lines from wired-qmk/rules.mk (keep VIAL_ENABLE for
#    Option A, or set VIA/VIAL = no for Option B).
# 4. APPEND wired-qmk/config.h to the keymap's existing config.h
#    (keep that file's Vial UID + unlock-combo lines).
cat /path/to/this/repo/wired-qmk/config.h >> keyboards/crkbd/keymaps/roba-wired/config.h

# 5. Compile (Pro Micro = crkbd/rev1)
qmk compile -kb crkbd/rev1 -km roba-wired
```

If `crkbd/rev1` errors, check the exact target on Keebmaker's default keymap
page; some kits use a specific revision string.

## Setting the colours

Edit the defines at the top of `keymap.c`. Format is `hue, saturation, value`,
each **0–255** in QMK:

```c
#define HSV_BASE   0,   0,  90   // set to YOUR colour (e.g. cyan = 128,255,255)
#define HSV_LOWER  170, 255, 150 // blue
#define HSV_RAISE  85,  255, 150 // green
#define HSV_ADJUST 0,   255, 150 // red
```

Hue reference: red 0 · orange 21 · yellow 43 · green 85 · cyan 128 · azure 149 ·
blue 170 · violet 191 · magenta 213. Saturation 255 = vivid, 0 = white.

## Flash

Produces a `.hex` (one image; flash it to **both** halves).

```bash
qmk flash -kb crkbd/rev1 -km roba-wired
```

Or use **QMK Toolbox**: load the `.hex`, put one half into the bootloader
(short the `RST`+`GND` pads twice, or press reset), flash, then repeat for the
other half.

## After flashing

- (Option A) If the layout looks reset, open **Vial** and re-import `corne.vil`
  — the colour hook is in firmware and works regardless of which layout loads.
- Hold **lower + raise** together to reach the adjust layer (red).

## If it still won't fit

Read the compiler's `The firmware is X bytes too large` line. Next levers:
drop `EXTRAKEY_ENABLE` (media keys, if unused), or switch to Option B (drop
Vial). The OLED's own footprint can also shrink by avoiding large fonts/logos
in the OLED render code. Beyond that, the nice!nano/ZMK build on `main` is the
roomy home for displays.
