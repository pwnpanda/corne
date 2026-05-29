# Vial (QMK) → ZMK port notes

Source: `corne.vil` (Vial protocol 6 / VIA protocol 9). Every translation
decision is listed here, especially the lossy ones you should review.

## Structural

- **Right-half column order is reversed.** Vial stores each right-hand row
  inner-column-first; ZMK keymaps are laid out left-to-right across the whole
  board. Every right-hand row (and the right thumb cluster) was reversed
  during the port. If you diff against the `.vil` by eye, expect mirrored rows.
- **Handedness.** QMK `MAGIC_EE_HANDS_*` keys are dropped. ZMK derives left/
  right from which shield half is flashed (`corne_left` / `corne_right`), so
  no in-keymap handedness toggle is needed.

## Clean 1:1 translations

| QMK | ZMK |
|-----|-----|
| `LCTL_T(X)`, `LALT_T`, `LSFT_T`, `RGUI_T`, … | `&mt MOD X` |
| `TD(3)` = tap `A` / hold `LGUI` | `&mt LGUI A` (it was a mod-tap, not a real tap dance) |
| `LSFT_T(KC_HOME)` etc. on raise | `&mt LSHFT HOME` etc. |
| `KC_GESC` (grave-escape) | `grave_escape` mod-morph (ESC, or `` ` `` under Shift/GUI) |
| `KC_NONUS_BSLASH` / `KC_NONUS_HASH` | `&kp NON_US_BSLH` / `&kp NON_US_HASH` |
| RGB / boot keys on adjust layer | `&rgb_ug …`, `&bootloader` |

## Tap dances → hold-taps

QMK multi-action tap dances (tap / hold / double-tap / tap-then-hold) have **no
ZMK equivalent**. ZMK tap-dance is tap-*count* based and can't mix a hold
action. The ones used here were effectively "tap X, hold Y", which map onto a
hold-tap behaviour (`&hk`, "hold-key"):

| Vial | tap / hold / double / tap-hold | ZMK |
|------|-------------------------------|-----|
| `TD(0)` | Bspc / Del / Bspc / Del | `&hk DEL BSPC` (double-tap → Del dropped; double-Bspc unaffected) |
| `TD(1)` | `'` / `;` / `;` / — | `&hk SEMI SQT` (double-tap `;` dropped) |
| `TD(4)` | `[` / `;` / `;` / `;` | `&hk SEMI LBKT` (double-tap `;` dropped) |
| `TD(2)` | `OSL(1)` / `OSL(3)` / `OSL(2)` / `OSL(3)` | replaced by toggle macros — see below |

If you genuinely use the double-tap actions, tell me and I'll add ZMK
`tap-dance` behaviours for the tap-count cases.

## Layer navigation + per-layer colour

The original switched layers with `TO(0..3)` toggles and a one-shot-layer
(`OSL`) tap dance. The port uses **toggle macros `&l0`–`&l3`** that switch the
layer *and* set the underglow colour in one action:

- `&l0` base → dim white   `&l1` lower → blue
- `&l2` raise → green       `&l3` adjust → red

Because they are toggles, exactly one layer is active at a time and the colour
is set reliably on every switch — no fragile press/release pairing.

Changes from the original navigation:
- From **base** you can now reach lower (`&l1`) *and* raise (`&l2`) directly
  (the original only reached lower from base, then raise from lower).
- `OSL` one-shot-layer behaviour is gone (toggles instead). The `&l0` key is
  your "home" — it drops everything back to base.
- Adjust (`&l3`) is reachable from lower and raise.

This is the area most likely to want tweaking once you settle the layout.

## Dropped / substituted — review these

- **`key_override`** (`M0` → `8` under specific mods): ZMK has no key-override
  primitive. Dropped. `M0` now always types AltGr+7.
- **`KC_JYEN`** (Japanese ¥, on lower layer): no clean ZMK keycode.
  Substituted `&kp NON_US_BSLH` as a placeholder. Change it if you need ¥.
- **RGB effect-mode keys** `RGB_M_G/R/B`: in QMK these select animation modes;
  remapped to *set solid green/red/blue* via `&rgb_ug RGB_COLOR_HSB(...)`,
  which is more useful here. `RGB_MOD` → `RGB_EFF` (cycle effect).
- **Space Cadet** `KC_LSPO` / `KC_RCPC`: ZMK has no Space Cadet. Approximated
  with `&mt LSHFT LPAR` / `&mt RCTRL RPAR` (hold = mod, tap = the paren). The
  real Space Cadet auto-taps the paren on solo press; `&mt` requires a clean
  tap. Close, not identical.
- **`KC_2` on the right pinky home (base layer)**: kept as-is (`&kp N2`) — it
  looks intentional in your layout, but flagging it in case it was a slip.

## Added for wireless

The original is a wired layout with **no Bluetooth keys** — you could not pair
or switch hosts. Added to the adjust layer's right hand:

- `&bt BT_SEL 0..3` — select Bluetooth profile / host
- `&bt BT_CLR` — clear the current profile's bond (re-pair)
- `&out OUT_TOG` — toggle USB / BLE output

## Combo

`LSPO + Backspace → Delete` ported as a ZMK positional combo on key-positions
`24` (left-shift, bottom outer) + `37` (backspace thumb), base layer only.
