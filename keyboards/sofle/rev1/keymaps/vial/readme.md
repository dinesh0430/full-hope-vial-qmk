# Vial keymap for Sofle (based on VIA)

Keyboard Layout: http://www.keyboard-layout-editor.com/#/gists/5e0381b049c62dce942eafc3e2d26f52 (used to generate vial.json and place encoders)

Default keymap in [Keyboard Layout Editor](http://www.keyboard-layout-editor.com/#/gists/76efb423a46cbbea75465cb468eef7ff) and [adjust layer](http://www.keyboard-layout-editor.com/#/gists/4bcf66f922cfd54da20ba04905d56bd4)

Features:

-   Symmetric modifiers (CMD/Super, Alt/Opt, Ctrl, Shift)
-   The OLED on master half shows selected mode and caps lock state and is rotated.
-   Vial support
    -   Left & Right encoders can be configured!
    -   Security Unlock/Lock Support: hold four corner keys
    -   no RGB support due to firmware size

# Important changes made in Antigravity

## Changes to vial.c for tapping term
In `quantum/vial.c`, the default tapping term logic was refactored to resolve a `multiple definition of 'get_tapping_term'` linking error that occurs when `TAPPING_TERM_PER_KEY` is enabled and a custom `get_tapping_term` function is defined in the keymap.

- **`vial_get_tapping_term`**: A new function created to encapsulate Vial's dynamic tapping term logic (which handles Vial GUI settings and Tap Dance timings).
- **`get_tapping_term`**: The existing function in `vial.c` was marked as `__attribute__((weak))` and updated to simply call `vial_get_tapping_term`.

**Why it was done**:
Marking the function as `weak` allows keymaps to define their own `get_tapping_term` without throwing linker errors. By exposing `vial_get_tapping_term`, custom keymaps can safely fall back to Vial's default logic for unhandled keys, preserving all dynamic GUI features while allowing per-key C-code overrides.