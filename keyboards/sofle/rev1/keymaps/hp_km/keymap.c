#include "keycodes.h"
#include QMK_KEYBOARD_H
#include "oled_hope.c"

// 1. Setup Aliases (<=7 chars)
#define DEL_WRD LCTL(KC_BSPC)
#define CT_LEFT LCTL(KC_LEFT)
#define CT_RGHT LCTL(KC_RIGHT)
#define CT_ENT  LCTL(KC_ENT)
#define CS_ENT  LCTL(LSFT(KC_ENT))
#define CT_SLSH LCTL(KC_SLSH)
#define CT_HOME LCTL(KC_HOME)
#define CT_END  LCTL(KC_END)
#define SF_LPR  LSFT(KC_9)
#define SF_RPR  LSFT(KC_0)
#define MS_BTN1 KC_MS_BTN1
#define MS_BTN2 KC_MS_BTN2
#define MS_BTN3 KC_MS_BTN3
#define MS_LEFT KC_MS_LEFT
#define MS_DOWN KC_MS_DOWN
#define MS_UP   KC_MS_UP
#define MS_RGHT KC_MS_RIGHT
#define WH_DOWN KC_MS_WH_DOWN
#define WH_UP   KC_MS_WH_UP
#define WH_LEFT KC_MS_WH_LEFT
#define WH_RGHT KC_MS_WH_RIGHT
#define TP_ACL  TD(TD_ACL)
#define TP_BTN  TD(TD_BTN)
#define INV_ACL TD(TD_SCLN_INV)

// 2. Custom macros
#define DEL_LIN (QK_USER + 0)      // Starts at the safe zone
#define SP_L1   (QK_USER + 1)      // Custom Space / Layer 1 / Layer 2
#define VS_COMM (QK_USER + 2)      // VSCode Comment: Ctrl+K, Ctrl+C
#define VS_UNCO (QK_USER + 3)      // VSCode Uncomment: Ctrl+K, Ctrl+U
#define ICON_UP (QK_USER + 4)      // Cycle Icon Up
#define ICON_DN (QK_USER + 5)      // Cycle Icon Down
#define ICN_RND (QK_USER + 6)      // Randomize Icons
#define SS_T_UP (QK_USER + 7)          // Increase screensaver timeout
#define SS_T_DN (QK_USER + 8)          // Decrease screensaver timeout
#define OFF_T_UP (QK_USER + 14)        // Increase screenoff timeout
#define OFF_T_DN (QK_USER + 15)        // Decrease screenoff timeout
#define DLY_UP   (QK_USER + 16)    // Mod: Encoder increase string delay
#define DLY_DN   (QK_USER + 17)    // Mod: Encoder decrease string delay
#define SPD_CYC (QK_USER + 9)      // Mod: Cycle default mouse speed (10, 15, 20, 25, 30, 45, 60)
#define SPD_UP  (QK_USER + 10)     // Mod: Encoder increase mouse speed
#define SPD_DN  (QK_USER + 11)     // Mod: Encoder decrease mouse speed
#define SPD_TOG (QK_USER + 12)     // Mod: Toggle between 5-step and 2-step speed increments
#define SCR_TOG (QK_USER + 13)     // Mod: Toggle scroll speed
#define ONE_HND (QK_USER + 18)     // Toggle One-Handed Mode
#define STRD_TG (QK_USER + 19)     // Toggle Sturdy Layout
#define TD_GUI  TD(0)

// 3. Tap Dance Setup
enum {
    TD_GUI_LEADER = 0,
    TD_ACL,
    TD_BTN,
    TD_SCLN_INV
};

extern tap_dance_action_t tap_dance_actions[];

typedef struct {
    bool is_press_action;
    uint8_t state;
} td_state_t;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD
};

static td_state_t td_gui_state = { .is_press_action = true, .state = 0 };
static td_state_t td_acl_state = { .is_press_action = true, .state = 0 };
static td_state_t td_btn_state = { .is_press_action = true, .state = 0 };
static td_state_t td_scln_inv_state = { .is_press_action = true, .state = 0 };

uint8_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (!state->pressed) return DOUBLE_TAP;
        else return DOUBLE_HOLD;
    }
    return 0;
}

void gui_lead_finished(tap_dance_state_t *state, void *user_data) {
    td_gui_state.state = cur_dance(state);
    switch (td_gui_state.state) {
        case SINGLE_TAP:
        case SINGLE_HOLD:
            register_code(KC_LGUI);
            break;
        case DOUBLE_TAP:
            leader_start();
            break;
    }
}

void gui_lead_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_gui_state.state) {
        case SINGLE_TAP:
        case SINGLE_HOLD:
            unregister_code(KC_LGUI);
            break;
    }
    td_gui_state.state = 0;
}

void acl_finished(tap_dance_state_t *state, void *user_data) {
    td_acl_state.state = cur_dance(state);
    switch (td_acl_state.state) {
        case SINGLE_HOLD:
            register_code(KC_ACL1);
            break;
        case DOUBLE_HOLD:
            register_code(KC_ACL0);
            break;
    }
}

void acl_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_acl_state.state) {
        case SINGLE_HOLD:
            unregister_code(KC_ACL1);
            break;
        case DOUBLE_HOLD:
            unregister_code(KC_ACL0);
            break;
    }
    td_acl_state.state = 0;
}

void btn_finished(tap_dance_state_t *state, void *user_data) {
    td_btn_state.state = cur_dance(state);
    switch (td_btn_state.state) {
        case SINGLE_HOLD:
            register_code(KC_MS_BTN1);
            break;
        case DOUBLE_HOLD:
            register_code(KC_MS_BTN2);
            break;
    }
}

void btn_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_btn_state.state) {
        case SINGLE_HOLD:
            unregister_code(KC_MS_BTN1);
            break;
        case DOUBLE_HOLD:
            unregister_code(KC_MS_BTN2);
            break;
    }
    td_btn_state.state = 0;
}

void scln_inv_finished(tap_dance_state_t *state, void *user_data) {
    td_scln_inv_state.state = cur_dance(state);
    switch (td_scln_inv_state.state) {
        case SINGLE_TAP: {
            extern uint8_t dynamic_mouse_speed;
            extern uint8_t speed_cycle_mode;
            if (speed_cycle_mode == 0) {
                if (dynamic_mouse_speed == 5) dynamic_mouse_speed = 10;
                else if (dynamic_mouse_speed == 10) dynamic_mouse_speed = 15;
                else dynamic_mouse_speed = 5;
            } else {
                if (dynamic_mouse_speed == 7) dynamic_mouse_speed = 14;
                else dynamic_mouse_speed = 7;
            }
            break;
        }
        case SINGLE_HOLD:
            register_code(KC_ACL0);
            break;
        case DOUBLE_HOLD:
            register_code(KC_ACL1);
            break;
    }
}

void scln_inv_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_scln_inv_state.state) {
        case SINGLE_HOLD:
            unregister_code(KC_ACL0);
            break;
        case DOUBLE_HOLD:
            unregister_code(KC_ACL1);
            break;
    }
    td_scln_inv_state.state = 0;
}

typedef union {
    uint32_t raw;
    struct {
        uint8_t string_delay;
    };
} user_config_t;

user_config_t user_config;

uint8_t speed_cycle_mode = 1; // Mod: 0=low(5,10,15), 1=mid(7,14)
uint8_t dynamic_string_delay = 50; // Initial string delay

bool eeprom_needs_update = false;
uint32_t eeprom_timer = 0;

void eeconfig_init_user(void) {
    user_config.raw = 0;
    user_config.string_delay = 50;
    eeconfig_update_user(user_config.raw);
}

void qp_setup_oled(void);

void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();
    if (user_config.string_delay < 10 || user_config.string_delay > 250) {
        user_config.string_delay = 50;
    }
    dynamic_string_delay = user_config.string_delay;

    qp_setup_oled();
    tap_dance_action_t action = ACTION_TAP_DANCE_FN_ADVANCED(NULL, gui_lead_finished, gui_lead_reset);
    tap_dance_actions[TD_GUI_LEADER] = action;

    tap_dance_action_t action_acl = ACTION_TAP_DANCE_FN_ADVANCED(NULL, acl_finished, acl_reset);
    tap_dance_actions[TD_ACL] = action_acl;

    tap_dance_action_t action_btn = ACTION_TAP_DANCE_FN_ADVANCED(NULL, btn_finished, btn_reset);
    tap_dance_actions[TD_BTN] = action_btn;

    tap_dance_action_t action_scln_inv = ACTION_TAP_DANCE_FN_ADVANCED(NULL, scln_inv_finished, scln_inv_reset);
    tap_dance_actions[TD_SCLN_INV] = action_scln_inv;

    // Hijack Vial's first combo in RAM to hardcode J+K -> TG(3)
    extern combo_t key_combos[];
    extern uint16_t key_combos_keys[][5];

    key_combos_keys[0][0] = KC_J;
    key_combos_keys[0][1] = KC_K;
    key_combos_keys[0][2] = COMBO_END;
    key_combos[0].keys = key_combos_keys[0];
    key_combos[0].keycode = TG(3);

    key_combos_keys[1][0] = KC_MS_UP;
    key_combos_keys[1][1] = KC_MS_DOWN;
    key_combos_keys[1][2] = COMBO_END;
    key_combos[1].keys = key_combos_keys[1];
    key_combos[1].keycode = TG(3);

    // Left-hand combo to toggle One-Handed Mode (Q + T on Base / P + Y on Layer 4)
    key_combos_keys[2][0] = KC_Q;
    key_combos_keys[2][1] = KC_T;
    key_combos_keys[2][2] = COMBO_END;
    key_combos[2].keys = key_combos_keys[2];
    key_combos[2].keycode = ONE_HND;

    key_combos_keys[3][0] = KC_P;
    key_combos_keys[3][1] = KC_Y;
    key_combos_keys[3][2] = COMBO_END;
    key_combos[3].keys = key_combos_keys[3];
    key_combos[3].keycode = ONE_HND;

    // Left-hand combo to toggle Sturdy Layer (A + G on Base / S + Y on Sturdy)
    key_combos_keys[4][0] = KC_A;
    key_combos_keys[4][1] = KC_G;
    key_combos_keys[4][2] = COMBO_END;
    key_combos[4].keys = key_combos_keys[4];
    key_combos[4].keycode = STRD_TG;

    key_combos_keys[5][0] = KC_S;
    key_combos_keys[5][1] = KC_Y;
    key_combos_keys[5][2] = COMBO_END;
    key_combos[5].keys = key_combos_keys[5];
    key_combos[5].keycode = STRD_TG;

    // Parity combo: One-Handed Mode on Sturdy (V + P)
    key_combos_keys[6][0] = KC_V;
    key_combos_keys[6][1] = KC_P;
    key_combos_keys[6][2] = COMBO_END;
    key_combos[6].keys = key_combos_keys[6];
    key_combos[6].keycode = ONE_HND;

    // Parity combo: Mouse Mode on Sturdy (N + A)
    key_combos_keys[7][0] = KC_N;
    key_combos_keys[7][1] = KC_A;
    key_combos_keys[7][2] = COMBO_END;
    key_combos[7].keys = key_combos_keys[7];
    key_combos[7].keycode = TG(3);
}


// 4. Fallback for TAPPING_TERM
#ifndef TAPPING_TERM
#define TAPPING_TERM 200
#endif

// 5. Custom State Machine for SP_L1
enum spc_state {
    SP_IDLE,
    SP_FIRST_PRESS,
    SP_HOLD_L1,
    SP_WAITING_DOUBLE,
    SP_HOLD_L2
};

static enum spc_state current_spc_state = SP_IDLE;
static uint16_t spc_timer = 0;

bool one_handed_mode_enabled = false;
static bool one_handed_space_held = false;
static bool one_handed_space_used = false;

void matrix_scan_user(void) {
    if (eeprom_needs_update && timer_elapsed32(eeprom_timer) > 3000) {
        eeprom_needs_update = false;
        user_config_t current_config;
        current_config.raw = eeconfig_read_user();
        if (current_config.string_delay != dynamic_string_delay) {
            current_config.string_delay = dynamic_string_delay;
            eeconfig_update_user(current_config.raw);
        }
    }

    if (current_spc_state == SP_FIRST_PRESS && timer_elapsed(spc_timer) >= 130) {
        current_spc_state = SP_HOLD_L1;
        layer_on(1);
    } else if (current_spc_state == SP_WAITING_DOUBLE && timer_elapsed(spc_timer) >= 130) {
        current_spc_state = SP_IDLE;
        tap_code(KC_SPC);
    }

    static bool last_one_hand_state = false;
    if (one_handed_mode_enabled != last_one_hand_state) {
        last_one_hand_state = one_handed_mode_enabled;
        extern void print_status_wpm(void);
        print_status_wpm();
    }
}

// Force correct handedness for split keyboard regardless of which side is plugged into USB
bool is_keyboard_left_impl(void) {
#ifdef IS_LEFT_SIDE
    return true;
#else
    return false;
#endif
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    extern void oled_reset_screensaver(void);
    if (record->event.pressed) {
        oled_reset_screensaver();
    }

    // When One-Handed Mode is active:
    // - Baseline: Left-hand keys type normally (Layer 0: Q W E R T...).
    // - Hold Space: Shifts to Layer 4 momentarily (right-hand keys Y U I O P...).
    // - Release Space: Returns to Base layer. If no keys were typed, tap Space.
    if (one_handed_mode_enabled) {
        if (keycode == SP_L1 || keycode == KC_SPC) {
            if (record->event.pressed) {
                one_handed_space_held = true;
                one_handed_space_used = false;
                layer_on(4); // Momentarily activate right-hand chars while Space is held
            } else {
                one_handed_space_held = false;
                layer_off(4); // Revert back to normal left-hand chars
                if (!one_handed_space_used) {
                    tap_code(KC_SPC);
                }
            }
            return false;
        }

        // If any other key is typed while holding Space, mark it as used so releasing doesn't emit a space
        if (record->event.pressed && one_handed_space_held) {
            one_handed_space_used = true;
        }
    }

    // Seed RNG and shuffle icons on the very first keypress.
    // timer_read32() at this point is unpredictable (depends on human reaction time),
    // giving us a unique shuffle every power cycle.
    static bool first_key_done = false;
    if (record->event.pressed && !first_key_done) {
        first_key_done = true;
        srand(timer_read32());
        extern void shuffle_icons(void);
        shuffle_icons();
    }
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
        case STRD_TG:
            if (record->event.pressed) {
                if (default_layer_state & (1UL << 5)) {
                    default_layer_set(1UL << 0);
                } else {
                    default_layer_set(1UL << 5);
                }
                extern void print_status_wpm(void);
                print_status_wpm();
            }
            return false;
        case ONE_HND:
            if (record->event.pressed) {
                one_handed_mode_enabled = !one_handed_mode_enabled;
                if (!one_handed_mode_enabled) {
                    layer_off(4);
                    one_handed_space_held = false;
                    one_handed_space_used = false;
                }
                extern void print_status_wpm(void);
                print_status_wpm();
            }
            return false;
        case SPD_TOG:
            if (record->event.pressed) {
                speed_cycle_mode = (speed_cycle_mode + 1) % 2;
            }
            return false;
        case SPD_CYC:
        case SPD_UP:
            if (record->event.pressed) {
                extern uint8_t dynamic_mouse_speed;
                if (speed_cycle_mode == 0) {
                    if (dynamic_mouse_speed == 5) dynamic_mouse_speed = 10;
                    else if (dynamic_mouse_speed == 10) dynamic_mouse_speed = 15;
                    else dynamic_mouse_speed = 5;
                } else {
                    if (dynamic_mouse_speed == 7) dynamic_mouse_speed = 14;
                    else dynamic_mouse_speed = 7;
                }
            }
            return false;
        case SPD_DN:
            if (record->event.pressed) {
                extern uint8_t dynamic_mouse_speed;
                if (speed_cycle_mode == 0) {
                    if (dynamic_mouse_speed == 15) dynamic_mouse_speed = 10;
                    else if (dynamic_mouse_speed == 10) dynamic_mouse_speed = 5;
                    else dynamic_mouse_speed = 15;
                } else {
                    if (dynamic_mouse_speed == 14) dynamic_mouse_speed = 7;
                    else dynamic_mouse_speed = 14;
                }
            }
            return false;
        case SCR_TOG:
            if (record->event.pressed) {
                extern uint8_t dynamic_scroll_speed;
                dynamic_scroll_speed += 1;
                if (dynamic_scroll_speed > 5) dynamic_scroll_speed = 1;
            }
            return false;
        case DLY_UP:
            if (record->event.pressed) {
                if (dynamic_string_delay <= 245) dynamic_string_delay += 10;
                eeprom_needs_update = true;
                eeprom_timer = timer_read32();
                extern void show_dly_setting_screen(void);
                show_dly_setting_screen();
            }
            return false;
        case DLY_DN:
            if (record->event.pressed) {
                if (dynamic_string_delay >= 10) dynamic_string_delay -= 10;
                eeprom_needs_update = true;
                eeprom_timer = timer_read32();
                extern void show_dly_setting_screen(void);
                show_dly_setting_screen();
            }
            return false;
        case ICON_UP:
            if (record->event.pressed) {
                extern void cycle_nerd_icon(bool forward);
                cycle_nerd_icon(true);
            }
            return false;
        case ICON_DN:
            if (record->event.pressed) {
                extern void cycle_nerd_icon(bool forward);
                cycle_nerd_icon(false);
            }
            return false;
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

        case VS_COMM:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                tap_code(KC_K);
                tap_code(KC_C);
                unregister_code(KC_LCTL);
            }
            return false;

        case VS_UNCO:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                tap_code(KC_K);
                tap_code(KC_U);
                unregister_code(KC_LCTL);
            }
            return false;

        case ICN_RND:
            if (record->event.pressed) {
                extern void reshuffle_icons(void);
                reshuffle_icons();
            }
            return false;

        case SS_T_UP:
            if (record->event.pressed) {
                extern uint32_t screensaver_timeout;
                screensaver_timeout += 1000;
                extern void show_timeout_setting_screen(void);
                show_timeout_setting_screen();
            }
            return false;

        case SS_T_DN:
            if (record->event.pressed) {
                extern uint32_t screensaver_timeout;
                if (screensaver_timeout > 1000) screensaver_timeout -= 1000;
                extern void show_timeout_setting_screen(void);
                show_timeout_setting_screen();
            }
            return false;

        case OFF_T_UP:
            if (record->event.pressed) {
                extern uint32_t screenoff_timeout;
                screenoff_timeout += 10000;
                extern void show_timeout_setting_screen(void);
                show_timeout_setting_screen();
            }
            return false;

        case OFF_T_DN:
            if (record->event.pressed) {
                extern uint32_t screenoff_timeout;
                if (screenoff_timeout > 10000) screenoff_timeout -= 10000;
                extern void show_timeout_setting_screen(void);
                show_timeout_setting_screen();
            }
            return false;

    }

    return true; // Pass all other keys through normally
}

// clang-format off

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
*                 | 1234567| 1234567| 1234567| 1234567| 1234567|  | 1234567| 1234567| 1234567| 1234567| 1234567|
*                 +--------------------------------------------'  '--------------------------------------------+
*/

[0] = LAYOUT(
  KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                      KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS,
  KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,                      KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , TG(3)  ,
  KC_ESC , KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,                      KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT,
  KC_LSFT, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_MUTE,    XXXXXXX, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_RSFT,
                    TD_GUI , KC_LCTL, KC_LALT, SP_L1  , KC_ENT ,    SP_L1  , KC_SPC , KC_RCTL, KC_RALT, KC_RGUI
),

[1] = LAYOUT(
  KC_PGUP, KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  ,                      KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_MINS,
  KC_PGDN, DEL_LIN, DEL_WRD, KC_HOME, KC_END , CT_ENT ,                      KC_Y   , KC_U   , KC_HOME, KC_END , SF_LPR , SF_RPR ,
  KC_UNDS, CT_SLSH, KC_LEFT, KC_UP  , KC_DOWN, KC_RGHT,                      KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, SF_LPR , KC_EQL ,
  KC_LSFT, VS_COMM, VS_UNCO, KC_COMM, KC_BSPC, KC_DEL , KC_MUTE,    XXXXXXX, KC_BSPC, KC_DEL , KC_LBRC, KC_RBRC, KC_EQL , KC_BSLS,
                    TD_GUI , KC_LCTL, KC_LALT, KC_NO  , KC_ENT ,    KC_NO  , KC_SPC , KC_RCTL, KC_RALT, KC_RGUI
),

[2] = LAYOUT(
  KC_GRV , KC_1   , KC_2   , QK_BOOT, KC_4   , KC_5   ,                      KC_6   , KC_7   , QK_BOOT, KC_9   , KC_0   , KC_MINS,
  KC_TAB , KC_F7  , KC_F8  , CT_HOME, CT_END , CS_ENT ,                      KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_BSPC,
  KC_MINS, QK_LEAD, CT_LEFT, KC_D   , KC_F   , CT_RGHT,                      CT_LEFT, KC_J   , KC_K   , CT_RGHT, KC_SCLN, KC_QUOT,
  KC_LSFT, KC_Z   , KC_X   , ICN_RND, ICON_DN, ICON_UP, KC_MUTE,    XXXXXXX, ICON_DN, ICON_UP, ICN_RND, KC_DOT , KC_SLSH, KC_RSFT,
                    TD_GUI , KC_LCTL, KC_LALT, KC_NO  , KC_ENT ,    KC_NO  , MO(1)  , KC_RCTL, KC_RALT, KC_RGUI
),

[3] = LAYOUT(
  KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                      KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS,
  KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,                      SPD_TOG, SCR_TOG, MS_BTN1, MS_BTN2, MS_BTN3, TG(3)  ,
  KC_ESC , KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,                      MS_LEFT, MS_DOWN, MS_UP  , MS_RGHT, INV_ACL, KC_QUOT,
  KC_LSFT, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_MUTE,    XXXXXXX, SCR_TOG, WH_DOWN, WH_UP  , WH_LEFT, WH_RGHT, KC_RSFT,
                    TD_GUI , KC_LCTL, KC_LALT, KC_TRNS, KC_ENT ,    TP_ACL , TP_BTN , KC_ACL2, KC_RALT, SPD_CYC
),

// Layer 4: One-Handed Reverse (Mirrors right-hand keys on Left half when Space is held; Esc becomes Backspace)
[4] = LAYOUT(
  KC_MINS, KC_0   , KC_9   , KC_8   , KC_7   , KC_6   ,                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_QUOT, KC_P   , KC_O   , KC_I   , KC_U   , KC_Y   ,                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_BSPC, KC_SCLN, KC_L   , KC_K   , KC_J   , KC_H   ,                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_ENT , KC_SLSH, KC_DOT , KC_COMM, KC_M   , KC_N   , KC_MUTE,    XXXXXXX, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                    TD_GUI , KC_LCTL, KC_LALT, SP_L1  , KC_ENT ,    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
),

// Layer 5: Sturdy Layout (by Oxey)
[5] = LAYOUT(
  KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                      KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS,
  KC_TAB , KC_V   , KC_M   , KC_L   , KC_C   , KC_P   ,                      KC_X   , KC_F   , KC_O   , KC_U   , KC_J   , TG(3)  ,
  KC_ESC , KC_S   , KC_T   , KC_R   , KC_D   , KC_Y   ,                      KC_DOT , KC_N   , KC_A   , KC_E   , KC_I   , KC_SLSH,
  KC_LSFT, KC_Z   , KC_K   , KC_Q   , KC_G   , KC_W   , KC_MUTE,    XXXXXXX, KC_B   , KC_H   , KC_QUOT, KC_SCLN, KC_COMM, KC_RSFT,
                    TD_GUI , KC_LCTL, KC_LALT, SP_L1  , KC_ENT ,    SP_L1  , KC_SPC , KC_RCTL, KC_RALT, KC_RGUI
)
};


const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(KC_BRID, KC_BRIU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(SS_T_DN, SS_T_UP), ENCODER_CCW_CW(OFF_T_DN, OFF_T_UP) },
    [2] = { ENCODER_CCW_CW(DLY_DN, DLY_UP),   ENCODER_CCW_CW(DLY_DN, DLY_UP) },
    [3] = { ENCODER_CCW_CW(SPD_DN, SPD_UP),   ENCODER_CCW_CW(SPD_DN, SPD_UP) },
    [4] = { ENCODER_CCW_CW(KC_BRID, KC_BRIU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [5] = { ENCODER_CCW_CW(KC_BRID, KC_BRIU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
};

// clang-format on

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

void leader_start_user(void) {
    extern void oled_set_leader_state(bool active);
    oled_set_leader_state(true);
}

void leader_end_user(void) {
    extern void oled_set_leader_state(bool active);
    oled_set_leader_state(false);

    if (leader_sequence_one_key(KC_A)) {
        SEND_STRING_DELAY("bravewater123@", dynamic_string_delay);
    } else if (leader_sequence_one_key(KC_S)) {
        SEND_STRING_DELAY("bravewater123#", dynamic_string_delay);
    } else if (leader_sequence_two_keys(KC_A, KC_A)) {
        // Example: double key uses slower delay, or just use same dynamic delay
        SEND_STRING_DELAY("bravewater123@", (dynamic_string_delay > 125) ? 250 : dynamic_string_delay * 2);
    }
}

