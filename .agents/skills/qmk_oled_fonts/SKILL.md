---
name: QMK OLED Font Generation
description: Instructions for generating and converting fonts for the monochrome OLED display using QMK Quantum Painter tools.
---

# Generating Fonts for OLED

When you need to add a new font to the firmware for the OLED display, follow these steps to generate a 1-bit (`mono2`) font:

1. **Locate your font:** Place your `.ttf` or `.otf` font file inside the `fonts_dump/` directory.
2. **Generate PNG:** Run `qmk painter-make-font-image -a -s <SIZE> -o <OUTPUT_PNG> -f <INPUT_FONT_FILE>`.
   - **Crucial:** Always use `-a` to disable anti-aliasing for the monochrome OLED.
3. **Convert to C array:** Run `qmk painter-convert-font-image -f mono2 -i <INPUT_PNG> -o <OUTPUT_DIR>`.
4. **Add to firmware:** Include the `.qff.c` file in `rules.mk` and the `.qff.h` file in `oled_hope.c`.
5. **Check Limits:** Remember to increment `QUANTUM_PAINTER_NUM_FONTS` in `config.h` if you are adding an additional font to the active pool!
