// clang-format off
#include QMK_KEYBOARD_H
#include "oled.c"

// 1. Setup Aliases (<7 chars)
#define DEL_WRD LCTL(KC_BSPC)     // Direct Windows shortcut 
#define CT_LEFT LCTL(KC_LEFT)
#define SF_LPRN LSFT_T(KC_LPRN)

// 2. Custom macros
#define DEL_LIN SAFE_RANGE         // Starts at the safe zone
#define SP_L1   (SAFE_RANGE + 1)   // Custom Space / Layer 1 / Layer 2

// 3. Fallback for TAPPING_TERM
#ifndef TAPPING_TERM
#define TAPPING_TERM 200
#endif

// 4. Custom State Machine for SP_L1
enum spc_state {
    SP_IDLE,
    SP_FIRST_PRESS,
    SP_HOLD_L1,
    SP_WAITING_DOUBLE,
    SP_HOLD_L2
};

static enum spc_state current_spc_state = SP_IDLE;
static uint16_t spc_timer = 0;

void matrix_scan_user(void) {
    if (current_spc_state == SP_FIRST_PRESS && timer_elapsed(spc_timer) >= TAPPING_TERM) {
        current_spc_state = SP_HOLD_L1;
        layer_on(1);
    } else if (current_spc_state == SP_WAITING_DOUBLE && timer_elapsed(spc_timer) >= TAPPING_TERM) {
        current_spc_state = SP_IDLE;
        tap_code(KC_SPC);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // If another key is pressed, handle any pending Space state first
    if (record->event.pressed && keycode != SP_L1) {
        if (current_spc_state == SP_FIRST_PRESS) {
            // User held Space and typed another key -> activate Layer 1 instantly
            current_spc_state = SP_HOLD_L1;
            layer_on(1);
        } else if (current_spc_state == SP_WAITING_DOUBLE) {
            // User tapped Space and then typed another key -> send Space first
            current_spc_state = SP_IDLE;
            tap_code(KC_SPC);
        }
    }

    switch (keycode) {
        case SP_L1:
            if (record->event.pressed) {
                if (current_spc_state == SP_WAITING_DOUBLE) {
                    // Double-tap! Activate Layer 2
                    current_spc_state = SP_HOLD_L2;
                    layer_on(2);
                } else {
                    // First press
                    current_spc_state = SP_FIRST_PRESS;
                    spc_timer = timer_read();
                }
            } else { // Released
                if (current_spc_state == SP_FIRST_PRESS) {
                    // Released before TAPPING_TERM and without other keypresses -> wait for double-tap
                    current_spc_state = SP_WAITING_DOUBLE;
                    spc_timer = timer_read();
                } else if (current_spc_state == SP_HOLD_L1) {
                    layer_off(1);
                    current_spc_state = SP_IDLE;
                } else if (current_spc_state == SP_HOLD_L2) {
                    layer_off(2);
                    current_spc_state = SP_IDLE;
                }
            }
            return false; // Intercept completely from QMK core

        case DEL_LIN:
            if (record->event.pressed) {
                // Selects to the start of the line, then deletes it
                tap_code16(LSFT(KC_HOME)); 
                tap_code(KC_BSPC);         
            }
            return false;
    }
    
    return true; // Pass all other keys through normally
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Layout Reference
.-----------------------------------------------------.                    .-----------------------------------------------------.
| 1234567| 1234567| 1234567| 1234567| 1234567| 1234567|                    | 1234567| 1234567| 1234567| 1234567| 1234567| 1234567|
|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
| 1234567| 1234567| 1234567| 1234567| 1234567| 1234567|                    | 1234567| 1234567| 1234567| 1234567| 1234567| 1234567|
|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
| 1234567| 1234567| 1234567| 1234567| 1234567| 1234567|                    | 1234567| 1234567| 1234567| 1234567| 1234567| 1234567|
|--------+--------+--------+--------+--------+--------+--------.  .--------+--------+--------+--------+--------+--------+--------|
| 1234567| 1234567| 1234567| 1234567| 1234567| 1234567| 1234567|  | 1234567| 1234567| 1234567| 1234567| 1234567| 1234567| 1234567|
'-----------------+--------+--------+--------+--------+--------+  +--------+--------+--------+--------+--------+-----------------'
* 		    	  | 1234567| 1234567| 1234567| 1234567| 1234567|  | 1234567| 1234567| 1234567| 1234567| 1234567| 					 
*                 +--------------------------------------------'  '--------------------------------------------'       
*/

[0] = LAYOUT(
  KC_GRV , KC_1   , KC_2   , QK_BOOT, KC_4   , KC_5   ,                      KC_6   , KC_7   , QK_BOOT, KC_9   , KC_0   , KC_GRV ,
  KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,                      KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_BSPC,
  KC_ESC , KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,                      KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT,
  SF_LPRN, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_MUTE,    XXXXXXX, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_RSFT,
                    KC_LGUI, KC_LCTL, KC_LALT, SP_L1  , KC_ENT ,    SP_L1  , KC_SPC , KC_RCTL, KC_RALT, KC_RGUI
),

[1] = LAYOUT(
  KC_GRV , KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  ,                      KC_F6  , KC_F7  , QK_BOOT, KC_F9  , KC_F10 , KC_GRV ,
  KC_TAB , DEL_LIN, DEL_WRD, KC_HOME, KC_END , KC_T   ,                      KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_BSPC,
  KC_UNDS, KC_A   , KC_LEFT, KC_UP  , KC_DOWN, KC_RGHT,                      KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, KC_SCLN, KC_QUOT,
  SF_LPRN, KC_Z   , KC_X   , KC_COMM, KC_BSPC, KC_DEL , KC_MUTE,    XXXXXXX, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_RSFT,
                    KC_LGUI, KC_LCTL, KC_LALT, KC_A   , KC_ENT ,    KC_SPC , MO(1)  , KC_RCTL, KC_RALT, KC_RGUI
),

[2] = LAYOUT(
  KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                      KC_6   , KC_7   , QK_BOOT, KC_9   , KC_0   , KC_GRV ,
  KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,                      KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_BSPC,
  KC_ESC , KC_UP  , CT_LEFT, KC_D   , KC_F   , KC_G   ,                      KC_M   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT,
  SF_LPRN, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_MUTE,    XXXXXXX, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_RSFT,
                    KC_LGUI, KC_LCTL, KC_LALT, MO(1)  , KC_ENT ,    KC_SPC , MO(1)  , KC_RCTL, KC_RALT, KC_RGUI
),

[3] = LAYOUT(
  KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                      KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_GRV ,
  KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,                      KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_BSPC,
  KC_ESC , KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,                      KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT,
  SF_LPRN, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_MUTE,    XXXXXXX, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_RSFT,
                    KC_LGUI, KC_LCTL, KC_LALT, MO(1)  , KC_ENT ,    KC_SPC , MO(1)  , KC_RCTL, KC_RALT, KC_RGUI
)
};


const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(KC_BRID, KC_BRIU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [2] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [3] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
};


#ifdef TAPPING_TERM_PER_KEY
// Declare the Vial fallback function
uint16_t vial_get_tapping_term(uint16_t keycode, keyrecord_t *record);

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case SP_L1:
			return 170; // Your custom tapping term logic
		default:
			// Fall back to Vial's dynamic settings and tap-dance configurations
			return vial_get_tapping_term(keycode, record);
	}
}
#endif
