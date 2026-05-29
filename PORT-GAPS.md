# Port gaps — graphical keymap

Which keys/config did **not** carry over cleanly from the original Vial layout
(`corne.vil`). One diagram for the **wireless (ZMK)** build (`main`) and one for
the **wired (QMK)** build (`wired`). Layers are labelled by their 0-indexed
number. Full prose is in `CHANGES-from-vial.md`.

Each grid is the physical Corne: left hand `│` right hand, three rows + three
thumbs per side.

```
Legend
  (plain)  ported cleanly, behaves like the original
  ◐        PARTIAL  — works, but a nuance was lost
  ⟳        CHANGED  — behaviour intentionally remapped
  ✗        NOT PORTED — dropped, or a placeholder substituted
  ＋       ADDED    — new key, not in the original layout
  ⊘        DISABLED by a build trim (wired only)
  ⚑        flagged — ported as-is but looks questionable
  †        reimplemented a different way, same end result
```

Tap/hold mod-taps are written `tap/HOLD`; hold mods: `⌘`GUI `⌃`Ctrl `⌥`Alt `⇧`Shift.

---

# Wireless (ZMK) — branch `main`

## Layer 0 — Base
```
 Tab     Q       W       F       P       B    │   J       L       U       Y       ,       .
 )/⌃ ◐   A/⌘     R/⌃     S/⌥     T/⇧     G    │   M       N/⇧     E/⌥     I/⌃     O/⌘     2 ⚑
 (/⇧ ◐   Z       X       C       D       V    │   K       H       \       [;◐     ';◐     Esc/`†
                         L0 ⟳    Bsp/Del◐ Spc │   Ent     L1 ⟳    L2 ⟳
```
- `)/⌃` `(/⇧` ◐ — Space Cadet (`KC_RCPC`/`KC_LSPO`) → `&mt`. Hold = mod, tap =
  paren, but `&mt` needs a clean tap; real Space Cadet auto-taps on solo press.
- `[;` `';` ◐ — tap-dances `[`/`;` and `'`/`;` → `&hk` hold-taps. The
  **double-tap `;`** action is gone (tap=`[`/`'`, hold=`;` kept).
- `Bsp/Del` ◐ — tap-dance Bspc/Del → `&hk DEL BSPC`. Double-tap (Del) dropped.
- `Esc/`` † — grave-escape reimplemented as a mod-morph; same result.
- `2` ⚑ — sits on the right pinky home in your layout; kept verbatim, but flag
  it in case it was a slip.
- `L0/L1/L2` ⟳ — were `TO()` toggles; now macros that also set the layer colour.

## Layer 1 — Lower
```
 ~       M0 ◐    M1      8       Home    PgUp │   PgDn    End     9       \       ÷       ×
 ─       1/⌘     2/⌃     3/⌥     4/⇧     5    │   6       7/⇧     8/⌥     9/⌃     0/⌘     /
 ─       =       ¥ ✗     `       ←       ↑    │   ↓       →       #       -       .       +
                         ─       ─       ─    │   L0 ⟳    L3 ⟳    L2 ⟳
```
- `M0` ◐ — AltGr+7 macro ported, but its `key_override` (M0 → `8` under certain
  mods) has **no ZMK equivalent and is dropped**. M1 (AltGr+0) is clean.
- `¥` ✗ — `KC_JYEN` has no clean ZMK keycode; **placeholder** `&kp NON_US_BSLH`.
- `L0/L2/L3` ⟳ — layer/colour macros (navigation reworked, see below).

## Layer 2 — Raise
```
 ─       ─       ⇧Home   ⇧↑      ⇧End    PgUp │   PgUp    ⇧7      ⇧8      ⇧9      ─       ─
 ─       RAlt    ⇧←      ⇧↓      ⇧→      PgDn │   PgDn    ⇧4      ⇧5      ⇧6      ─       ─
 ─       ─       `       ⇧\      ⇧]      ⇧0   │   ⇧0      ⇧1      ⇧2      ⇧3      ]       ─
                         ─       ─       ─    │   L0 ⟳    L3 ⟳    ─
```
- No per-key gaps — every `LSFT_T(...)` ported cleanly to `&mt LSHFT x`. Only the
  layer thumbs (`L0/L3` ⟳) changed.

## Layer 3 — Adjust
```
 Boot    RGBgrn⟳ RGBred⟳ RGBblu⟳ ─       ─    │  BTclr＋  BT0 ＋   BT1 ＋   BT2 ＋   BT3 ＋   USB⟷＋
 RGBtog  Hue+    Sat+    Bri+    ─       Spd+ │   ─       ─       ─       ─       ─       ─ ✗
 RGBeff⟳ Hue-    Sat-    Bri-    ─       Spd- │   ─       ─       ─       ─       ─       ─ ✗
                         ─       ─       ─    │   L0 ⟳    ─       ─
```
- `RGBgrn/red/blu` ⟳ — were `RGB_M_*` animation-mode keys; remapped to *set a
  solid colour* (more useful here). `RGBeff` ⟳ — `RGB_MOD` → cycle effect.
- `BTclr/BT0-3/USB⟷` ＋ — **added** for wireless (Bluetooth profiles + USB/BLE
  toggle); the wired original had none.
- Right-hand `─ ✗` — the two `MAGIC_EE_HANDS_*` keys were dropped; ZMK sets
  handedness from which half is flashed, so they're unnecessary.

## Config-level (not a single key)
- `key_override` M0→`8` — ✗ dropped (no ZMK primitive).
- `MAGIC_EE_HANDS_*` — ✗ dropped (handled by the shield halves).
- `OSL()` one-shot layers (in tap-dance TD2) — ⟳ replaced by the toggle macros.
- Combo `(`+`Bspc` → `Delete` — ported as a positional combo (key-positions
  24 + 37), base layer only. Clean.

---

# Wired (QMK) — branch `wired`

The wired build runs **your original `corne.vil` layout** (re-imported in Vial)
on the same QMK family it came from — so **every key ports at full fidelity**.
Space Cadet, all tap-dance actions (incl. double-tap), `OSL`, the `key_override`,
`KC_JYEN`, media keys as written — all behave exactly as before. There are **no
per-key port gaps**.

The only losses come from the flash trims needed to fit the OLED on the
atmega32u4 (see `wired-qmk/`):

## Layer 0 — Base (only the trim-disabled items marked)
```
 Tab     Q       W       F       P       B    │   J       L       U       Y       ,       .
 )/⌃     A/⌘     R/⌃     S/⌥     T/⇧     G    │   M       N/⇧     E/⌥     I/⌃     O/⌘     2
 (/⇧ ⊘   Z       X       C       D       V    │   K       H       \       [;      ';      Esc/`
                         TO0     Bsp/Del⊘ Spc │   Ent     OSL     TO1
```
- `(`+`Bsp/Del` ⊘ — the **`(`+`Bspc` → `Delete` combo is disabled**
  (`COMBO_ENABLE = no`, dropped to save flash). The keys themselves still work;
  only the combo gesture is off. (Re-enabling fits only on the plain-QMK
  Option B — Option A has just ~1.1 KB free.)

## Config-level (wired)
- `COMBO_ENABLE = no` — ⊘ the Delete combo above.
- `EXTRAKEY_ENABLE = no` — ⊘ media/consumer keys won't work (your layout doesn't
  appear to use any).
- Per-key RGB matrix off — underglow only (your original used underglow anyway).
- The fallback `keymap.c` in `wired-qmk/` is a generic default, **not** your
  layout — it only appears if the Vial EEPROM is wiped; re-import `corne.vil`.

Layers 1–3 on wired are your original Lower/Raise/Adjust, unchanged — nothing to
flag, so they're not redrawn here.

> Want the wired build to match wireless exactly (combo + media on)? Use the
> plain-QMK **Option B** firmware (9.3 KB free) — see `wired-qmk/README.md`.
