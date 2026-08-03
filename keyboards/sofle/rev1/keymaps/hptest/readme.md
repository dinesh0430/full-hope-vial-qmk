# Antigravity Sofle Keymap: Comprehensive Guide & Context
**ATTENTION FUTURE AI AGENTS:** If you are reading this, you are stepping into a heavily customized QMK + Vial keymap environment. Please read this document entirely before making any changes to `keymap.c`, `config.h`, or core QMK files. It contains critical context about hardware constraints, previous memory bugs, and injection techniques.

## 1. Hardware Context: Broken TRRS, Dual Master Setup & KB2040 MCU
The user has a physically broken TRRS cable. As a result, the two halves of the Sofle keyboard are *not* connected to each other. Instead, both the left and right halves are connected directly to the computer via separate USB cables. Because of this, both halves act as completely independent "Master" keyboards. 

**MCU & Memory Notes:** 
Both sides of the keyboard are powered by the **Adafruit KB2040 (RP2040)** MCU. Because the RP2040 has an enormous amount of flash memory, **there is absolutely no need to perform any memory or firmware size optimizations.** You can freely enable features, expand macros, and maximize array sizes (like `VIAL_TAP_DANCE_ENTRIES 12`) without ever worrying about firmware size limits.

**Critical constraints to keep in mind:**
- **No Cross-Hand Communication:** The left half has absolutely no knowledge of what the right half is doing, and vice versa. 
- **Layer States Do Not Sync:** If the user activates a layer on the left half (e.g., holding `SP_L1`), the right half remains on Layer 0. 
- **No Cross-Hand Modifiers:** A combo or tap-dance modifier cannot bridge across the two halves.
- **OLED Screens:** Since both are connected via USB, both halves consider themselves the master. Thus, state changes (like `leader_sequence_active`) will only render on the OLED of the half where the physical key was pressed.

## 2. Compilation Flags & Negative Testing
Because both halves act as independent masters, `config.h` has a fail-safe check to ensure you specify exactly which side you are building. If you run a standard compilation without passing the side flag, it will intentionally fail. 
Future AI agents must test their code changes using the following command format:

```bash
# Fast-path make command for AI testing (swap RIGHT with LEFT as needed):
make -r -R -f builddefs/build_keyboard.mk -s KEYBOARD=sofle/rev1 KEYMAP=hptest KEYBOARD_FILESAFE=sofle_rev1 VERBOSE=false COLOR=false SILENT=false QMK_BIN="qmk" CONVERT_TO=kb2040 EXTRAFLAGS=-DIS_RIGHT_SIDE TARGET=sofle_rev1_hptest_right
```
This command serves as an excellent way to verify syntax and linker errors locally without getting bogged down by the interactive QMK wrapper.

## 3. Overview of `keymap.c` Features
- **Custom Space/Layer Key (`SP_L1`)**: Handled entirely in `process_record_user`. It tracks timestamps to determine whether a tap should send `KC_SPC` or a hold should activate `Layer 1`.
- **VSCode Macros (`VS_COMM`, `VS_UNCO`)**: Custom keycodes that send `Ctrl+K, Ctrl+C` and `Ctrl+K, Ctrl+U` when pressed.
- **Leader Sequence OLED Indicator**: In `oled.c`, we read `leader_sequence_active()`. When true, the left OLED renders the text "LDR" with inverted colors to visually alert the user.

## 4. Deep Dive: The Tap Dance Journey (Errors & Solutions)
We needed to hardcode a complex Tap Dance for Left Alt (Tap = Alt, Hold = Alt, Double Tap = Leader). Because we are using Vial (`VIAL_ENABLE = yes`), Vial forcefully takes ownership of the QMK `tap_dance_actions` array. This led to several cascading issues:

1. **The Linker Error (Multiple Definitions):** Attempting to define `tap_dance_actions[]` in `keymap.c` directly threw a `multiple definition` error because `vial.c` unconditionally defines it.
2. **The Array Bounds Error:** Trying to disable Vial's array by setting `#define VIAL_TAP_DANCE_ENTRIES 0` caused a `-Werror=array-bounds` error, because `vial.c` loops over the array regardless.
3. **The Type Name Syntax Error:** While building the state machine, an agent incorrectly used `qk_tap_dance_state_t` as a type. The correct QMK type is `tap_dance_state_t`.
4. **The Ghost Mouse Pointer Memory Bug (CRITICAL):** We initially used the `ACTION_TAP_DANCE_DOUBLE` macro inside `keyboard_post_init_user()` to inject the combo. This macro creates a config struct *on the stack*. When the init function ended, the struct was destroyed, leaving a dangling pointer in the tap dance array. When the key was pressed, the keyboard executed random garbage memory, resulting in wild, ghost mouse cursor movements!

**The Final Solution:** 
We kept `#define VIAL_TAP_DANCE_ENTRIES 12` so Vial could safely generate the array. Then, we declared `extern tap_dance_action_t tap_dance_actions[];` in `keymap.c`. Inside the `keyboard_post_init_user()` hook, we injected our Tap Dance into Index 0.
Crucially, we used `ACTION_TAP_DANCE_FN_ADVANCED` with static function pointers (`alt_lead_finished`, `alt_lead_reset`) and a global `td_state_t` struct, which perfectly avoided the stack memory destruction bug. 
*(Note: Because we overwrite Index 0 at boot, Index 0 in the Vial GUI is hijacked and will display garbage. Use Indices 1-11 for GUI tap dances).*

## 5. Overriding Tapping Terms
We wanted a custom tapping term specifically for our tap dances, but QMK handles this globally via `get_tapping_term()`.
- **The Issue:** `vial.c` already defines `get_tapping_term()`. If we define it in `keymap.c`, we get another `multiple definition` linker error.
- **The Solution:** We edited `quantum/vial.c` to rename its logic to `vial_get_tapping_term` and made its `get_tapping_term` wrapper `__attribute__((weak))`. This allowed us to define our own `get_tapping_term` in `keymap.c` to intercept specific keys (like `SP_L1` returning 170ms), while defaulting everything else back to `vial_get_tapping_term`. Note that `#define TAPPING_TERM_PER_KEY` must be enabled in `config.h` (or passed via compilation flags) for this to work.

## 6. The Combos Journey (And Why We Reverted It)
We attempted to hardcode a Combo (D + F = Output String) bypassing the GUI.
- **How it worked:** Similar to Tap Dances, we defined a `PROGMEM` key array, declared `extern combo_t key_combos[];`, and injected it into `key_combos[0]` inside `keyboard_post_init_user()`. Since Combos can only output basic keycodes, we mapped it to a custom `CB_BRAVE` macro and processed the string output in `process_record_user`.
- **Combo Term Limitation:** We discovered that overriding the combo timing (`get_combo_term`) requires editing `qmk_settings.c` to make it weak (just like we did for `vial.c`).
- **Conclusion:** The user decided they didn't need the combo, so all code related to it was reverted to keep the firmware clean. However, the memory-injection technique *does* work for Combos if ever needed in the future.

## 7. The Quantum Painter 200-Icon OLED Display
We are using Quantum Painter to drive the OLED displays (specifically `oled_hope.c` which is included directly into `keymap.c`).
A major feature of this keymap is a massive 200-icon Nerd Font display that lives at the bottom of the OLED screen and can be cycled forward/backward using the rotary encoders or `ICON_UP`/`ICON_DN` keys. 

### How `gen_nerd.py` Works
Because the RP2040 has massive amounts of memory, we compiled a custom 32px font file (`nerd_32.qff.c`) containing exactly 200 highly-specific icons chosen by the user. 
To manage this easily without manually editing binary font blobs, we built a Python script: `gen_nerd.py`. 
- **What it does:** It takes a Python array of raw Unicode Hex codepoints (e.g., `0xF0973`), opens a master `.ttf` font file, extracts *only* those specific 200 icons, and compiles them directly into Quantum Painter `.qff.c` and `.qff.h` files!
- **Visual Editing:** Inside `gen_nerd.py` and `oled_hope.c`, we formatted the arrays so that every single Hex code sits on its own line next to a C/Python comment containing the *actual rendered unicode icon* (e.g., `0xF0973, # 󰥳`). This allows the user to visually scroll through the source code and simply delete the lines of icons they no longer want!

### Instructions for Future AI Agents:
If the user asks you to add, remove, or modify icons in the rotation, follow this strict pipeline:
1. **Update `gen_nerd.py`:** Add or remove the hex codes in the `codepoints` array. (Keep the visual unicode comments intact if possible).
2. **Update `oled_hope.c`:** Make the exact same additions/removals to the `nerd_icon_list[]` C array so that the runtime rotation logic matches the compiled font.
3. **Execute the Script:** Run `python3 gen_nerd.py` in the terminal to parse the TTF file and re-generate the `nerd_32.qff.c` font arrays!
4. **Compile Firmware:** Once the python script succeeds, the firmware is ready to be flashed.

### The Randomizer (Shuffle Logic)
To prevent the user from seeing the exact same starting icon every time they plug in the keyboard, we implemented a full array shuffle.
- **How it works:** A secondary `uint8_t shuffled_indices[NUM_ICONS]` tracker array is created in RAM. 
- **The Trigger:** Inside `process_record_user`, the very first keypress of the keyboard triggers `shuffle_icons()`. We do this because `timer_read32()` provides a wildly chaotic millisecond seed based on human reaction time.
- **The Reshuffle:** The user also has an `ICN_RND` key mapped on Layer 2. When pressed, it resets the tracker and immediately re-shuffles the deck using a brand new chaotic seed, generating a completely new rotation sequence. 
- **Split Keyboard Quirk:** Remember that `process_record_user` only runs on the Master half (the side with the USB cable). But since the shuffle randomizes the array globally in memory on that first keypress, the rotation logic works perfectly no matter which encoder or key is used to cycle the icons!
