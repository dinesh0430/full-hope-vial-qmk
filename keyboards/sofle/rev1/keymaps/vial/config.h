// clang-format off

#pragma once

#define BOOTMAGIC_ROW 0
#define BOOTMAGIC_COLUMN 0
#define BOOTMAGIC_ROW_RIGHT 9
#define BOOTMAGIC_COLUMN_RIGHT 0

#if defined(IS_LEFT_SIDE)
    // Left Side Explicit Configuration
    #define VIAL_KEYBOARD_UID { 0x05, 0xCD, 0x9F, 0x8A, 0xF4, 0xDF, 0xDE, 0xB2 }
    // Uses default PRODUCT_ID (0x0287) from your info.json/rules.mk
	// // #define BOOTMAGIC_ROW 0
    // // #define BOOTMAGIC_COLUMN 0

#elif defined(IS_RIGHT_SIDE)
    // Right Side Explicit Configuration
    #define VIAL_KEYBOARD_UID { 0x05, 0xCD, 0x9F, 0x8A, 0xF4, 0xDF, 0xDE, 0xB3 }
    #undef  PRODUCT_ID
    #define PRODUCT_ID 0x0288
    #define MASTER_RIGHT
	// // #define BOOTMAGIC_ROW 9
    // // #define BOOTMAGIC_COLUMN 0

#else
    // Fail-safe: Stop the build if you forgot to specify a side in the terminal
    #error "Error: You must specify -DIS_LEFT_SIDE or -DIS_RIGHT_SIDE in your build command!"
#endif

// The four corners; these are no longer required since we set VIAL_INSECURE = yes in rules.mk
// #define VIAL_UNLOCK_COMBO_ROWS { 0, 1, 2, 3 }
// #define VIAL_UNLOCK_COMBO_COLS { 0, 0, 0, 0 }

// Maxed Out Vial Overrides (Thanks to 8MB Flash Storage!)
#define DYNAMIC_KEYMAP_LAYER_COUNT 8
#define DYNAMIC_KEYMAP_MACRO_COUNT 32
#define VIAL_TAP_DANCE_ENTRIES 12
#define VIAL_COMBO_ENTRIES 16

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#undef  RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U

// // #define CONSOLE_ENABLE
#define DEBUG_MATRIX_SCAN_RATE

// #define TAPPING_TERM_PER_KEY