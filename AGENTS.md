# QMK Firmware Project Rules & Context

This file contains critical context and rules for agents working on this QMK firmware project (Sofle Rev1 with KB2040).

## Build Instructions
- Always compile the firmware for the KB2040 MCU.
- **Left Side:** `qmk compile -kb sofle/rev1 -km hptest -e CONVERT_TO=kb2040 -e EXTRAFLAGS="-DIS_LEFT_SIDE"`
- **Right Side:** `qmk compile -kb sofle/rev1 -km hptest -e CONVERT_TO=kb2040 -e EXTRAFLAGS="-DIS_RIGHT_SIDE"`
- **Testing Tip:** When making rapid changes to UI or logic, you only need to compile and flash the **Left Side** to test the change. You do not need to compile both sides every time.
- **Automation (Hooks):** The exact Left Side compilation command above is whitelisted via Antigravity lifecycle hooks.
  - `.agents/hooks.json` intercepts all `run_command` tool calls and pipes them to `.agents/allow_compile.py`.
  - `.agents/allow_compile.py` parses the requested shell command. If it perfectly matches the Left Side compile command, it returns `{"decision": "allow"}` to bypass the terminal prompt. For all other commands, it returns `{"decision": "ask"}` to safely prompt the user.
  - As an agent, you can execute the left-side compile command as a background task without ever prompting the user for permission.
- **Connectivity Modes:** Thanks to the RP2040 (KB2040) native hardware USB detection, the firmware dynamically detects master/slave roles automatically without extra flags. It seamlessly supports three configurations:
  - **Mode 1:** Left connected via USB to host, Right connected to Left via TRRS (Standard split behavior).
  - **Mode 2:** Right connected via USB to host, Left connected to Right via TRRS (Reversed split behavior).
  - **Mode 3:** Both Left and Right connected directly to the host via USB (No TRRS cable). Both halves will instantly detect USB, become masters, and function as two independent halves simultaneously.

## Quantum Painter & OLED Font Guidelines
- **Font Limit (`QUANTUM_PAINTER_NUM_FONTS`):** The number of loaded fonts is hardcoded in `config.h`. If you load a new font via `qp_load_font_mem()` and the screen goes completely blank/white, it means the font pointer returned NULL because you exceeded this limit. Always increment `QUANTUM_PAINTER_NUM_FONTS` when adding new fonts to fix this!
- **Font Sources (`fonts_dump/`):** There are several raw `.ttf` and `.otf` source fonts stored in the `keyboards/sofle/rev1/keymaps/hptest/fonts_dump/` folder. If existing fonts have rendering issues at certain sizes, you can use these source files to generate new fonts and sizes.
- **Font Generation (`mono2`):** When generating fonts for this monochrome OLED, use `qmk painter-make-font-image -a` (no anti-aliasing) and convert to `mono2`. Monospace or dedicated pixel fonts (like `modernpixels` or `SFMono`) often look best at small sizes (e.g., 16px).
- **Color Inversion:** To display inverted text (e.g. black text on a white background), first draw a white background using `qp_rect(display, x, y, width, height, 0, 0, 255, true);`, then draw the text over it using `qp_drawtext_recolor(display, x, y, font, "Text", 0, 0, 0, 0, 0, 255);`.

## Split Communication (`oled_sync_data_t`)
- **RP2040 Memory Alignment (Hard Faults):** The Cortex-M0+ on the KB2040 does **not** support unaligned 32-bit memory access. 
- Any struct sent over the custom split RPC must be packed using `__attribute__((packed))`.
- When reading multi-byte variables (like `uint32_t`) from the raw `in_data` buffer in the slave handler, **you must use `memcpy`** to copy the data into a local variable first. Directly casting or dereferencing misaligned pointers will cause the slave half to hard fault and freeze.
- **RPC Payload Limit:** Custom split transactions have a hard cap of 32 bytes (`RPC_M2S_BUFFER_SIZE`). Do not exceed this in your sync structs.
