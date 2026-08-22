# 34-Key Timeless Bottom Row Mods Layout for Sofle Rev1

A minimalist, ergonomic **34-key layout** (3x5 alpha grid + 2 primary thumbs per hand) built for the **Sofle Rev1** with **Adafruit KB2040 (RP2040)** controllers, featuring **urob's timeless modifier architecture** ported natively to QMK with full **Vial** support.

---

## 1. Physical Layout & Layer Overview

The layout isolates 34 active keys across the Sofle matrix. All unused outer columns and number rows (except the top-left bootloader key) are transparent (`_______`) and can be remapped on-the-fly in Vial.

```
       LEFT HAND (3x5 + 2)                                 RIGHT HAND (3x5 + 2)
.-----------------------------------.               .-----------------------------------.
|BOOT |     |     |     |     |     |               |     |     |     |     |     |BOOT |  <-- Row 0 (Top Corner Boot)
|-----+-----+-----+-----+-----+-----|               |-----+-----+-----+-----+-----+-----|
|     |  Q  |  W  |  E  |  R  |  T  |               |  Y  |  U  |  I  |  O  |  P  |     |  <-- Top Row
|     |  A  |  S  |  D  |  F  |  G  |               |  H  |  J  |  K  |  L  |  ;  |     |  <-- Home Row (Plain Alphas)
|     |  Z  |  X  |  C  |  V  |  B  |               |  N  |  M  |  ,  |  .  |  /  |     |  <-- Bottom Row Mods (SACG)
'-----------------------------------'               '-----------------------------------'
                |NAV/BSP|NUM/ENT|                       |SYM/ENT|FUN/SPC|                  <-- Dual-Role Thumbs
                '---------------'                       '---------------'
```

---

## 2. Layer Maps

### Layer 0: Base (QWERTY + Bottom Row Mods)
* **Home Row:** Completely standard, unmodified alpha keys (zero hold latency when typing words).
* **Bottom Row Mods (SACG Order - Pinky $\rightarrow$ Index):**
  * Left: `Z` (Shift), `X` (Alt), `C` (Ctrl), `V` (GUI / Win / Cmd)
  * Right: `M` (GUI / Win / Cmd), `,` (Ctrl), `.` (Alt), `/` (Shift)
* **Thumb Cluster:**
  * Left 1: `NAV_BSP` $\rightarrow$ Tap: `Backspace` | Hold: `Layer 1 (NAV)`
  * Left 2: `NUM_ENT` $\rightarrow$ Tap: `Enter`     | Hold: `Layer 2 (NUM)`
  * Right 1: `SYM_ENT` $\rightarrow$ Tap: `Enter`     | Hold: `Layer 3 (SYM)`
  * Right 2: `FUN_SPC` $\rightarrow$ Tap: `Space`     | Hold: `Layer 4 (FUN)`

```
Layer 0 Matrix:
.-----------------------------------.               .-----------------------------------.
|   Q   |   W   |   E   |   R   |   T   |               |   Y   |   U   |   I   |   O   |   P   |
|   A   |   S   |   D   |   F   |   G   |               |   H   |   J   |   K   |   L   |   ;   |
| Z/SFT | X/ALT | C/CTL | V/GUI |   B   |               |   N   | M/GUI | ,/CTL | ./ALT | //SFT |
'-----------------------------------'               '-----------------------------------'
                |NAV/BSP|NUM/ENT|                       |SYM/ENT|FUN/SPC|
                '---------------'                       '---------------'
```

---

### Layer 1: Navigation & Editing
* **Top Row:** Numbers `1` through `0` across both hands.
* **Left Home Row (Tap Dance + Arrows):**
  * `TP_CTCS` (Pinky): Tap Dance $\rightarrow$ **Tap = Escape**, **Hold = Ctrl**, **Double-Tap Hold = Ctrl+Shift**.
  * `KC_LEFT`, `KC_UP`, `KC_DOWN`, `KC_RGHT`: Full directional navigation under your left hand.
  * *Hold pinky + arrows = jump words (`Ctrl+Arrows`). Double-tap hold pinky + arrows = highlight words/lines (`Ctrl+Shift+Arrows`).*
* **Bottom Pinky Modifiers (Dual-Role Shift):**
  * `SFT_UND` (Left Pinky): Tap $\rightarrow$ `Ctrl+Z` (Undo) | Hold $\rightarrow$ **Shift** (`KC_LSFT`).
  * `SFT_ESC` (Right Pinky): Tap $\rightarrow$ `Escape` | Hold $\rightarrow$ **Shift** (`KC_RSFT`).
* **Editing Shortcuts:**
  * `CT_CUT` (`Ctrl+X`), `CT_COPY` (`Ctrl+C`), `CT_PSTE` (`Ctrl+V`), `DEL_WRD` (`Ctrl+Backspace`).

```
Layer 1 Matrix:
.-----------------------------------.               .-----------------------------------.
|   1   |   2   |   3   |   4   |   5   |               |   6   |   7   |   8   |   9   |   0   |
|TP_CTCS|  LEFT |   UP  |  DOWN | RIGHT |               |  LEFT |  DOWN |   UP  | RIGHT |CW_TOGG|
|SFT_UND| CT_X  | CT_C  | CT_V  |DEL_WRD|               |  BSPC |  TAB  |  ENT  |  DEL  |SFT_ESC|
'-----------------------------------'               '-----------------------------------'
                |_______|_______|                       |  ENT  |  DEL  |
                '---------------'                       '---------------'
```

---

### Layer 2: Numbers & Clean Numpad
* **Left Hand:** Quick programming symbols (`!`, `@`, `#`, `$`, `%`, `^`, `&`, `*`, `=`, `+`, `~`, `` ` ``, `-`, `_`, `|`).
* **Right Hand:** Clean 3x3 numeric keypad with `0` placed directly on the home-row pinky position (`:` / `;`).

```
Layer 2 Matrix:
.-----------------------------------.               .-----------------------------------.
|   !   |   @   |   #   |   $   |   %   |               |   /   |   7   |   8   |   9   |   +   |
|   ^   |   &   |   *   |   =   |   +   |               |   *   |   4   |   5   |   6   |   0   |  <-- 0 on Semicolon
|   ~   |   `   |   -   |   _   |   |   |               |   -   |   1   |   2   |   3   |   .   |
'-----------------------------------'               '-----------------------------------'
                |_______|  BSPC |                       |   =   | Enter |
                '---------------'                       '---------------'
```

---

### Layer 3: Symbols & Brackets
* **Programmer-Optimized:** Paired brackets (`{}`, `[]`, `()`, `<>`), quotes, slashes, and mathematical operators.

```
Layer 3 Matrix:
.-----------------------------------.               .-----------------------------------.
|   {   |   }   |   [   |   ]   |   $   |               |   `   |   ~   |   &   |   ^   |   !   |
|   (   |   )   |   <   |   >   |   @   |               |   :   |   ;   |   '   |   "   |   ?   |
|   |   |   \   |   /   |   %   |   #   |               |   _   |   -   |   +   |   =   |   *   |
'-----------------------------------'               '-----------------------------------'
                |  BSPC | Space |                       |_______|_______|
                '---------------'                       '---------------'
```

---

### Layer 4: Function Keys, Media & Mouse Control
* **F1–F12 Keys:** Full function key row on left hand.
* **Media & Volume:** Previous, Next, Play/Pause, Vol+, Vol-, Mute.
* **Mouse Control:** Cursor movement (`MS_LEFT/DOWN/UP/RGHT`), mouse buttons (`BTN1`, `BTN2`, `BTN3`), and scroll wheel (`WH_LEFT/DOWN/UP/RGHT`).

---

## 3. All Settings Used for the Timeless Modifier Architecture

This configuration ports **urob's timeless home row mods** (originally designed for ZMK) into native QMK:

```
                            [ Key Pressed ]
                                   │
              Was another key pressed within 150ms? (Flow Tap)
                     ┌─────────────┴─────────────┐
                    YES                          NO
                     │                           │
          [ Settle as Plain TAP ]       Is next key on Opposite Hand?
          (Typing at speed:             (Chordal Hold Matrix)
           Zero Modifier Misfires)        ┌──────┴──────┐
                                         YES            NO
                                          │             │
                                    [ Settle as    [ Settle as
                                       HOLD ]         TAP ]
```

### Complete Parameter Reference:

| Parameter | Value | Purpose & Architectural Role |
| :--- | :---: | :--- |
| **`TAPPING_TERM`** | `280ms` | Generous hold window. Makes the keys timer-insensitive ("timeless") so resting a finger on a key never triggers a modifier prematurely. |
| **`PERMISSIVE_HOLD`** | `Enabled` | Resolves mod-tap as a hold if another key is tapped and released during the hold (`A ↓, B ↓, B ↑, A ↑`), regardless of how quickly it is typed. |
| **`FLOW_TAP_TERM`** | `150ms` | If a mod-tap key is pressed within 150ms of a preceding alpha, it settles immediately as a tap. Completely eliminates input lag and rolling misfires during fast typing bursts. |
| **`CHORDAL_HOLD`** | `Enabled` | Uses `chordal_hold_layout` matrix to enforce the **Opposite Hands Rule** (Left modifiers only trigger on Right keys). Same-hand rolls immediately settle as taps. |
| **`HOLD_ON_OTHER_KEY_PRESS_PER_KEY`** | `Per-Key` | **Shift (`BM_Z`, `BM_SLS`):** `true` for instant 0ms cross-hand capitalization (`/s` $\rightarrow$ `S`).<br>**Ctrl/Alt/GUI (`BM_C`, `BM_X`, etc.):** `false` to prevent fast typing rolls (e.g. `ch` $\rightarrow$ `Ctrl+H`) from falsely triggering hotkeys. |
| **`QUICK_TAP_TERM`** | `0ms` | Disables double-tap auto-repeat so fast repeated taps never trigger unexpected holds. |
| **`VIAL_INSECURE`** | `yes` | Permanently unlocks Vial GUI at [vial.rocks](https://vial.rocks) without requiring an unlock combo. |

---

## 4. Deep Dive: Vial Quirks, Traps, and Architectural Workarounds

When building advanced QMK features on top of a Vial fork (`vial-qmk`), Vial introduces several hidden runtime behaviors that can break standard QMK logic. Below is a detailed breakdown of the traps encountered and how we resolved them:

### Trap 1: The EEPROM Dynamic Settings Override (`QMK_SETTINGS` & `QS`)
* **The Problem:** In standard QMK, `#define` macros in `config.h` control tapping behavior directly. However, in Vial (`QMK_SETTINGS = yes`), `quantum/qmk_settings.c` reads tapping settings from EEPROM at boot and overrides `#define` macros at runtime. If the keyboard's EEPROM was initialized before our firmware changes, `QS.tapping_v2` contains `0` (disabled) for `PERMISSIVE_HOLD` or `CHORDAL_HOLD`, causing QMK to completely ignore `config.h`!
* **How We Found It:** When testing fast cross-hand capitalization (`/` + `s`), the keyboard produced `/s` unless we waited for the tapping term. Adding `#define` macros in `config.h` had zero effect because the EEPROM struct was overriding them.
* **The Solution:** In `keymap.c` $\rightarrow$ `keyboard_post_init_user()`, we forcefully bitwise-OR the runtime `QS.tapping_v2` flags and set the timing parameters on every single boot:
  ```c
  #ifdef QMK_SETTINGS
      QS.tapping_v2 |= (1 << QS_tapping_permissive_hold_bit) |
                       (1 << QS_tapping_chordal_hold_bit);
      QS.tapping_term = 280;
      QS.flow_tap_term = 150;
      QS.quick_tap_term = 0;
  #endif
  ```

---

### Trap 2: Non-Weak Wrapper Functions in `qmk_settings.c`
* **The Problem:** QMK defines customization hooks like `get_hold_on_other_key_press()` and `get_chordal_hold()` as `__attribute__((weak))` so keymaps can define per-key logic. However, in `vial-qmk`, `quantum/qmk_settings.c` implemented these functions *without* the `weak` attribute. Attempting to define `get_hold_on_other_key_press()` in `keymap.c` threw a linker error: `multiple definition of 'get_hold_on_other_key_press'`.
* **The Solution:** We patched `quantum/qmk_settings.c` to mark `get_hold_on_other_key_press` and `get_chordal_hold` as `__attribute__((weak))`. This allows `keymap.c` to implement per-key rules:
  ```c
  bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
      switch (keycode) {
          case BM_Z:
          case BM_SLS:
              return true;  // Instant 0ms hold for Shift capitalization
          default:
              return false; // Prevent Ctrl/Alt/GUI rolls from misfiring
      }
  }
  ```

---

### Trap 3: The `Ctrl+H` Bigram Rolling Misfire (`BM_C + KC_H`)
* **The Problem:** In English, "ch" is a frequent bigram (*check, character, which, change*). When typing "ch" quickly, the left index finger presses `C` (`BM_C` = `LCTL_T(KC_C)`) and the right index finger rolls into `H` (`KC_H`) before `C` is released. Because `C` is on the Left hand and `H` is on the Right hand, `CHORDAL_HOLD` classified it as an opposite-hand chord. If `HOLD_ON_OTHER_KEY_PRESS` was enabled globally, the 0ms key-down of `H` immediately converted `C` to `Ctrl`, firing an accidental `Ctrl + H` hotkey!
* **The Solution:**
  1. Disabled `HOLD_ON_OTHER_KEY_PRESS` for `BM_C` (and all non-Shift mods), so `C` requires `PERMISSIVE_HOLD` (nested tap) or `TAPPING_TERM` to become `Ctrl`.
  2. Added an explicit exception in `get_chordal_hold()` to guarantee that `BM_C + KC_H` always settles as plain letters `c` and `h`:
  ```c
  bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record,
                        uint16_t other_keycode, keyrecord_t *other_record) {
      switch (tap_hold_keycode) {
          case BM_C:
              if (other_keycode == KC_H) return false; // Always plain 'ch'
              break;
      }
      return get_chordal_hold_default(tap_hold_record, other_record);
  }
  ```

---

### Trap 4: Vial Tap Dance Array Ownership & Dangling Pointers
* **The Problem:** Vial allocates `tap_dance_actions[]` dynamically in RAM based on `#define VIAL_TAP_DANCE_ENTRIES 12`. Statically defining `tap_dance_actions[]` in `keymap.c` causes a `multiple definition` linker error. Furthermore, using `ACTION_TAP_DANCE_DOUBLE` inside functions creates stack-allocated structs that get destroyed upon function exit, leaving dangling pointers that trigger erratic ghost mouse movements when tapped.
* **The Solution:** Declare `extern tap_dance_action_t tap_dance_actions[];` and inject `ACTION_TAP_DANCE_FN_ADVANCED` actions using static function pointers and global `td_state_t` structs inside `keyboard_post_init_user()`:
  * Index 0: `TP_CTCS` (Tap = Esc, Hold = Ctrl, Double-Tap Hold = Ctrl+Shift)
  * Index 1: `SFT_UND` (Tap = Ctrl+Z, Hold = Shift)

---

## 5. Bootloader & Recovery Options

1. **Software Bootloader Key:**
   * Key `[0, 0]` (the physical top-left corner key, standard Escape position) is mapped to `QK_BOOT` on **all layers (0–4)**. Tapping it instantly puts the keyboard into bootloader mode.
2. **Hardware Bootmagic Lite:**
   * Plug in USB while holding the **Top-Left key (`[0, 0]`)** on Left half or **Top-Right key (`[5, 0]`)** on Right half $\rightarrow$ enters bootloader.
3. **RP2040 Double-Tap Reset:**
   * Double-tap the physical reset button on the KB2040 MCU within 1000ms $\rightarrow$ enters bootloader.

---

## 6. Build Instructions

### Strict Build Rule (Left Side Only):
As specified in `AGENTS.md`, only compile the **Left Side** firmware:

```bash
qmk compile -kb sofle/rev1 -km 34_key -e CONVERT_TO=kb2040 -e EXTRAFLAGS="-DIS_LEFT_SIDE" -e TARGET=sofle_rev1_34_key_left
```

This generates `sofle_rev1_34_key_left_kb2040.uf2` in the root folder, ready to drag-and-drop onto the `RPI-RP2` drive.

