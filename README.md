# Corne Choc v2.5 — wireless ZMK config

## What this does

ZMK firmware configuration for a Corne Choc v2.5 (Keebmaker, ordered Feb 2025)
running wirelessly on **nice!nano v2** controllers. It is a port of an existing
Vial/QMK layout (`corne.vil`) with one headline feature added: **the RGB
underglow changes colour when you switch layers** (base = white, lower = blue,
raise = green, adjust = red). GitHub Actions builds flashable `.uf2` firmware
on every push.

## Use cases

- Run the Corne wirelessly over Bluetooth instead of USB/QMK.
- Get an at-a-glance colour cue for the active layer.
- Edit the keymap as code and have CI build firmware automatically.

## Requirements

- **Hardware:** Corne Choc v2.5, two **nice!nano v2** controllers (replacing the
  wired controllers — sockets strongly recommended), two LiPo batteries, two
  power switches. The nice!nano-compatible controller is sold by Keebmaker.
- **Underglow LEDs** wired on the board (the per-layer colour drives these). If
  your board is per-key RGB, ZMK lights the whole chain one colour — see
  *Implemented features* for the limitation.
- A GitHub account (CI builds the firmware; no local toolchain needed).
- For flashing: nothing but a USB cable and a file manager — the nice!nano
  mounts as a USB drive in bootloader mode.

## Installation / Setup

1. **Push this repo to GitHub.** The included workflow
   (`.github/workflows/build.yml`) calls ZMK's reusable build workflow.
2. **Get the firmware.** Open the repo's *Actions* tab → latest run →
   *Artifacts*. Download `firmware.zip`. It contains `corne_left ...uf2`,
   `corne_right ...uf2`, and `settings_reset ...uf2`.
3. **Flash each half** (one at a time, left first is the usual convention):
   - Double-tap the nice!nano reset to enter bootloader; it mounts as a USB
     drive (`NICENANO`).
   - Drag the matching `.uf2` onto it. It reboots automatically.
4. **Pair:** on the host, pair with the keyboard. Use the adjust layer
   (`&l3`, red) `BT_SEL 0..3` keys to pick a profile and `BT_CLR` to re-pair.

If pairing ever gets stuck, flash `settings_reset ...uf2` to both halves to wipe
stored bonds, then re-flash the normal firmware.

## Usage

Layers (each entered via a colour-setting toggle key on the thumbs):

| Layer | Colour | Reach it with | Contents |
|-------|--------|---------------|----------|
| 0 base | dim white | `&l0` (left outer thumb) | Colemak-DH + home-row mods |
| 1 lower | blue | `&l1` (right thumb) | numbers, nav, AltGr macros |
| 2 raise | green | `&l2` (right thumb) | shifted nav, symbols |
| 3 adjust | red | `&l3` (from lower/raise) | RGB, Bluetooth, bootloader |

`&l0` is the "home" key — it drops back to base from anywhere. Manual RGB
controls (toggle, hue, saturation, brightness, effect, speed) live on the
adjust layer's left hand.

To change the keymap, edit `config/corne.keymap`, push, and grab the new
firmware from Actions. The [ZMK Keymap Editor](https://nickcoutsos.github.io/keymap-editor/)
can edit it visually against this repo.

## Displays

The Corne's two 128x32 OLEDs are enabled (`CONFIG_ZMK_DISPLAY=y` in
`corne.conf`). Out of the box you get ZMK's built-in status screen: active
layer, battery %, Bluetooth profile, output (USB/BLE), and WPM. They went dark
after your earlier Vial flash only because that generic build shipped no OLED
rendering code — nothing is wrong with the panels.

Want something fancier (bongo cat, vertical widgets, custom art)? Those come
from third-party ZMK modules, added via `config/west.yml`. Pick from:

- **[zmk-nice-oled](https://github.com/mctechnology17/zmk-nice-oled)** — richest
  option: bongo/Luna cat, modifier indicators, battery art, vertical widgets.
  Works on OLED and nice!view. Note: LVGL-heavy; stacking large widgets can
  exhaust nRF52840 RAM, and animations on the peripheral half hurt battery.
- **[zmk-dongle-display](https://github.com/englmaxi/zmk-dongle-display)** —
  custom status screen; on a 128x32 OLED you must drop some widgets (e.g.
  `CONFIG_ZMK_DONGLE_DISPLAY_BONGO_CAT=n`).
- **[mctechnology17/zmk-config](https://github.com/mctechnology17/zmk-config)**
  — a ready-made base bundling several display modules; good to copy patterns.
- **Custom art:** [YousefHadder/corne-zmk](https://github.com/YousefHadder/corne-zmk)
  rotates 1-bit images; convert with [image2cpp](https://github.com/javl/image2cpp)
  or [qle](https://github.com/joric/qle).
- **More:** [awesome-zmk](https://github.com/mctechnology17/awesome-zmk).

Two honest caveats: ZMK still labels displays **not production-ready** (a known
bug can leave the screen blank after external-power cutoff resume), and every
widget you add costs battery and RAM. The built-in screen is the safe default;
say the word and I'll wire up `zmk-nice-oled` on a branch.

## Testing

No automated tests — this is firmware config, validated by ZMK's build (a green
Actions run means it compiles) and by flashing to the board. The lossy QMK→ZMK
conversions are documented in `CHANGES-from-vial.md` for manual review.

## Deployment

Deployment = flashing the CI-built `.uf2` to each half (see *Installation*).
There is no server component.

## Implemented features

- Wireless (BLE) split on nice!nano v2, with USB fallback (`&out OUT_TOG`).
- Per-layer underglow colour via toggle macros `&l0`–`&l3`.
- Four-layer Colemak-DH layout ported from the Vial config, with home-row mods,
  grave-escape, approximated Space Cadet parens, and hold-tap keys replacing
  the old tap dances.
- Bluetooth profile management and bootloader access on the adjust layer.
- OLED status screens (layer / battery / BT / output / WPM), blanked on idle.
- Battery saving: RGB auto-off when idle, deep sleep after 30 min.

**Known limitation — RGB scope.** ZMK drives the entire LED chain as one
"underglow" colour; it has no QMK-style per-key RGB *matrix*. "Whole board
changes colour per layer" works; per-key positional effects (reactive typing,
lighting only certain keys) are not available on ZMK.

## Planned features

- Resolve the items flagged in `CHANGES-from-vial.md` (Yen key substitution,
  whether the double-tap tap-dance actions are needed, the `KC_2` pinky).
- Layout refinement (the layout is expected to change — this port is the
  baseline).
- Optional: a richer OLED display module (bongo cat / widgets) — see *Displays*.
- A wired QMK build for the original AVR hardware lives on the `wired` branch.

## Claude Sessions

| Session | Summary | Date |
|---------|---------|------|
| `keyboard-config` | Ported the Vial/QMK Corne layout to a wireless ZMK config with per-layer underglow colour; scaffolded repo, keymap, CI build, and port notes | 2026-05-29 |
