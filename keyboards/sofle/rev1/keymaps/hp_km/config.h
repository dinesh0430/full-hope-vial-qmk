// clang-format off

#pragma once

// Custom split transaction IDs for syncing OLED state from master to slave.
// This macro injects our transaction ID into QMK's serial_transaction_id enum.
#define SPLIT_TRANSACTION_IDS_USER SPLIT_OLED_SYNC

#define BOOTMAGIC_ROW 0
#define BOOTMAGIC_COLUMN 0
#define BOOTMAGIC_ROW_RIGHT 9
#define BOOTMAGIC_COLUMN_RIGHT 0

#define EE_HANDS

// Unified Keyboard Identification
#undef PRODUCT
#undef KEYBOARD_NAME
#define PRODUCT "Sofle"
#define KEYBOARD_NAME "Sofle"
#define VIAL_KEYBOARD_UID { 0x05, 0xCD, 0x9F, 0x8A, 0xF4, 0xDF, 0xDE, 0xB2 }

// Maxed Out Vial Overrides (Thanks to 8MB Flash Storage!)
#define DYNAMIC_KEYMAP_LAYER_COUNT 5
#define DYNAMIC_KEYMAP_MACRO_COUNT 32
#define VIAL_TAP_DANCE_ENTRIES 12

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#undef  RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U

#define DEBUG_MATRIX_SCAN_RATE

#define I2C0_CLOCK_SPEED 400000
#define I2C1_CLOCK_SPEED 400000
#define QUANTUM_PAINTER_NUM_FONTS 4 // 4 active fonts: fool_16, fool_32, rubik_48, nerd_32
#define QUANTUM_PAINTER_NUM_IMAGES 34

#ifndef TAPPING_TERM_PER_KEY
#define TAPPING_TERM_PER_KEY
#endif

// Disable the Leader timer entirely for stress-free single-key macros
#define LEADER_NO_TIMEOUT

// Disable native Quantum Painter display sleep (custom 1-min screensaver & 5-min sleep handled in oled_hope.c)
#define QUANTUM_PAINTER_DISPLAY_TIMEOUT 0

// Mouse Key Tuning (Near-constant speed, minimal acceleration)
#define MK_COMBINED
// Custom modification: We edited `quantum/mousekey.c` directly to add support for the `MK_COMBINED_ACL1_SPEED` macro.
// Normally in `MK_COMBINED` mode, the `KC_ACL1` speed is strictly tied to `(MOUSEKEY_MOVE_DELTA * MOUSEKEY_MAX_SPEED) / 2`.
// By adding this macro support to QMK core, we can set `KC_ACL1` to a specific constant speed (pixels per tick)
// without affecting the default, un-accelerated starting/max speeds.
#define MK_COMBINED_ACL1_SPEED 20
#define MOUSEKEY_MOVE_DELTA   14
#define MOUSEKEY_DELAY        0
#define MOUSEKEY_INTERVAL     16
#define MOUSEKEY_MAX_SPEED    8
#define MOUSEKEY_TIME_TO_MAX  250

// Scroll Wheel Tuning
#define MOUSEKEY_WHEEL_DELAY        0
#define MOUSEKEY_WHEEL_INTERVAL     50
#define MOUSEKEY_WHEEL_MAX_SPEED    6
#define MOUSEKEY_WHEEL_TIME_TO_MAX  80

// Smoother HID reports
#define MOUSE_EXTENDED_REPORT

// Onboard KB2040 NeoPixel RGB LED (GPIO 17)
#undef RGB_DI_PIN
#define RGB_DI_PIN 17U
#undef WS2812_DI_PIN
#define WS2812_DI_PIN 17U
#undef RGBLED_NUM
#undef RGBLIGHT_LED_COUNT
#define RGBLIGHT_LED_COUNT 2
#undef RGBLED_SPLIT
#define RGBLED_SPLIT { 1, 1 }
#define RGBLIGHT_LIMIT_VAL 255 // Full maximum brightness (100%)

// RGBLIGHT Animation Effects
#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_RAINBOW_MOOD
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#define RGBLIGHT_EFFECT_TWINKLE
#define RGBLIGHT_EFFECT_ALTERNATING
#define RGBLIGHT_EFFECT_STATIC_GRADIENT
