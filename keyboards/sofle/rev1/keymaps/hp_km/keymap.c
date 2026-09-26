#include "keycodes.h"
#include QMK_KEYBOARD_H
#ifdef QMK_SETTINGS_ENABLE
#include "qmk_settings.h"
#endif
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
#define LAY_34T (QK_USER + 20)     // Toggle 34-Key Mode
#define CY_FWD  (QK_USER + 21)     // Cyclotab: Alt+Tab forward
#define CY_REV  (QK_USER + 22)     // Cyclotab: Shift+Alt+Tab reverse
#define TD_GUI  TD(0)

// 34-Key Bottom Row Mods (QWERTY)
#define BM_Z    LSFT_T(KC_Z)
#define BM_X    LALT_T(KC_X)
#define BM_C    LCTL_T(KC_C)
#define BM_V    LGUI_T(KC_V)
#define BM_M    RGUI_T(KC_M)
#define BM_COM  RCTL_T(KC_COMM)
#define BM_DOT  LALT_T(KC_DOT)
#define BM_SLS  RSFT_T(KC_SLSH)

// 34-Key Bottom Row Mods (Sturdy)
#define SB_Z    LSFT_T(KC_Z)
#define SB_K    LALT_T(KC_K)
#define SB_Q    LCTL_T(KC_Q)
#define SB_G    LGUI_T(KC_G)
#define SB_H    RGUI_T(KC_H)
#define SB_QOT  RCTL_T(KC_QUOT)
#define SB_SCL  LALT_T(KC_SCLN)
#define SB_COM  RSFT_T(KC_COMM)

// 34-Key Dual-Role Thumbs
#define NAV_BSP TD(TD_NAV_BSP)
#define NUM_ENT LT(9, KC_ENT)
#define SYM_ENT LT(10, KC_ENT)
#define FUN_SPC LT(4, KC_SPC)

// Tap dance & Mod-taps from 34-key
#define TP_CTCS TD(TD_CTL_CS)
#define SFT_UND TD(TD_SFT_UND)
#define SFT_ESC RSFT_T(KC_ESC)
#define CT_UNDO LCTL(KC_Z)
#define CT_CUT  LCTL(KC_X)
#define CT_COPY LCTL(KC_C)
#define CT_PSTE LCTL(KC_V)

// 3. Tap Dance Setup
enum {
    TD_GUI_LEADER = 0,
    TD_ACL,
    TD_BTN,
    TD_SCLN_INV,
    TD_CTL_CS,
    TD_SFT_UND,
    TD_NAV_BSP
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

static td_state_t td_ctl_cs_state  = { .is_press_action = true, .state = 0 };
static td_state_t td_sft_und_state = { .is_press_action = true, .state = 0 };

void ctl_cs_finished(tap_dance_state_t *state, void *user_data) {
    td_ctl_cs_state.state = cur_dance(state);
    switch (td_ctl_cs_state.state) {
        case SINGLE_TAP:
            register_code(KC_ESC);
            break;
        case SINGLE_HOLD:
            register_code(KC_LCTL);
            break;
        case DOUBLE_TAP:
        case DOUBLE_HOLD:
            register_code(KC_LCTL);
            register_code(KC_LSFT);
            break;
    }
}

void ctl_cs_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_ctl_cs_state.state) {
        case SINGLE_TAP:
            unregister_code(KC_ESC);
            break;
        case SINGLE_HOLD:
            unregister_code(KC_LCTL);
            break;
        case DOUBLE_TAP:
        case DOUBLE_HOLD:
            unregister_code(KC_LSFT);
            unregister_code(KC_LCTL);
            break;
    }
    td_ctl_cs_state.state = 0;
}

void sft_und_finished(tap_dance_state_t *state, void *user_data) {
    td_sft_und_state.state = cur_dance(state);
    switch (td_sft_und_state.state) {
        case SINGLE_TAP:
            register_code16(KC_UNDS);
            break;
        case SINGLE_HOLD:
        case DOUBLE_TAP:
        case DOUBLE_HOLD:
            register_code(KC_LSFT);
            break;
    }
}

void sft_und_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_sft_und_state.state) {
        case SINGLE_TAP:
            unregister_code16(KC_UNDS);
            break;
        case SINGLE_HOLD:
        case DOUBLE_TAP:
        case DOUBLE_HOLD:
            unregister_code(KC_LSFT);
            break;
    }
    td_sft_und_state.state = 0;
}

static td_state_t td_nav_bsp_state = { .is_press_action = true, .state = 0 };

void nav_bsp_finished(tap_dance_state_t *state, void *user_data) {
    td_nav_bsp_state.state = cur_dance(state);
    switch (td_nav_bsp_state.state) {
        case SINGLE_TAP:
            register_code(KC_BSPC);
            break;
        case SINGLE_HOLD:
            register_code(KC_LSFT);
            break;
        case DOUBLE_TAP:
            register_code(KC_LCTL);
            register_code(KC_BSPC);
            break;
        case DOUBLE_HOLD:
            layer_on(8);
            break;
    }
}

void nav_bsp_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_nav_bsp_state.state) {
        case SINGLE_TAP:
            unregister_code(KC_BSPC);
            break;
        case SINGLE_HOLD:
            unregister_code(KC_LSFT);
            break;
        case DOUBLE_TAP:
            unregister_code(KC_BSPC);
            unregister_code(KC_LCTL);
            break;
        case DOUBLE_HOLD:
            layer_off(8);
            break;
    }
    td_nav_bsp_state.state = 0;
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

#ifdef RGBLIGHT_ENABLE
    void update_rgb_indicators(void);
    rgblight_enable_noeeprom();
    update_rgb_indicators();
#endif

    qp_setup_oled();
    tap_dance_action_t action = ACTION_TAP_DANCE_FN_ADVANCED(NULL, gui_lead_finished, gui_lead_reset);
    tap_dance_actions[TD_GUI_LEADER] = action;

    tap_dance_action_t action_acl = ACTION_TAP_DANCE_FN_ADVANCED(NULL, acl_finished, acl_reset);
    tap_dance_actions[TD_ACL] = action_acl;

    tap_dance_action_t action_btn = ACTION_TAP_DANCE_FN_ADVANCED(NULL, btn_finished, btn_reset);
    tap_dance_actions[TD_BTN] = action_btn;

    tap_dance_action_t action_scln_inv = ACTION_TAP_DANCE_FN_ADVANCED(NULL, scln_inv_finished, scln_inv_reset);
    tap_dance_actions[TD_SCLN_INV] = action_scln_inv;

    tap_dance_action_t action_ctl_cs = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ctl_cs_finished, ctl_cs_reset);
    tap_dance_actions[TD_CTL_CS] = action_ctl_cs;

    tap_dance_action_t action_sft_und = ACTION_TAP_DANCE_FN_ADVANCED(NULL, sft_und_finished, sft_und_reset);
    tap_dance_actions[TD_SFT_UND] = action_sft_und;

    tap_dance_action_t action_nav_bsp = ACTION_TAP_DANCE_FN_ADVANCED(NULL, nav_bsp_finished, nav_bsp_reset);
    tap_dance_actions[TD_NAV_BSP] = action_nav_bsp;

    // Hijack Vial's first combo in RAM to hardcode J+K -> TG(4)
    extern combo_t key_combos[];
    extern uint16_t key_combos_keys[][5];

#ifdef QMK_SETTINGS_ENABLE
    QS.combo_term = COMBO_TERM;
    QS.tapping_v2 |= (1 << QS_tapping_permissive_hold_bit) |
                     (1 << QS_tapping_chordal_hold_bit);
    QS.flow_tap_term = 150;
    QS.quick_tap_term = 0;
#endif

    key_combos_keys[0][0] = KC_J;
    key_combos_keys[0][1] = KC_K;
    key_combos_keys[0][2] = COMBO_END;
    key_combos[0].keys = key_combos_keys[0];
    key_combos[0].keycode = TG(4);

    key_combos_keys[1][0] = KC_MS_UP;
    key_combos_keys[1][1] = KC_MS_DOWN;
    key_combos_keys[1][2] = COMBO_END;
    key_combos[1].keys = key_combos_keys[1];
    key_combos[1].keycode = TG(4);

    // Left-hand combo to toggle One-Handed Mode (Q + T on Base / P + Y on Layer 5)
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
    key_combos[7].keycode = TG(4);

    // 34-Key Mode Toggle combo: Z + X + C (QWERTY)
    key_combos_keys[8][0] = KC_Z;
    key_combos_keys[8][1] = KC_X;
    key_combos_keys[8][2] = KC_C;
    key_combos_keys[8][3] = COMBO_END;
    key_combos[8].keys = key_combos_keys[8];
    key_combos[8].keycode = LAY_34T;

    // Parity combo: 34-Key Mode Toggle on Sturdy (Z + K + Q)
    key_combos_keys[9][0] = KC_Z;
    key_combos_keys[9][1] = KC_K;
    key_combos_keys[9][2] = KC_Q;
    key_combos_keys[9][3] = COMBO_END;
    key_combos[9].keys = key_combos_keys[9];
    key_combos[9].keycode = LAY_34T;

    // C + V combo -> Backspace (QWERTY)
    key_combos_keys[10][0] = KC_C;
    key_combos_keys[10][1] = KC_V;
    key_combos_keys[10][2] = COMBO_END;
    key_combos[10].keys = key_combos_keys[10];
    key_combos[10].keycode = KC_BSPC;

    // BM_C + BM_V combo -> Backspace (34_qwerty explicit fallback)
    key_combos_keys[11][0] = BM_C;
    key_combos_keys[11][1] = BM_V;
    key_combos_keys[11][2] = COMBO_END;
    key_combos[11].keys = key_combos_keys[11];
    key_combos[11].keycode = KC_BSPC;

    // Q + G combo -> Backspace (Sturdy parity)
    key_combos_keys[12][0] = KC_Q;
    key_combos_keys[12][1] = KC_G;
    key_combos_keys[12][2] = COMBO_END;
    key_combos[12].keys = key_combos_keys[12];
    key_combos[12].keycode = KC_BSPC;

    // SB_Q + SB_G combo -> Backspace (34_sturdy explicit fallback)
    key_combos_keys[13][0] = SB_Q;
    key_combos_keys[13][1] = SB_G;
    key_combos_keys[13][2] = COMBO_END;
    key_combos[13].keys = key_combos_keys[13];
    key_combos[13].keycode = KC_BSPC;

    // Q + W combo -> Escape (QWERTY)
    key_combos_keys[14][0] = KC_Q;
    key_combos_keys[14][1] = KC_W;
    key_combos_keys[14][2] = COMBO_END;
    key_combos[14].keys = key_combos_keys[14];
    key_combos[14].keycode = KC_ESC;

    // V + M combo -> Escape (Sturdy parity)
    key_combos_keys[15][0] = KC_V;
    key_combos_keys[15][1] = KC_M;
    key_combos_keys[15][2] = COMBO_END;
    key_combos[15].keys = key_combos_keys[15];
    key_combos[15].keycode = KC_ESC;

    // M + , combo -> Enter (QWERTY)
    key_combos_keys[16][0] = KC_M;
    key_combos_keys[16][1] = KC_COMM;
    key_combos_keys[16][2] = COMBO_END;
    key_combos[16].keys = key_combos_keys[16];
    key_combos[16].keycode = KC_ENT;

    // BM_M + BM_COM combo -> Enter (34_qwerty explicit fallback)
    key_combos_keys[17][0] = BM_M;
    key_combos_keys[17][1] = BM_COM;
    key_combos_keys[17][2] = COMBO_END;
    key_combos[17].keys = key_combos_keys[17];
    key_combos[17].keycode = KC_ENT;

    // H + ' combo -> Enter (Sturdy parity)
    key_combos_keys[18][0] = KC_H;
    key_combos_keys[18][1] = KC_QUOT;
    key_combos_keys[18][2] = COMBO_END;
    key_combos[18].keys = key_combos_keys[18];
    key_combos[18].keycode = KC_ENT;

    // SB_H + SB_QOT combo -> Enter (34_sturdy explicit fallback)
    key_combos_keys[19][0] = SB_H;
    key_combos_keys[19][1] = SB_QOT;
    key_combos_keys[19][2] = COMBO_END;
    key_combos[19].keys = key_combos_keys[19];
    key_combos[19].keycode = KC_ENT;

    // W + E combo -> Cyclotab Forward (QWERTY)
    key_combos_keys[20][0] = KC_W;
    key_combos_keys[20][1] = KC_E;
    key_combos_keys[20][2] = COMBO_END;
    key_combos[20].keys = key_combos_keys[20];
    key_combos[20].keycode = CY_FWD;

    // M + L combo -> Cyclotab Forward (Sturdy parity)
    key_combos_keys[21][0] = KC_M;
    key_combos_keys[21][1] = KC_L;
    key_combos_keys[21][2] = COMBO_END;
    key_combos[21].keys = key_combos_keys[21];
    key_combos[21].keycode = CY_FWD;

    // Q + E combo -> Cyclotab Reverse (QWERTY)
    key_combos_keys[22][0] = KC_Q;
    key_combos_keys[22][1] = KC_E;
    key_combos_keys[22][2] = COMBO_END;
    key_combos[22].keys = key_combos_keys[22];
    key_combos[22].keycode = CY_REV;

    // V + L combo -> Cyclotab Reverse (Sturdy parity)
    key_combos_keys[23][0] = KC_V;
    key_combos_keys[23][1] = KC_L;
    key_combos_keys[23][2] = COMBO_END;
    key_combos[23].keys = key_combos_keys[23];
    key_combos[23].keycode = CY_REV;
}

// =============================================================================
// Chordal Hold Handedness Map (Opposite Hands Rule)
// =============================================================================
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT(
  '*'    , '*'    , '*'    , '*'    , '*'    , '*'    ,                      '*'    , '*'    , '*'    , '*'    , '*'    , '*'    ,
  '*'    , 'L'    , 'L'    , 'L'    , 'L'    , 'L'    ,                      'R'    , 'R'    , 'R'    , 'R'    , 'R'    , '*'    ,
  '*'    , 'L'    , 'L'    , 'L'    , 'L'    , 'L'    ,                      'R'    , 'R'    , 'R'    , 'R'    , 'R'    , '*'    ,
  '*'    , 'L'    , 'L'    , 'L'    , 'L'    , 'L'    , '*'    ,    '*'    , 'R'    , 'R'    , 'R'    , 'R'    , 'R'    , '*'    ,
                    '*'    , '*'    , '*'    , '*'    , '*'    ,    '*'    , '*'    , '*'    , '*'    , '*'
);

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case BM_Z:
        case BM_SLS:
        case SB_COM:
            // Shift mods: instant 0ms hold on opposite hand press for fast capitalization!
            return true;
        default:
            return false;
    }
}

bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record,
                      uint16_t other_keycode, keyrecord_t *other_record) {
    switch (tap_hold_keycode) {
        case BM_C:
        case SB_Q:
            if (other_keycode == KC_H) {
                return false;
            }
            break;
    }
    return get_chordal_hold_default(tap_hold_record, other_record);
}

// Reference Layer for Combos: Enables combos on 34-key mod-tap layers to resolve plain alphas cleanly
uint8_t combo_ref_from_layer(uint8_t layer) {
    switch (layer) {
        case 6: // 34_qwerty
        case 8: // 34_l1
        case 9: // 34_l2
        case 10: // 34_l3
            return 0; // Evaluate against Base QWERTY
        case 7: // 34_sturdy
            return 1; // Evaluate against Base Sturdy
        default:
            return layer;
    }
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

// 6. Cyclotab (Alt+Tab Swapper) Engine
bool is_cyclotab_active = false;
uint32_t cyclotab_timer = 0;

void cyclotab_step(bool reverse) {
    if (!is_cyclotab_active) {
        is_cyclotab_active = true;
        register_code(KC_LALT);
    }
    if (reverse) {
        register_code(KC_LSFT);
    }
    tap_code(KC_TAB);
    if (reverse) {
        unregister_code(KC_LSFT);
    }
    cyclotab_timer = timer_read32();
}

void cyclotab_stop(void) {
    if (is_cyclotab_active) {
        unregister_code(KC_LALT);
        is_cyclotab_active = false;
    }
}

void matrix_scan_user(void) {
    if (is_cyclotab_active && timer_elapsed32(cyclotab_timer) > 1200) {
        cyclotab_stop();
    }

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
        layer_on(2);
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

#ifdef RGBLIGHT_ENABLE
    extern bool leader_is_flashing;
    extern uint32_t leader_flash_timer;
    extern bool leader_flash_toggle;
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
    // - Baseline: Left-hand keys type normally (Layer 0 / 1).
    // - Hold Space: Shifts to Layer 5 momentarily (right-hand keys Y U I O P...).
    // - Release Space: Returns to Base layer. If no keys were typed, tap Space.
    if (one_handed_mode_enabled) {
        if (keycode == SP_L1 || keycode == KC_SPC) {
            if (record->event.pressed) {
                one_handed_space_held = true;
                one_handed_space_used = false;
                layer_on(5); // Momentarily activate right-hand chars while Space is held
            } else {
                one_handed_space_held = false;
                layer_off(5); // Revert back to normal left-hand chars
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
            // User held Space and typed another key -> activate Layer 2 instantly
            current_spc_state = SP_HOLD_L1;
            layer_on(2);
            keycode = keymap_key_to_keycode(2, record->event.key);
        } else if (current_spc_state == SP_WAITING_DOUBLE) {
            // User tapped Space and then typed another key -> send Space first
            current_spc_state = SP_IDLE;
            tap_code(KC_SPC);
        }
    }


    static bool cyclotab_just_stopped = false;

    // Active Cyclotab navigation mode: E cycles forward, W cycles backward
    if (is_cyclotab_active) {
        if (keycode == KC_E || keycode == KC_L || keycode == CY_FWD) {
            if (record->event.pressed) {
                cyclotab_step(false); // Cycle forward
            }
            return false;
        }
        if (keycode == KC_W || keycode == KC_M || keycode == CY_REV) {
            if (record->event.pressed) {
                cyclotab_step(true); // Cycle backward
            }
            return false;
        }
        // Any other key press cleanly commits the window without typing into the app!
        if (record->event.pressed) {
            cyclotab_stop();
            cyclotab_just_stopped = true;
            return false; // Consume press event
        }
    }

    // Consume the release event of the key used to commit the window
    if (cyclotab_just_stopped) {
        if (!record->event.pressed) {
            cyclotab_just_stopped = false;
            return false; // Consume release event
        }
    }

    switch (keycode) {
        case CY_FWD:
            if (record->event.pressed) {
                cyclotab_step(false);
            }
            return false;

        case CY_REV:
            if (record->event.pressed) {
                cyclotab_step(true);
            }
            return false;

        case LAY_34T:
            if (record->event.pressed) {
                uint8_t def = get_highest_layer(default_layer_state);
                if (def == 0) {
                    default_layer_set(1UL << 6); // 58 QWERTY -> 34 QWERTY
                } else if (def == 1) {
                    default_layer_set(1UL << 7); // 58 Sturdy -> 34 Sturdy
                } else if (def == 6) {
                    default_layer_set(1UL << 0); // 34 QWERTY -> 58 QWERTY
                } else if (def == 7) {
                    default_layer_set(1UL << 1); // 34 Sturdy -> 58 Sturdy
                } else {
                    default_layer_set(1UL << 0);
                }
                extern void print_status_wpm(void);
                print_status_wpm();
            }
            return false;

        case STRD_TG:
            if (record->event.pressed) {
                uint8_t def = get_highest_layer(default_layer_state);
                if (def == 0) {
                    default_layer_set(1UL << 1); // 58 QWERTY -> 58 Sturdy
                } else if (def == 1) {
                    default_layer_set(1UL << 0); // 58 Sturdy -> 58 QWERTY
                } else if (def == 6) {
                    default_layer_set(1UL << 7); // 34 QWERTY -> 34 Sturdy
                } else if (def == 7) {
                    default_layer_set(1UL << 6); // 34 Sturdy -> 34 QWERTY
                } else {
                    default_layer_set(1UL << 1);
                }
                extern void print_status_wpm(void);
                print_status_wpm();
            }
            return false;
        case ONE_HND:
            if (record->event.pressed) {
                one_handed_mode_enabled = !one_handed_mode_enabled;
                if (!one_handed_mode_enabled) {
                    layer_off(5);
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
                    // Double-tap! Activate Layer 3 (Media/Config)
                    current_spc_state = SP_HOLD_L2;
                    layer_on(3);
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
                    layer_off(2);
                    current_spc_state = SP_IDLE;
                } else if (current_spc_state == SP_HOLD_L2) {
                    layer_off(3);
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
  KC_GRV , KC_1   , KC_2   , QK_BOOT, KC_4   , KC_5   ,                      KC_6   , KC_7   , QK_BOOT, KC_9   , KC_0   , KC_MINS,
  KC_TAB , KC_F7  , KC_F8  , CT_HOME, CT_END , CS_ENT ,                      KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_BSPC,
  KC_MINS, QK_LEAD, CT_LEFT, KC_D   , KC_F   , CT_RGHT,                      CT_LEFT, KC_J   , KC_K   , CT_RGHT, KC_SCLN, KC_QUOT,
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
),

// Layer 5: One-Handed Reverse (Mirrors right-hand keys on Left half when Space is held; Esc becomes Backspace)
[5] = LAYOUT(
  KC_MINS, KC_0   , KC_9   , KC_8   , KC_7   , KC_6   ,                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_QUOT, KC_P   , KC_O   , KC_I   , KC_U   , KC_Y   ,                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_BSPC, KC_SCLN, KC_L   , KC_K   , KC_J   , KC_H   ,                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_ENT , KC_SLSH, KC_DOT , KC_COMM, KC_M   , KC_N   , KC_MUTE,    XXXXXXX, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                    TD_GUI , KC_LCTL, KC_LALT, SP_L1  , KC_ENT ,    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
),

#undef  _______
#define _______ KC_NO

// Layer 6: 34_qwerty (34-Key QWERTY + Bottom Row Mods)
[6] = LAYOUT(
  QK_BOOT, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, QK_BOOT,
  _______, KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,                      KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , _______,
  _______, KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,                      KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, _______,
  _______, BM_Z   , BM_X   , BM_C   , BM_V   , KC_B   , _______,    _______, KC_N   , BM_M   , BM_COM , BM_DOT , BM_SLS , _______,
                    _______, _______, _______, NAV_BSP, NUM_ENT,    SYM_ENT, FUN_SPC, _______, _______, _______
),

// Layer 7: 34_sturdy (34-Key Sturdy + Bottom Row Mods)
[7] = LAYOUT(
  QK_BOOT, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, QK_BOOT,
  _______, KC_V   , KC_M   , KC_L   , KC_C   , KC_P   ,                      KC_X   , KC_F   , KC_O   , KC_U   , KC_J   , _______,
  _______, KC_S   , KC_T   , KC_R   , KC_D   , KC_Y   ,                      KC_DOT , KC_N   , KC_A   , KC_E   , KC_I   , _______,
  _______, SB_Z   , SB_K   , SB_Q   , SB_G   , KC_W   , _______,    _______, KC_B   , SB_H   , SB_QOT , SB_SCL , SB_COM , _______,
                    _______, _______, _______, NAV_BSP, NUM_ENT,    SYM_ENT, FUN_SPC, _______, _______, _______
),

// Layer 8: 34_l1 (Navigation, Editing & Word Actions)
[8] = LAYOUT(
  QK_BOOT, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, QK_BOOT,
  _______, KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                      KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , _______,
  _______, TP_CTCS, KC_LEFT, KC_UP  , KC_DOWN, KC_RGHT,                      KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, CW_TOGG, _______,
  _______, SFT_UND, CT_CUT , CT_COPY, CT_PSTE, DEL_WRD, _______,    _______, KC_BSPC, CY_FWD , KC_ENT , KC_DEL , SFT_ESC, _______,
                    _______, _______, _______, CY_REV , _______,    KC_ENT , KC_DEL , _______, _______, _______
),

// Layer 9: 34_l2 (Numbers & Numpad)
[9] = LAYOUT(
  QK_BOOT, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, QK_BOOT,
  _______, KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC,                      KC_SLSH, KC_7   , KC_8   , KC_9   , KC_PLUS, _______,
  _______, KC_CIRC, KC_AMPR, KC_ASTR, KC_EQL , KC_PLUS,                      KC_ASTR, KC_4   , KC_5   , KC_6   , KC_0   , _______,
  _______, KC_TILD, KC_GRV , KC_MINS, KC_UNDS, KC_PIPE, _______,    _______, KC_MINS, KC_1   , KC_2   , KC_3   , KC_DOT , _______,
                    _______, _______, _______, KC_BSPC, _______,    KC_EQL , KC_ENT , _______, _______, _______
),

// Layer 10: 34_l3 (Symbols & Brackets)
[10] = LAYOUT(
  QK_BOOT, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, QK_BOOT,
  _______, KC_LCBR, KC_RCBR, KC_LBRC, KC_RBRC, KC_DLR ,                      KC_GRV , KC_TILD, KC_AMPR, KC_CIRC, KC_EXLM, _______,
  _______, KC_LPRN, KC_RPRN, KC_LABK, KC_RABK, KC_AT  ,                      KC_COLN, KC_SCLN, KC_QUOT, KC_DQUO, KC_QUES, _______,
  _______, KC_PIPE, KC_BSLS, KC_SLSH, KC_PERC, KC_HASH, _______,    _______, KC_UNDS, KC_MINS, KC_PLUS, KC_EQL , KC_ASTR, _______,
                    _______, _______, _______, KC_BSPC, KC_SPC ,    _______, _______, _______, _______, _______
)
};

#undef  _______
#define _______ KC_TRNS


const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0]  = { ENCODER_CCW_CW(KC_BRID, KC_BRIU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1]  = { ENCODER_CCW_CW(KC_BRID, KC_BRIU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [2]  = { ENCODER_CCW_CW(SS_T_DN, SS_T_UP), ENCODER_CCW_CW(OFF_T_DN, OFF_T_UP) },
    [3]  = { ENCODER_CCW_CW(DLY_DN, DLY_UP),   ENCODER_CCW_CW(DLY_DN, DLY_UP) },
    [4]  = { ENCODER_CCW_CW(SPD_DN, SPD_UP),   ENCODER_CCW_CW(SPD_DN, SPD_UP) },
    [5]  = { ENCODER_CCW_CW(KC_BRID, KC_BRIU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [6]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGDN, KC_PGUP) },
    [7]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGDN, KC_PGUP) },
    [8]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGDN, KC_PGUP) },
    [9]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGDN, KC_PGUP) },
    [10] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGDN, KC_PGUP) },
};

// clang-format on

#ifdef TAPPING_TERM_PER_KEY
// Declare the Vial fallback function
uint16_t vial_get_tapping_term(uint16_t keycode, keyrecord_t *record);

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SP_L1:
            return 170;
        case NAV_BSP:
            return 150;
        case NUM_ENT:
        case SYM_ENT:
        case FUN_SPC:
        case BM_Z:
        case BM_X:
        case BM_C:
        case BM_V:
        case BM_M:
        case BM_COM:
        case BM_DOT:
        case BM_SLS:
        case SB_K:
        case SB_Q:
        case SB_G:
        case SB_H:
        case SB_QOT:
        case SB_SCL:
        case SB_COM:
            return 280;
        default:
            return vial_get_tapping_term(keycode, record);
    }
}
#endif

bool leader_is_flashing = false;
uint32_t leader_flash_timer = 0;
bool leader_flash_toggle = false;

void update_rgb_indicators_state(layer_state_t state, layer_state_t def_state) {
#ifdef RGBLIGHT_ENABLE
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
        // Layer 2: Nav & Editing -> Rainbow
        rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_MOOD);
    } else if (active_layer == 5) {
        // Layer 5: One-Handed Reverse -> Orange
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
        rgblight_sethsv_noeeprom(HSV_ORANGE);
    } else if (active_layer == 8) {
        // Layer 8: 34_l1 (34 Nav) -> Rainbow
        rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_MOOD);
    } else if (active_layer == 9) {
        // Layer 9: 34_l2 (34 Num) -> Static Gold
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
        rgblight_sethsv_noeeprom(HSV_GOLD);
    } else if (active_layer == 10) {
        // Layer 10: 34_l3 (34 Sym) -> Twinkle Purple
        rgblight_mode_noeeprom(RGBLIGHT_MODE_TWINKLE);
        rgblight_sethsv_noeeprom(HSV_PURPLE);
    } else if (default_layer == 7 || active_layer == 7) {
        // Layer 7: 34_sturdy -> Breathing Cyan
        rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING);
        rgblight_sethsv_noeeprom(HSV_CYAN);
    } else if (default_layer == 6 || active_layer == 6) {
        // Layer 6: 34_qwerty -> Static Cyan
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
        rgblight_sethsv_noeeprom(HSV_CYAN);
    } else if (default_layer == 1 || active_layer == 1) {
        // Layer 1: Sturdy Layout -> Breathing Green
        rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING);
        rgblight_sethsv_noeeprom(HSV_GREEN);
    } else {
        // Layer 0: Base QWERTY -> Static Blue
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
        rgblight_sethsv_noeeprom(HSV_BLUE);
    }
#endif
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

void leader_start_user(void) {
    extern void oled_set_leader_state(bool active);
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
    extern void oled_set_leader_state(bool active);
    oled_set_leader_state(false);
#ifdef RGBLIGHT_ENABLE
    leader_is_flashing = false;
    update_rgb_indicators();
#endif

    if (leader_sequence_one_key(KC_A)) {
        SEND_STRING_DELAY("bravewater123@", dynamic_string_delay);
    } else if (leader_sequence_one_key(KC_S)) {
        SEND_STRING_DELAY("bravewater123#", dynamic_string_delay);
    } else if (leader_sequence_two_keys(KC_A, KC_A)) {
        // Example: double key uses slower delay, or just use same dynamic delay
        SEND_STRING_DELAY("bravewater123@", (dynamic_string_delay > 125) ? 250 : dynamic_string_delay * 2);
    }
}

