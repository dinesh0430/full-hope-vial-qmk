#include "keycodes.h"
#include QMK_KEYBOARD_H
#ifdef QMK_SETTINGS
#    include "quantum/qmk_settings.h"
#endif

// =============================================================================
// 1. Aliases (strictly <= 7 characters for matrix alignment)
// =============================================================================
// Bottom Row Mods (Pinky -> Index: Shift, Alt, Ctrl, GUI)
#define BM_Z    LSFT_T(KC_Z)
#define BM_X    LALT_T(KC_X)
#define BM_C    LCTL_T(KC_C)
#define BM_V    LGUI_T(KC_V)

#define BM_M    RGUI_T(KC_M)
#define BM_COM  RCTL_T(KC_COMM)
#define BM_DOT  RALT_T(KC_DOT)
#define BM_SLS  RSFT_T(KC_SLSH)

// 34-Key Dual-Role Thumbs (Tap / Hold Layer-Tap)
#define NAV_BSP LT(1, KC_BSPC)
#define NUM_ENT LT(2, KC_ENT)
#define SYM_ENT LT(3, KC_ENT)
#define FUN_SPC LT(4, KC_SPC)
#define TP_CTCS TD(TD_CTL_CS)
#define SFT_UND TD(TD_SFT_UND)
#define SFT_ESC RSFT_T(KC_ESC)

// Navigation & Shortcut Aliases
#define DEL_WRD LCTL(KC_BSPC)
#define CT_LEFT LCTL(KC_LEFT)
#define CT_RGHT LCTL(KC_RIGHT)
#define CT_ENT  LCTL(KC_ENT)
#define CS_ENT  LCTL(LSFT(KC_ENT))
#define CT_SLSH LCTL(KC_SLSH)
#define CT_HOME LCTL(KC_HOME)
#define CT_END  LCTL(KC_END)
#define CT_UNDO LCTL(KC_Z)
#define CT_CUT  LCTL(KC_X)
#define CT_COPY LCTL(KC_C)
#define CT_PSTE LCTL(KC_V)

// Mouse Key Aliases
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

// Custom Keycodes
#define DEL_LIN (QK_USER + 0)
#define VS_COMM (QK_USER + 1)
#define VS_UNCO (QK_USER + 2)

// =============================================================================
// 2. Chordal Hold Handedness Map (Opposite Hands Rule)
// =============================================================================
// 'L' = Left Hand Alpha, 'R' = Right Hand Alpha, '*' = Thumb / Key Hand Agnostic
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT(
  '*'    , '*'    , '*'    , '*'    , '*'    , '*'    ,                      '*'    , '*'    , '*'    , '*'    , '*'    , '*'    ,
  '*'    , 'L'    , 'L'    , 'L'    , 'L'    , 'L'    ,                      'R'    , 'R'    , 'R'    , 'R'    , 'R'    , '*'    ,
  '*'    , 'L'    , 'L'    , 'L'    , 'L'    , 'L'    ,                      'R'    , 'R'    , 'R'    , 'R'    , 'R'    , '*'    ,
  '*'    , 'L'    , 'L'    , 'L'    , 'L'    , 'L'    , '*'    ,    '*'    , 'R'    , 'R'    , 'R'    , 'R'    , 'R'    , '*'    ,
                    '*'    , '*'    , '*'    , '*'    , '*'    ,    '*'    , '*'    , '*'    , '*'    , '*'
);

// =============================================================================
// 3. Hardware Initialization & Tap Dance Setup
// =============================================================================
bool is_keyboard_left_impl(void) {
#ifdef IS_LEFT_SIDE
    return true;
#else
    return false;
#endif
}

enum {
    TD_CTL_CS = 0,
    TD_SFT_UND,
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

static td_state_t td_ctl_cs_state  = { .is_press_action = true, .state = 0 };
static td_state_t td_sft_und_state = { .is_press_action = true, .state = 0 };

static uint8_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (!state->pressed) return DOUBLE_TAP;
        else return DOUBLE_HOLD;
    }
    return 0;
}

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
            tap_code16(LCTL(KC_Z));
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
            break;
        case SINGLE_HOLD:
        case DOUBLE_TAP:
        case DOUBLE_HOLD:
            unregister_code(KC_LSFT);
            break;
    }
    td_sft_und_state.state = 0;
}

void keyboard_post_init_user(void) {
    // Inject Tap Dance actions into Vial's RAM table
    tap_dance_action_t action = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ctl_cs_finished, ctl_cs_reset);
    tap_dance_actions[TD_CTL_CS] = action;

    tap_dance_action_t und_action = ACTION_TAP_DANCE_FN_ADVANCED(NULL, sft_und_finished, sft_und_reset);
    tap_dance_actions[TD_SFT_UND] = und_action;

#ifdef QMK_SETTINGS
    // =========================================================================
    // Dynamic QMK Settings / Vial EEPROM Overrides
    // =========================================================================
    // Force runtime settings to timeless HRM / BRM standards:
    //
    // 1. QS_tapping_permissive_hold_bit:
    //    Guarantees mod-tap settles as hold when nested/tapped alongside another key.
    // 2. QS_tapping_chordal_hold_bit:
    //    Enforces the handedness map so same-hand rolls never trigger mods.
    // 3. Timing parameters:
    //    TAPPING_TERM = 280ms, FLOW_TAP_TERM = 150ms, QUICK_TAP_TERM = 0ms.
    // =========================================================================
    QS.tapping_v2 |= (1 << QS_tapping_permissive_hold_bit) |
                     (1 << QS_tapping_chordal_hold_bit);
    QS.tapping_term = 280;
    QS.flow_tap_term = 150;
    QS.quick_tap_term = 0;
#endif
}

// =============================================================================
// Per-Key Hold on Other Key Press & Chordal Hold Exceptions
// =============================================================================
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case BM_Z:
        case BM_SLS:
            // Shift mods: instant 0ms hold on opposite hand press for fast capitalization!
            return true;
        default:
            // Ctrl, Alt, GUI (BM_C, BM_X, BM_V, BM_M, BM_COM, BM_DOT) and Layer-Taps:
            // Do NOT trigger instant hold on other key press!
            // Prevents roll misfires (like 'ch' triggering Ctrl+H).
            // Deliberate hotkeys use PERMISSIVE_HOLD and TAPPING_TERM (280ms).
            return false;
    }
}

bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record,
                      uint16_t other_keycode, keyrecord_t *other_record) {
    // Specific bigram exceptions that should NEVER trigger modifiers during typing rolls:
    switch (tap_hold_keycode) {
        case BM_C:
            // Fast roll 'ch' -> always settle as plain letters (c, h)
            if (other_keycode == KC_H) {
                return false;
            }
            break;
    }
    return get_chordal_hold_default(tap_hold_record, other_record);
}



// =============================================================================
// 4. Custom Key Processing
// =============================================================================
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DEL_LIN:
            if (record->event.pressed) {
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
    }
    return true;
}

// =============================================================================
// 5. 34-Key Keymaps Matrix (Strictly 7 Chars Per Keycode)
// =============================================================================
// clang-format off

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/*
 * Layer 0: Base 34-Key (QWERTY + Bottom Row Mods)
 * 3x5 Alphas per hand + 2 Primary Thumbs per hand (all unused outer keys _______)
 */
[0] = LAYOUT(
  QK_BOOT, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, QK_BOOT,
  _______, KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,                      KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , _______,
  _______, KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,                      KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, _______,
  _______, BM_Z   , BM_X   , BM_C   , BM_V   , KC_B   , _______,    _______, KC_N   , BM_M   , BM_COM , BM_DOT , BM_SLS , _______,
                    _______, _______, _______, NAV_BSP, NUM_ENT,    SYM_ENT, FUN_SPC, _______, _______, _______
),

/*
 * Layer 1: Navigation, Editing & Word Actions
 */
[1] = LAYOUT(
  QK_BOOT, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, QK_BOOT,
  _______, KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                      KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , _______,
  _______, TP_CTCS, KC_LEFT, KC_UP  , KC_DOWN, KC_RGHT,                      KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, CW_TOGG, _______,
  _______, SFT_UND, CT_CUT , CT_COPY, CT_PSTE, DEL_WRD, _______,    _______, KC_BSPC, KC_TAB , KC_ENT , KC_DEL , SFT_ESC, _______,
                    _______, _______, _______, _______, _______,    KC_ENT , KC_DEL , _______, _______, _______
),

/*
 * Layer 2: Numbers & Clean Numpad
 */
[2] = LAYOUT(
  QK_BOOT, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, QK_BOOT,
  _______, KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC,                      KC_SLSH, KC_7   , KC_8   , KC_9   , KC_PLUS, _______,
  _______, KC_CIRC, KC_AMPR, KC_ASTR, KC_EQL , KC_PLUS,                      KC_ASTR, KC_4   , KC_5   , KC_6   , KC_0   , _______,
  _______, KC_TILD, KC_GRV , KC_MINS, KC_UNDS, KC_PIPE, _______,    _______, KC_MINS, KC_1   , KC_2   , KC_3   , KC_DOT , _______,
                    _______, _______, _______, KC_BSPC, _______,    KC_EQL , KC_ENT , _______, _______, _______
),

/*
 * Layer 3: Symbols & Brackets (Optimized for Programming)
 */
[3] = LAYOUT(
  QK_BOOT, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, QK_BOOT,
  _______, KC_LCBR, KC_RCBR, KC_LBRC, KC_RBRC, KC_DLR ,                      KC_GRV , KC_TILD, KC_AMPR, KC_CIRC, KC_EXLM, _______,
  _______, KC_LPRN, KC_RPRN, KC_LABK, KC_RABK, KC_AT  ,                      KC_COLN, KC_SCLN, KC_QUOT, KC_DQUO, KC_QUES, _______,
  _______, KC_PIPE, KC_BSLS, KC_SLSH, KC_PERC, KC_HASH, _______,    _______, KC_UNDS, KC_MINS, KC_PLUS, KC_EQL , KC_ASTR, _______,
                    _______, _______, _______, KC_BSPC, KC_SPC ,    _______, _______, _______, _______, _______
),

/*
 * Layer 4: Function Keys, Mouse Control, Media & Bootloader
 */
[4] = LAYOUT(
  QK_BOOT, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, QK_BOOT,
  _______, KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  ,                      KC_MPRV, KC_MNXT, KC_MPLY, KC_VOLU, QK_BOOT, _______,
  _______, KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 ,                      MS_LEFT, MS_DOWN, MS_UP  , MS_RGHT, KC_VOLD, _______,
  _______, KC_F11 , KC_F12 , KC_PSCR, KC_SCRL, KC_PAUS, _______,    _______, WH_LEFT, WH_DOWN, WH_UP  , WH_RGHT, KC_MUTE, _______,
                    _______, _______, _______, MS_BTN1, MS_BTN2,    MS_BTN3, _______, _______, _______, _______
)

};

// Rotary Encoders: Left = Volume, Right = Navigation / Scrolling
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGDN, KC_PGUP) },
    [1] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGDN, KC_PGUP) },
    [2] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGDN, KC_PGUP) },
    [3] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGDN, KC_PGUP) },
    [4] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGDN, KC_PGUP) },
};

// clang-format on
