# QMK Firmware Project Rules & Context

This file contains critical context and rules for agents working on this QMK firmware project (Sofle Rev1 with KB2040).

## Build Instructions
- Always compile the firmware for the KB2040 MCU.
- **Left Side Compilation (ONLY COMPILE THIS):**
  `qmk compile -kb sofle/rev1 -km 34_key -e CONVERT_TO=kb2040 -e EXTRAFLAGS="-DIS_LEFT_SIDE" -e TARGET=sofle_rev1_34_key_left`
- **STRICT RULE - DO NOT COMPILE RIGHT SIDE:** You must **ONLY** compile the **Left Side**. Never compile or run commands for the Right Side unless the user explicitly asks for it in the chat prompt.
- **Automation (Hooks):** The exact Left Side compilation command above is whitelisted via Antigravity lifecycle hooks.
  - `.agents/hooks.json` intercepts all `run_command` tool calls and pipes them to `.agents/allow_compile.py`.
  - `.agents/allow_compile.py` parses the requested shell command. If it perfectly matches the Left Side compile command, it returns `{"decision": "allow"}` to bypass the terminal prompt.
  - As an agent, you can execute the left-side compile command as a background task without ever prompting the user for permission.
- **Connectivity Modes:** Thanks to the RP2040 (KB2040) native hardware USB detection, the firmware dynamically detects master/slave roles automatically without extra flags. It seamlessly supports three configurations:
  - **Mode 1:** Left connected via USB to host, Right connected to Left via TRRS (Standard split behavior).
  - **Mode 2:** Right connected via USB to host, Left connected to Right via TRRS (Reversed split behavior).
  - **Mode 3:** Both Left and Right connected directly to the host via USB (No TRRS cable). Both halves will instantly detect USB, become masters, and function as two independent halves simultaneously.

## Quantum Painter & OLED Font Guidelines
- **Font Limit (`QUANTUM_PAINTER_NUM_FONTS`):** The number of loaded fonts is hardcoded in `config.h`. If you load a new font via `qp_load_font_mem()` and the screen goes completely blank/white, it means the font pointer returned NULL because you exceeded this limit. Always increment `QUANTUM_PAINTER_NUM_FONTS` when adding new fonts to fix this!
- **Font Sources (`fonts_dump/`):** There are several raw `.ttf` and `.otf` source fonts stored in the `keyboards/sofle/rev1/keymaps/34_key/fonts_dump/` folder. If existing fonts have rendering issues at certain sizes, you can use these source files to generate new fonts and sizes.
- **Font Generation (`mono2`):** When generating fonts for this monochrome OLED, use `qmk painter-make-font-image -a` (no anti-aliasing) and convert to `mono2`. Monospace or dedicated pixel fonts (like `modernpixels` or `SFMono`) often look best at small sizes (e.g., 16px).
- **Color Inversion:** To display inverted text (e.g. black text on a white background), first draw a white background using `qp_rect(display, x, y, width, height, 0, 0, 255, true);`, then draw the text over it using `qp_drawtext_recolor(display, x, y, font, "Text", 0, 0, 0, 0, 0, 255);`.

## Split Communication (`oled_sync_data_t`)
- **RP2040 Memory Alignment (Hard Faults):** The Cortex-M0+ on the KB2040 does **not** support unaligned 32-bit memory access. 
- Any struct sent over the custom split RPC must be packed using `__attribute__((packed))`.
- When reading multi-byte variables (like `uint32_t`) from the raw `in_data` buffer in the slave handler, **you must use `memcpy`** to copy the data into a local variable first. Directly casting or dereferencing misaligned pointers will cause the slave half to hard fault and freeze.
- **RPC Payload Limit:** Custom split transactions have a hard cap of 32 bytes (`RPC_M2S_BUFFER_SIZE`). Do not exceed this in your sync structs.

## Keymap Formatting & Keycode Length Rules
- **Column Alignment & Width Limit:** Every keycode entry in the `keymaps[][MATRIX_ROWS][MATRIX_COLS]` matrix in `keymap.c` must have a maximum length of **7 characters** (padded with trailing spaces if shorter) so that all commas line up vertically and the grid remains human-readable.
- **Short Aliases for Long Keycodes:** If a keycode or action exceeds 7 characters (e.g. `LCTL(KC_BSPC)` or `QK_SWAP_HANDS_TOGGLE`), define a meaningful `#define` alias of **<= 7 characters** at the top of `keymap.c` and use that alias in the matrix.
  - **Example:**
    ```c
    // Top of keymap.c (Aliases <= 7 chars)
    #define DEL_WRD LCTL(KC_BSPC)  // 7 chars: replaces 14-char keycode
    #define SH_TOGG QK_SWAP_HANDS_TOGGLE // 7 chars: replaces 21-char keycode
    ```
    ```c
    // In keymaps matrix:
    [0] = LAYOUT(
      KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                      KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS,
      KC_TAB , KC_Q   , DEL_WRD, KC_E   , KC_R   , KC_T   ,                      KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , SH_TOGG
    )
    ```
