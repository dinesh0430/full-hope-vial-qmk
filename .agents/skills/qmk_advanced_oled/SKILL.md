---
name: qmk_advanced_oled
description: >-
  Instructions for creating temporary OLED UI overlays, handling split-sync blocking 
  to prevent slave-side lag, and saving encoder-driven settings to EEPROM using lazy-writes.
---

# QMK Advanced OLED & EEPROM Guide

This skill outlines the patterns used in this project for building temporary OLED settings screens, synchronizing (or intentionally isolating) them across split halves, and safely saving settings to memory.

## 1. OLED Overlays & Timeouts
When adding a new temporary setting screen (like modifying a delay or timeout), it should pop up over the normal display and disappear after a short delay.

*   **Triggering:** Use a `display_override` integer (e.g., `3` for screensaver timeouts, `4` for string delay) to signal `oled_hope.c` to draw the overlay.
*   **Timeout Cleanup:** In `oled_hope.c`, inside `oled_task_user()`, ensure your override ID is included in the 1-second timeout reset block. If you forget to add your ID to the `if (my_override == 3 || my_override == 4)` condition, the screen will get stuck and redraw indefinitely when keys are pressed.

## 2. Split Sync Blocking (Preventing Slave Lag)
By default, `display_override` is synchronized to the slave half every 50ms so both screens match. However, some temporary overlays (like the `DLY` screen) should only show on the master half (where the encoder is being turned) to avoid visual flashing and processing lag on the slave.

*   **How to Block:** In `oled_hope.c` (around the `transaction_rpc_send` block), intercept the `sync_data.display_override` assignment.
*   **Example:** `sync_data.display_override = (display_override == 4) ? 0 : display_override;`
    This ensures the slave continues rendering the normal layer screen (`0`) while the master shows the temporary overlay (`4`).

## 3. EEPROM Lazy-Writing for Encoders
When using an encoder to change a setting, writing to EEPROM on every single click will quickly destroy the flash memory (limited to ~100k writes) and cause severe matrix stuttering.

Instead, use the **Lazy-Write** pattern:

1.  **RAM Update:** In the encoder action (`DLY_UP`/`DLY_DN`), instantly update the live RAM variable (e.g., `dynamic_string_delay += 10`).
2.  **Start Timer:** Set `eeprom_needs_update = true;` and `eeprom_timer = timer_read32();`.
3.  **Deferred Commit:** In `matrix_scan_user()`, wait until 3 seconds have passed since the last encoder turn before writing to EEPROM.
    ```c
    if (eeprom_needs_update && timer_elapsed32(eeprom_timer) > 3000) {
        eeprom_needs_update = false;
        user_config_t current_config;
        current_config.raw = eeconfig_read_user();
        if (current_config.string_delay != dynamic_string_delay) {
            current_config.string_delay = dynamic_string_delay;
            eeconfig_update_user(current_config.raw);
        }
    }
    ```

### EEPROM Initialization
Always ensure your custom variables are stored in a `typedef union user_config_t` and properly initialized in `eeconfig_init_user()` (providing safe defaults) and loaded on boot in `keyboard_post_init_user()`.
