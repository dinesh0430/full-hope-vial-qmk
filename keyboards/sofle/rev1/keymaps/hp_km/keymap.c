#include QMK_KEYBOARD_H

// =============================================================================
// Shared Global State (Declared before including oled_hope.c)
// =============================================================================
uint8_t speed_cycle_mode = 1;       // Mouse speed mode: 0 = Low (5, 10, 15), 1 = Mid (7, 14)
uint8_t dynamic_string_delay = 50;  // Milliseconds delay between characters in macros (RAM only)

#include "oled_hope.c"

// =============================================================================
// 1. Keycode Aliases (<= 7 Characters for Matrix Column Alignment)
// =============================================================================
// Navigation & Word Editing
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

// Mouse Buttons, Movement & Scrolling
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

// Tap Dance Aliases
#define TD_GUI  TD(TD_GUI_LEADER)
#define TP_ACL  TD(TD_ACL)
#define TP_BTN  TD(TD_BTN)
#define INV_ACL TD(TD_SCLN_INV)

// =============================================================================
// 2. Custom Keycodes
// =============================================================================
#define DEL_LIN  (QK_USER + 0)   // Delete Line: Shift+Home, then Backspace
#define SP_L1    (QK_USER + 1)   // Multi-function Space: Tap=Space, Hold=Nav(L2), Dbl-Hold=Media(L3)
#define VS_COMM  (QK_USER + 2)   // VSCode Comment: Ctrl+K, Ctrl+C
#define VS_UNCO  (QK_USER + 3)   // VSCode Uncomment: Ctrl+K, Ctrl+U
#define ICON_UP  (QK_USER + 4)   // Cycle OLED Nerd Icon forward
#define ICON_DN  (QK_USER + 5)   // Cycle OLED Nerd Icon backward
#define ICN_RND  (QK_USER + 6)   // Reshuffle OLED Nerd Icons
#define SS_T_UP  (QK_USER + 7)   // Increase screensaver timeout (+1s)
#define SS_T_DN  (QK_USER + 8)   // Decrease screensaver timeout (-1s)
#define SPD_CYC  (QK_USER + 9)   // Cycle mouse speed presets
#define SPD_UP   (QK_USER + 10)  // Increase mouse speed
#define SPD_DN   (QK_USER + 11)  // Decrease mouse speed
#define SPD_TOG  (QK_USER + 12)  // Toggle mouse speed stepping mode (Low <-> Mid)
#define SCR_TOG  (QK_USER + 13)  // Cycle scroll speed (1 to 5)
#define OFF_T_UP (QK_USER + 14)  // Increase screen-off timeout (+10s)
#define OFF_T_DN (QK_USER + 15)  // Decrease screen-off timeout (-10s)
#define DLY_UP   (QK_USER + 16)  // Increase string send delay (+10ms)
#define DLY_DN   (QK_USER + 17)  // Decrease string send delay (-10ms)
#define STRD_TG  (QK_USER + 18)  // Toggle Default Base Layer: 0 (QWERTY) <-> 1 (Sturdy)

// =============================================================================
// 3. Helper Functions
// =============================================================================
static void cycle_mouse_speed(bool forward) {
    if (speed_cycle_mode == 0) {
        // Low mode presets: 5 -> 10 -> 15 -> 5
        if (forward) {
            if (dynamic_mouse_speed == 5) dynamic_mouse_speed = 10;
            else if (dynamic_mouse_speed == 10) dynamic_mouse_speed = 15;
            else dynamic_mouse_speed = 5;
        } else {
            if (dynamic_mouse_speed == 15) dynamic_mouse_speed = 10;
            else if (dynamic_mouse_speed == 10) dynamic_mouse_speed = 5;
            else dynamic_mouse_speed = 15;
        }
    } else {
        // Mid mode presets: 7 -> 14 -> 7
        if (dynamic_mouse_speed == 7) dynamic_mouse_speed = 14;
        else dynamic_mouse_speed = 7;
    }
}

// =============================================================================
// 4. Tap Dance Setup
// =============================================================================
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

static td_state_t td_gui_state      = { .is_press_action = true, .state = 0 };
static td_state_t td_acl_state      = { .is_press_action = true, .state = 0 };
static td_state_t td_btn_state      = { .is_press_action = true, .state = 0 };
static td_state_t td_scln_inv_state = { .is_press_action = true, .state = 0 };

static uint8_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        return (!state->pressed) ? SINGLE_TAP : SINGLE_HOLD;
    } else if (state->count == 2) {
        return (!state->pressed) ? DOUBLE_TAP : DOUBLE_HOLD;
    }
    return 0;
}

// TD_GUI_LEADER: Tap/Hold = LGUI | Double-tap = Leader Key
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
    if (td_gui_state.state == SINGLE_TAP || td_gui_state.state == SINGLE_HOLD) {
        unregister_code(KC_LGUI);
    }
    td_gui_state.state = 0;
}

// TD_ACL: Single hold = Acceleration 1 | Double hold = Acceleration 0
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
    if (td_acl_state.state == SINGLE_HOLD) {
        unregister_code(KC_ACL1);
    } else if (td_acl_state.state == DOUBLE_HOLD) {
        unregister_code(KC_ACL0);
    }
    td_acl_state.state = 0;
}

// TD_BTN: Single hold = Mouse Button 1 | Double hold = Mouse Button 2
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
    if (td_btn_state.state == SINGLE_HOLD) {
        unregister_code(KC_MS_BTN1);
    } else if (td_btn_state.state == DOUBLE_HOLD) {
        unregister_code(KC_MS_BTN2);
    }
    td_btn_state.state = 0;
}

// TD_SCLN_INV: Tap = Cycle Speed | Single hold = ACL0 | Double hold = ACL1
void scln_inv_finished(tap_dance_state_t *state, void *user_data) {
    td_scln_inv_state.state = cur_dance(state);
    switch (td_scln_inv_state.state) {
        case SINGLE_TAP:
            cycle_mouse_speed(true);
            break;
        case SINGLE_HOLD:
            register_code(KC_ACL0);
            break;
        case DOUBLE_HOLD:
            register_code(KC_ACL1);
            break;
    }
}

void scln_inv_reset(tap_dance_state_t *state, void *user_data) {
    if (td_scln_inv_state.state == SINGLE_HOLD) {
        unregister_code(KC_ACL0);
    } else if (td_scln_inv_state.state == DOUBLE_HOLD) {
        unregister_code(KC_ACL1);
    }
    td_scln_inv_state.state = 0;
}

// =============================================================================
// 5. RGB Indicators Forward Declarations
// =============================================================================
#ifdef RGBLIGHT_ENABLE
bool leader_is_flashing = false;
uint32_t leader_flash_timer = 0;
bool leader_flash_toggle = false;
void update_rgb_indicators(void);
void update_rgb_indicators_state(layer_state_t state, layer_state_t def_state);
#endif

// =============================================================================
// 6. Keyboard Initialization
// =============================================================================
void eeconfig_init_user(void) {
    default_layer_set(1UL << 1);
    eeconfig_update_default_layer(1UL << 1);
}

void keyboard_post_init_user(void) {
    default_layer_set(1UL << 1);

#ifdef RGBLIGHT_ENABLE
    rgblight_enable_noeeprom();
    update_rgb_indicators();
#endif

    qp_setup_oled();

    // Register Tap Dance actions
    tap_dance_actions[TD_GUI_LEADER] = (tap_dance_action_t)ACTION_TAP_DANCE_FN_ADVANCED(NULL, gui_lead_finished, gui_lead_reset);
    tap_dance_actions[TD_ACL]        = (tap_dance_action_t)ACTION_TAP_DANCE_FN_ADVANCED(NULL, acl_finished, acl_reset);
    tap_dance_actions[TD_BTN]        = (tap_dance_action_t)ACTION_TAP_DANCE_FN_ADVANCED(NULL, btn_finished, btn_reset);
    tap_dance_actions[TD_SCLN_INV]   = (tap_dance_action_t)ACTION_TAP_DANCE_FN_ADVANCED(NULL, scln_inv_finished, scln_inv_reset);
}

// =============================================================================
// 7. Custom State Machine for Space (SP_L1)
// =============================================================================
// - Single tap: KC_SPC
// - Hold: Activates Navigation & Editing (Layer 2)
// - Double-tap and hold: Activates Media & Config (Layer 3)
#define SPACE_HOLD_TIMEOUT 130 // Milliseconds to hold Space before activating Layer 2

enum spc_state {
    SP_IDLE,
    SP_FIRST_PRESS,
    SP_HOLD_L1,
    SP_WAITING_DOUBLE,
    SP_HOLD_L2
};

static enum spc_state current_spc_state = SP_IDLE;
static uint16_t spc_timer = 0;



// =============================================================================
// 9. Matrix Scan User Task
// =============================================================================
void matrix_scan_user(void) {
    // Space dual-role timeout handling
    if (current_spc_state == SP_FIRST_PRESS && timer_elapsed(spc_timer) >= SPACE_HOLD_TIMEOUT) {
        current_spc_state = SP_HOLD_L1;
        layer_on(2);
    } else if (current_spc_state == SP_WAITING_DOUBLE && timer_elapsed(spc_timer) >= SPACE_HOLD_TIMEOUT) {
        current_spc_state = SP_IDLE;
        tap_code(KC_SPC);
    }

#ifdef RGBLIGHT_ENABLE
    // Alternating Red/Blue flashing indicator during active Leader Key sequence
    if (leader_is_flashing) {
        if (timer_elapsed32(leader_flash_timer) > 150) {
            leader_flash_timer = timer_read32();
            leader_flash_toggle = !leader_flash_toggle;
            if (leader_flash_toggle) {
                rgblight_setrgb_at(255, 0, 0, 0); // Left Red
                rgblight_setrgb_at(0, 0, 255, 1); // Right Blue
            } else {
                rgblight_setrgb_at(0, 0, 255, 0); // Left Blue
                rgblight_setrgb_at(255, 0, 0, 1); // Right Red
            }
        }
    }
#endif
}

// =============================================================================
// 10. Key Record Processing
// =============================================================================
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        oled_reset_screensaver();
    }

    // Seed RNG and shuffle OLED icons on the very first physical keystroke
    static bool first_key_done = false;
    if (record->event.pressed && !first_key_done) {
        first_key_done = true;
        srand(timer_read32());
        shuffle_icons();
    }

    // Resolve any pending Space state if another key is pressed while Space is held
    if (record->event.pressed && keycode != SP_L1) {
        if (current_spc_state == SP_FIRST_PRESS) {
            current_spc_state = SP_HOLD_L1;
            layer_on(2);
            keycode = keymap_key_to_keycode(2, record->event.key);
        } else if (current_spc_state == SP_WAITING_DOUBLE) {
            current_spc_state = SP_IDLE;
            tap_code(KC_SPC);
        }
    }

    switch (keycode) {
        // Toggle Default Layer: QWERTY (0) <-> Sturdy (1)
        case STRD_TG:
            if (record->event.pressed) {
                uint8_t def = get_highest_layer(default_layer_state);
                default_layer_set(def == 0 ? (1UL << 1) : (1UL << 0));
                print_status_wpm();
            }
            return false;

        // Dedicated Handedness Setting Keys (Layer 3)
        case EH_LEFT:
            if (record->event.pressed) {
                eeconfig_update_handedness(true);
                mcu_reset();
            }
            return false;

        case EH_RGHT:
            if (record->event.pressed) {
                eeconfig_update_handedness(false);
                mcu_reset();
            }
            return false;

        // Mouse Speed Tuning
        case SPD_TOG:
            if (record->event.pressed) {
                speed_cycle_mode = (speed_cycle_mode + 1) % 2;
            }
            return false;

        case SPD_CYC:
        case SPD_UP:
            if (record->event.pressed) {
                cycle_mouse_speed(true);
            }
            return false;

        case SPD_DN:
            if (record->event.pressed) {
                cycle_mouse_speed(false);
            }
            return false;

        case SCR_TOG:
            if (record->event.pressed) {
                dynamic_scroll_speed = (dynamic_scroll_speed >= 5) ? 1 : (dynamic_scroll_speed + 1);
            }
            return false;

        // Macro String Delay (RAM-only encoder tuning)
        case DLY_UP:
            if (record->event.pressed) {
                if (dynamic_string_delay <= 245) dynamic_string_delay += 10;
                show_dly_setting_screen();
            }
            return false;

        case DLY_DN:
            if (record->event.pressed) {
                if (dynamic_string_delay >= 10) dynamic_string_delay -= 10;
                show_dly_setting_screen();
            }
            return false;

        // OLED Nerd Icon Cycling
        case ICON_UP:
            if (record->event.pressed) {
                cycle_nerd_icon(true);
            }
            return false;

        case ICON_DN:
            if (record->event.pressed) {
                cycle_nerd_icon(false);
            }
            return false;

        case ICN_RND:
            if (record->event.pressed) {
                reshuffle_icons();
            }
            return false;

        // Multi-function Space (Tap=Space, Hold=L2, Double-tap Hold=L3)
        case SP_L1:
            if (record->event.pressed) {
                if (current_spc_state == SP_WAITING_DOUBLE) {
                    current_spc_state = SP_HOLD_L2;
                    layer_on(3);
                } else {
                    current_spc_state = SP_FIRST_PRESS;
                    spc_timer = timer_read();
                }
            } else {
                if (current_spc_state == SP_FIRST_PRESS) {
                    current_spc_state = SP_WAITING_DOUBLE;
                    spc_timer = timer_read();
                } else if (current_spc_state == SP_HOLD_L1) {
                    layer_off(2);
                    current_spc_state = SP_IDLE;
                } else if (current_spc_state == SP_HOLD_L2) {
                    layer_off(3);
                    current_spc_state = SP_IDLE;
                }
            }
            return false;

        // Line Deletion (Shift+Home then Backspace)
        case DEL_LIN:
            if (record->event.pressed) {
                tap_code16(LSFT(KC_HOME));
                tap_code(KC_BSPC);
            }
            return false;

        // VSCode Quick Comment / Uncomment
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

        // Display Timeout Settings
        case SS_T_UP:
            if (record->event.pressed) {
                screensaver_timeout += 1000;
                show_timeout_setting_screen();
            }
            return false;

        case SS_T_DN:
            if (record->event.pressed) {
                if (screensaver_timeout > 1000) screensaver_timeout -= 1000;
                show_timeout_setting_screen();
            }
            return false;

        case OFF_T_UP:
            if (record->event.pressed) {
                screenoff_timeout += 10000;
                show_timeout_setting_screen();
            }
            return false;

        case OFF_T_DN:
            if (record->event.pressed) {
                if (screenoff_timeout > 10000) screenoff_timeout -= 10000;
                show_timeout_setting_screen();
            }
            return false;
    }

    return true;
}

// clang-format off

// =============================================================================
// 11. Keymap Matrix Definitions (Strictly <= 7 chars per entry, column-aligned)
// =============================================================================
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Layout Reference:
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

// Layer 0: Base QWERTY Layout
[0] = LAYOUT(
  KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                      KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS,
  KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,                      KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , TG(4)  ,
  KC_ESC , KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,                      KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT,
  KC_LSFT, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_MUTE,    XXXXXXX, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_RSFT,
                    TD_GUI , KC_LCTL, KC_LALT, SP_L1  , KC_ENT ,    SP_L1  , KC_SPC , KC_RCTL, KC_LALT, KC_RGUI
),

// Layer 1: Sturdy Layout (by Oxey)
[1] = LAYOUT(
  KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                      KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS,
  KC_TAB , KC_V   , KC_M   , KC_L   , KC_C   , KC_P   ,                      KC_X   , KC_F   , KC_O   , KC_U   , KC_J   , TG(4)  ,
  KC_ESC , KC_S   , KC_T   , KC_R   , KC_D   , KC_Y   ,                      KC_DOT , KC_N   , KC_A   , KC_E   , KC_I   , KC_SLSH,
  KC_LSFT, KC_Z   , KC_K   , KC_Q   , KC_G   , KC_W   , KC_MUTE,    XXXXXXX, KC_B   , KC_H   , KC_QUOT, KC_SCLN, KC_COMM, KC_RSFT,
                    TD_GUI , KC_LCTL, KC_LALT, SP_L1  , KC_ENT ,    SP_L1  , KC_SPC , KC_RCTL, KC_LALT, KC_RGUI
),

// Layer 2: Navigation & Editing (Hold Space)
[2] = LAYOUT(
  KC_PGUP, KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  ,                      KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_MINS,
  KC_PGDN, DEL_LIN, DEL_WRD, KC_HOME, KC_END , CT_ENT ,                      KC_Y   , KC_U   , KC_HOME, KC_END , SF_LPR , SF_RPR ,
  KC_UNDS, CT_SLSH, KC_LEFT, KC_UP  , KC_DOWN, KC_RGHT,                      KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, SF_LPR , KC_EQL ,
  KC_LSFT, VS_COMM, VS_UNCO, KC_COMM, KC_BSPC, KC_DEL , KC_MUTE,    XXXXXXX, KC_BSPC, KC_DEL , KC_LBRC, KC_RBRC, KC_EQL , KC_BSLS,
                    TD_GUI , KC_LCTL, KC_LALT, KC_NO  , KC_ENT ,    KC_NO  , KC_SPC , KC_RCTL, KC_LALT, KC_RGUI
),

// Layer 3: Media, Config & Bootmagic (Double-tap Hold Space)
[3] = LAYOUT(
  KC_GRV , KC_1   , EH_LEFT, QK_BOOT, KC_4   , KC_5   ,                      KC_6   , KC_7   , QK_BOOT, EH_RGHT, KC_0   , KC_MINS,
  KC_TAB , KC_F7  , KC_F8  , CT_HOME, CT_END , CS_ENT ,                      KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_BSPC,
  KC_MINS, QK_LEAD, CT_LEFT, KC_D   , KC_F   , CT_RGHT,                      CT_LEFT, KC_J   , KC_K   , CT_RGHT, QK_LEAD, KC_QUOT,
  KC_LSFT, KC_Z   , KC_X   , ICN_RND, ICON_DN, ICON_UP, KC_MUTE,    XXXXXXX, ICON_DN, ICON_UP, ICN_RND, KC_DOT , KC_SLSH, KC_RSFT,
                    TD_GUI , KC_LCTL, KC_LALT, KC_NO  , KC_ENT ,    KC_NO  , MO(2)  , KC_RCTL, KC_LALT, KC_RGUI
),

// Layer 4: Mouse Mode
[4] = LAYOUT(
  KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                      KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS,
  KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,                      SPD_TOG, SCR_TOG, MS_BTN1, MS_BTN2, MS_BTN3, TG(4)  ,
  KC_ESC , KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,                      MS_LEFT, MS_DOWN, MS_UP  , MS_RGHT, INV_ACL, KC_QUOT,
  KC_LSFT, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_MUTE,    XXXXXXX, SCR_TOG, WH_DOWN, WH_UP  , WH_LEFT, WH_RGHT, KC_RSFT,
                    TD_GUI , KC_LCTL, KC_LALT, KC_TRNS, KC_ENT ,    TP_ACL , TP_BTN , KC_ACL2, KC_LALT, SPD_CYC
)
};

// =============================================================================
// 12. Rotary Encoder Mapping
// =============================================================================
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(KC_BRID, KC_BRIU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },   // Base QWERTY: Brightness / Volume
    [1] = { ENCODER_CCW_CW(KC_BRID, KC_BRIU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },   // Sturdy: Brightness / Volume
    [2] = { ENCODER_CCW_CW(SS_T_DN, SS_T_UP), ENCODER_CCW_CW(OFF_T_DN, OFF_T_UP) }, // Nav/Edit: Screensaver / Screen-off Timeout
    [3] = { ENCODER_CCW_CW(DLY_DN,  DLY_UP),  ENCODER_CCW_CW(DLY_DN,  DLY_UP) },  // Media/Config: String Delay Tuning
    [4] = { ENCODER_CCW_CW(SPD_DN,  SPD_UP),  ENCODER_CCW_CW(SPD_DN,  SPD_UP) },  // Mouse Mode: Cursor Speed Tuning
};

// clang-format on

// =============================================================================
// 13. Tapping Term Per Key
// =============================================================================
#ifdef TAPPING_TERM_PER_KEY
uint16_t vial_get_tapping_term(uint16_t keycode, keyrecord_t *record);

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    return vial_get_tapping_term(keycode, record);
}
#endif

// =============================================================================
// 14. RGB Layer Indicators
// =============================================================================
#ifdef RGBLIGHT_ENABLE
void update_rgb_indicators_state(layer_state_t state, layer_state_t def_state) {
    if (leader_is_flashing) {
        return;
    }

    uint8_t active_layer = get_highest_layer(state);
    uint8_t default_layer = get_highest_layer(def_state);

    if (active_layer == 4) {
        // Layer 4: Mouse Mode -> Flashing Red
        rgblight_mode_noeeprom(RGBLIGHT_MODE_ALTERNATING);
        rgblight_sethsv_noeeprom(HSV_RED);
    } else if (active_layer == 3) {
        // Layer 3: Media & Boot -> Twinkle Gold
        rgblight_mode_noeeprom(RGBLIGHT_MODE_TWINKLE);
        rgblight_sethsv_noeeprom(HSV_GOLDENROD);
    } else if (active_layer == 2) {
        // Layer 2: Nav & Editing -> Rainbow Mood
        rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_MOOD);
    } else if (default_layer == 1 || active_layer == 1) {
        // Layer 1: Sturdy Layout -> Breathing Green
        rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING);
        rgblight_sethsv_noeeprom(HSV_GREEN);
    } else {
        // Layer 0: Base QWERTY -> Static Blue
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
        rgblight_sethsv_noeeprom(HSV_BLUE);
    }
}

void update_rgb_indicators(void) {
    update_rgb_indicators_state(layer_state, default_layer_state);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    update_rgb_indicators_state(state, default_layer_state);
    return state;
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    update_rgb_indicators_state(layer_state, state);
    return state;
}
#endif

// =============================================================================
// 15. Leader Key Sequence Handlers
// =============================================================================
void leader_start_user(void) {
    oled_set_leader_state(true);
#ifdef RGBLIGHT_ENABLE
    leader_is_flashing = true;
    leader_flash_timer = timer_read32();
    leader_flash_toggle = true;
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_setrgb_at(255, 0, 0, 0); // Left Red
    rgblight_setrgb_at(0, 0, 255, 1); // Right Blue
#endif
}

void leader_end_user(void) {
    oled_set_leader_state(false);
#ifdef RGBLIGHT_ENABLE
    leader_is_flashing = false;
    update_rgb_indicators();
#endif

    if (leader_sequence_one_key(KC_S)) {
        SEND_STRING_DELAY("bravewater123@", dynamic_string_delay);
    } else if (leader_sequence_one_key(KC_T)) {
        SEND_STRING_DELAY("bravewater123#", dynamic_string_delay);
    } else if (leader_sequence_two_keys(KC_S, KC_S)) {
        SEND_STRING_DELAY("bravewater123@", (dynamic_string_delay > 125) ? 250 : dynamic_string_delay * 2);
    } else if (leader_sequence_one_key(KC_Z)) {
        // Leader + Z: Toggle Default Base Layer (QWERTY <-> Sturdy)
        uint8_t def = get_highest_layer(default_layer_state);
        default_layer_set(def == 0 ? (1UL << 1) : (1UL << 0));
        print_status_wpm();
    }
}
