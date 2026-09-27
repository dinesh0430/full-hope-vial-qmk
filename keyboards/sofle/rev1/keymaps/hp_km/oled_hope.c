/* Quantum Painter OLED Display Logic */
#include QMK_KEYBOARD_H
#include <stdio.h>
#include <stdbool.h>

#ifdef QUANTUM_PAINTER_ENABLE
#include <qp.h>
#include "split_common/transactions.h"
#include "fonts/fool_16.qff.h"
#include "fonts/fool_32.qff.h"
#include "fonts/rubik_48.qff.h"
#include "fonts/nerd_32.qff.h"
#include "gfx/doom_fire.qgf.h"
#include "gfx/wireframe_cube.qgf.h"
#include "gfx/liquid_metaballs.qgf.h"
#include "gfx/game_of_life.qgf.h"
#include "gfx/starfield_warp.qgf.h"
#include "gfx/tornado_vortex.qgf.h"
#include "gfx/hurricane_eye.qgf.h"
#include "gfx/plasma_wave.qgf.h"
#include "gfx/lava_lamp.qgf.h"
#include "gfx/raindrop_ripples.qgf.h"
#include "gfx/wireframe_torus.qgf.h"
#include "gfx/matrix_rain.qgf.h"
#include "gfx/lorenz_attractor.qgf.h"
#include "char_anims.h"

/* Shared state with keymap.c and quantum/mousekey.c */
extern uint8_t dynamic_mouse_speed;
extern uint8_t dynamic_scroll_speed;
extern uint8_t speed_cycle_mode;
extern uint8_t dynamic_string_delay;

/* Function Prototypes */
void print_status_wpm(void);
void oled_reset_screensaver(void);
void shuffle_icons(void);
void reshuffle_icons(void);
void cycle_nerd_icon(bool forward);
void show_timeout_setting_screen(void);
void show_dly_setting_screen(void);
void oled_set_leader_state(bool active);
void qp_setup_oled(void);
void oled_sync_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data);

painter_device_t display;
painter_font_handle_t fool_16_font;
painter_font_handle_t fool_32_font;
painter_font_handle_t rubik_48_font;
painter_font_handle_t nerd_32_font;

#define NUM_SCREENSAVERS 13
painter_image_handle_t screensaver_imgs[NUM_SCREENSAVERS] = {NULL};
deferred_token screensaver_anim_token = INVALID_DEFERRED_TOKEN;
static uint8_t current_screensaver_idx = 0;
static uint32_t screensaver_switch_timer = 0;

static void stop_screensaver_anim_if_running(void) {
    if (screensaver_anim_token != INVALID_DEFERRED_TOKEN) {
        qp_stop_animation(screensaver_anim_token);
        screensaver_anim_token = INVALID_DEFERRED_TOKEN;
    }
}

static void start_screensaver_anim(uint8_t idx) {
    stop_screensaver_anim_if_running();
    if (idx < NUM_SCREENSAVERS && screensaver_imgs[idx] != NULL) {
        qp_clear(display);
        screensaver_anim_token = qp_animate(display, 0, 0, screensaver_imgs[idx]);
    }
}

// Shuffle-deck screensaver picker: every screensaver plays once before any repeats
static uint8_t ss_deck[NUM_SCREENSAVERS];
static uint8_t ss_deck_pos = NUM_SCREENSAVERS; // Start exhausted to trigger initial shuffle

static void shuffle_ss_deck(void) {
    // Initialize deck with indices 0..NUM_SCREENSAVERS-1
    for (uint8_t i = 0; i < NUM_SCREENSAVERS; i++) ss_deck[i] = i;
    // Fisher-Yates shuffle using timer entropy
    for (uint8_t i = NUM_SCREENSAVERS - 1; i > 0; i--) {
        uint32_t h = (timer_read32() * 1664525u + 1013904223u);
        uint8_t j = h % (i + 1);
        uint8_t tmp = ss_deck[i];
        ss_deck[i] = ss_deck[j];
        ss_deck[j] = tmp;
    }
    ss_deck_pos = 0;
}

static uint8_t pick_random_screensaver(void) {
    if (ss_deck_pos >= NUM_SCREENSAVERS) shuffle_ss_deck();
    return ss_deck[ss_deck_pos++];
}

static painter_image_handle_t current_char_anim_img = NULL;
deferred_token char_anim_token = INVALID_DEFERRED_TOKEN;
static uint8_t current_char_anim_idx = 0;
static uint32_t char_anim_switch_timer = 0;

static void stop_char_anim_if_running(void) {
    if (char_anim_token != INVALID_DEFERRED_TOKEN) {
        qp_stop_animation(char_anim_token);
        char_anim_token = INVALID_DEFERRED_TOKEN;
    }
    if (current_char_anim_img != NULL) {
        qp_close_image(current_char_anim_img);
        current_char_anim_img = NULL;
    }
}

static void start_char_anim(uint8_t idx) {
    stop_char_anim_if_running();
    if (idx < NUM_CHAR_ANIMS) {
        current_char_anim_img = qp_load_image_mem(char_anim_list[idx].data);
        if (current_char_anim_img != NULL) {
            // Clear bottom slot only (Y: 71..127)
            qp_rect(display, 0, 71, 31, 127, 0, 0, 0, true);
            uint8_t h = char_anim_list[idx].height;
            uint8_t w = char_anim_list[idx].width;
            int16_t anim_x = (32 - w) / 2;
            if (anim_x < 0) anim_x = 0;
            int16_t anim_y = 71 + (57 - h) / 2;
            if (anim_y < 71) anim_y = 71;
            char_anim_token = qp_animate(display, anim_x, anim_y, current_char_anim_img);
        }
    }
}

#include "nerd_icon_list.inc"

static uint16_t current_icon_idx = 0;
bool is_leader_active = false;
static uint32_t last_activity_timer = 0;
bool is_screensaver_active = false;
bool is_screen_off = false;

static uint16_t shuffled_indices[NUM_ICONS];
static bool is_shuffled = false;
static uint16_t shuffle_deck_pos = 0;

// Display override: 0 = normal (independent pages), 1 = MOU, 2 = LDR
static uint8_t display_override = 0;

uint32_t screensaver_timeout = 3000;
uint32_t screenoff_timeout = 300000;

/* Data struct synced from master to slave via custom split transaction.
 * 
 * IMPORTANT ARCHITECTURE NOTE for RP2040 (Cortex-M0+):
 * 1. __attribute__((packed)) is REQUIRED. Without it, the compiler adds hidden padding bytes to
 *    align the uint32_t fields, which causes a struct size mismatch or unaligned memory access.
 * 2. In `oled_sync_slave_handler`, you MUST use `memcpy` to copy the incoming data into a local
 *    variable before accessing the 32-bit fields. Casting the raw `in_data` byte buffer directly
 *    to this struct pointer causes a hardware Hard Fault (crashing the slave matrix/OLED) 
 *    because the Cortex-M0+ processor does not support unaligned 32-bit read instructions.
 * 3. Max size is strictly 32 bytes (QMK's default RPC_M2S_BUFFER_SIZE).
 */
typedef struct __attribute__((packed)) {
    uint8_t display_override;
    bool is_shuffled;
    uint8_t dynamic_mouse_speed;
    uint8_t dynamic_scroll_speed;
    uint8_t speed_cycle_mode;
    uint32_t screensaver_timeout;
    uint32_t screenoff_timeout;
    uint8_t dynamic_string_delay;
    uint8_t activity_counter;
} oled_sync_data_t;

// Slave-side copy of synced data
static oled_sync_data_t slave_oled_sync = {0, false, 0, 0, 0, 3000, 300000, 50, 0};

static uint16_t get_my_icon_idx(void) {
    if (!is_shuffled) {
        shuffle_icons();
    }
    return shuffled_indices[shuffle_deck_pos % NUM_ICONS];
}


#define NUM_FALLING_ICONS 4
#define FALLING_ICON_SPACING 38  // Tighter spacing so icons flow continuously

typedef struct {
    int16_t y;
    int16_t x;
    uint16_t icon_idx;
} falling_icon_t;

static uint16_t screensaver_deck_pos = 0;
static uint16_t get_next_screensaver_icon(void) {
    if (!is_shuffled) {
        shuffle_icons();
    }
    screensaver_deck_pos++;
    if (screensaver_deck_pos >= NUM_ICONS) {
        shuffle_icons();
        screensaver_deck_pos = 0;
    }
    return shuffled_indices[screensaver_deck_pos % NUM_ICONS];
}

static bool screensaver_initialized = false;

static void print_screensaver(void) {
    static falling_icon_t icons[NUM_FALLING_ICONS];

    if (!screensaver_initialized) {
        for (int i = 0; i < NUM_FALLING_ICONS; i++) {
            icons[i].y = i * 36;
            icons[i].icon_idx = get_next_screensaver_icon();

            const char* icon_str = nerd_icon_list[icons[i].icon_idx];
            int16_t iw = qp_textwidth(nerd_32_font, icon_str);
            icons[i].x = (iw < 32) ? ((32 - iw) / 2) : 0;
        }
        screensaver_initialized = true;
    }

    qp_clear(display);

    for (int i = 0; i < NUM_FALLING_ICONS; i++) {
        const char* icon_str = nerd_icon_list[icons[i].icon_idx];
        qp_drawtext(display, icons[i].x, icons[i].y, nerd_32_font, icon_str);

        icons[i].y += 2; // Fall down by 2 pixels per frame

        // Once it falls completely off the bottom, wrap it around with a new non-repeating icon
        if (icons[i].y > 128) {
            icons[i].y -= (FALLING_ICON_SPACING * NUM_FALLING_ICONS);

            icons[i].icon_idx = get_next_screensaver_icon();
            icon_str = nerd_icon_list[icons[i].icon_idx];
            int16_t iw = qp_textwidth(nerd_32_font, icon_str);
            icons[i].x = (iw < 32) ? ((32 - iw) / 2) : 0;
        }
    }

    qp_flush(display);
}

static uint8_t master_activity_counter = 0;

void oled_reset_screensaver(void) {
    static bool is_seeded = false;
    if (!is_seeded) {
        srand(timer_read32());
        is_seeded = true;
    }
    last_activity_timer = timer_read32();
    if (is_keyboard_master()) {
        master_activity_counter++;
    }
    if (is_screen_off) {
        is_screen_off = false;
        qp_power(display, true);
        qp_clear(display);
        print_status_wpm();
    }
    if (is_screensaver_active) {
        is_screensaver_active = false;
        screensaver_initialized = false;
        stop_screensaver_anim_if_running();
        qp_clear(display);
        print_status_wpm();
    }
}

void shuffle_icons(void) {
    for (uint16_t i = 0; i < NUM_ICONS; i++) {
        shuffled_indices[i] = i;
    }

    // Fisher-Yates Shuffle
    for (uint16_t i = NUM_ICONS - 1; i > 0; i--) {
        uint16_t j = rand() % (i + 1);
        uint16_t temp = shuffled_indices[i];
        shuffled_indices[i] = shuffled_indices[j];
        shuffled_indices[j] = temp;
    }

    is_shuffled = true;
    shuffle_deck_pos = 0;
    print_status_wpm(); // Force redraw to show the newly assigned random starting icon
}

void reshuffle_icons(void) {
    is_shuffled = false;
    shuffle_icons();
}



uint32_t timeout_setting_timer = 0;

void show_timeout_setting_screen(void) {
    if (is_screen_off) {
        is_screen_off = false;
        qp_power(display, true);
    }
    if (is_screensaver_active) {
        is_screensaver_active = false;
        stop_screensaver_anim_if_running();
    }
    stop_char_anim_if_running();
    display_override = 3;
    timeout_setting_timer = timer_read32();
    print_status_wpm();
}

void show_dly_setting_screen(void) {
    if (is_screen_off) {
        is_screen_off = false;
        qp_power(display, true);
    }
    if (is_screensaver_active) {
        is_screensaver_active = false;
        stop_screensaver_anim_if_running();
    }
    stop_char_anim_if_running();
    display_override = 4;
    timeout_setting_timer = timer_read32();
    print_status_wpm();
}

// Returns the active display override for the current side
static uint8_t get_my_display_override(void) {
    if (is_keyboard_master()) {
        return display_override;
    } else {
        return slave_oled_sync.display_override;
    }
}

void print_status_wpm(void) {
    uint8_t my_override = get_my_display_override();

    if (my_override == 2) {
        // LDR (Leader) override — show on both sides
        qp_rect(display, 0, 0, 31, 127, 0, 0, 255, true);

        const char* text_l = "L";
        const char* text_d = "D";
        const char* text_r = "R";

        int16_t lw = qp_textwidth(fool_32_font, text_l);
        int16_t dw = qp_textwidth(fool_32_font, text_d);
        int16_t rw = qp_textwidth(fool_32_font, text_r);

        int16_t lx = (32 - lw) / 2; if (lx < 0) lx = 0;
        int16_t dx = (32 - dw) / 2; if (dx < 0) dx = 0;
        int16_t rx = (32 - rw) / 2; if (rx < 0) rx = 0;

        qp_drawtext_recolor(display, lx, 14, fool_32_font, text_l, 0,0,0, 0,0,255);
        qp_drawtext_recolor(display, dx, 52, fool_32_font, text_d, 0,0,0, 0,0,255);
        qp_drawtext_recolor(display, rx, 90, fool_32_font, text_r, 0,0,0, 0,0,255);

        qp_flush(display);
        return;
    }

    if (my_override == 3) {
        // Timeout Settings Screen
        qp_clear(display);

        // Draw "SS"
        int16_t lw1 = qp_textwidth(fool_16_font, "SS");
        int16_t lx1 = (32 - lw1) / 2; if (lx1 < 0) lx1 = 0;
        qp_drawtext(display, lx1, 5, fool_16_font, "SS");

        char ss_str[16];
        snprintf(ss_str, sizeof(ss_str), "%ds", (int)(screensaver_timeout / 1000));
        int16_t vw1 = qp_textwidth(fool_16_font, ss_str);
        int16_t vx1 = (32 - vw1) / 2; if (vx1 < 0) vx1 = 0;
        qp_drawtext(display, vx1, 22, fool_16_font, ss_str);

        // Draw "OFF"
        int16_t lw2 = qp_textwidth(fool_16_font, "OFF");
        int16_t lx2 = (32 - lw2) / 2; if (lx2 < 0) lx2 = 0;
        qp_drawtext(display, lx2, 65, fool_16_font, "OFF");

        char off_str[16];
        snprintf(off_str, sizeof(off_str), "%ds", (int)(screenoff_timeout / 1000));
        int16_t vw2 = qp_textwidth(fool_16_font, off_str);
        int16_t vx2 = (32 - vw2) / 2; if (vx2 < 0) vx2 = 0;
        qp_drawtext(display, vx2, 82, fool_16_font, off_str);

        qp_flush(display);
        return;
    }

    if (my_override == 4) {
        // Delay Settings Screen
        qp_clear(display);

        // Draw "DLY"
        int16_t lw1 = qp_textwidth(fool_16_font, "DLY");
        int16_t lx1 = (32 - lw1) / 2; if (lx1 < 0) lx1 = 0;
        qp_drawtext(display, lx1, 20, fool_16_font, "DLY");

        char dly_str[16];
        uint8_t current_dly = is_keyboard_master() ? dynamic_string_delay : slave_oled_sync.dynamic_string_delay;
        snprintf(dly_str, sizeof(dly_str), "%d", current_dly);
        int16_t vw1 = qp_textwidth(fool_16_font, dly_str);
        int16_t vx1 = (32 - vw1) / 2; if (vx1 < 0) vx1 = 0;
        qp_drawtext(display, vx1, 60, fool_16_font, dly_str);

        qp_flush(display);
        return;
    }

    // NORMAL SCREEN
    if (is_keyboard_master()) {
        // Redraw only the top portion for the layer indicator (keeps bottom animation intact)
        qp_rect(display, 0, 0, 31, 70, 0, 0, 0, true);
    } else {
        qp_clear(display);
    }

    uint8_t layer = layer_state ? get_highest_layer(layer_state) : get_highest_layer(default_layer_state);

    // Top: Layer in decorative HUD frame
    const char* layer_str;
    if (layer == 0) layer_str = "0";
    else if (layer == 1) layer_str = "S";
    else if (layer == 2) layer_str = "1";
    else if (layer == 3) layer_str = "2";
    else if (layer == 4) layer_str = "4";
    else layer_str = "+";

    int16_t lw = qp_textwidth(rubik_48_font, layer_str);
    int16_t lx = (32 - lw) / 2;
    if (lx < 0) lx = 0;
    qp_drawtext(display, lx, 1, rubik_48_font, layer_str);

    // Base layout indicator: "Strdy" or "Qwrty" in place of the separator line
    bool is_sturdy = (default_layer_state & (1UL << 1)) || (layer == 1);
    const char* layout_tag = is_sturdy ? "Strdy" : "Qwrty";
    int16_t tag_w = qp_textwidth(fool_16_font, layout_tag);
    int16_t tag_x = (32 - tag_w) / 2;
    if (tag_x < 0) tag_x = 0;
    qp_drawtext(display, tag_x, 57, fool_16_font, layout_tag);

    // Bottom area
    if (is_keyboard_master()) {
        // Master: start character animation if not currently running
        if (char_anim_token == INVALID_DEFERRED_TOKEN && !is_screensaver_active && !is_screen_off) {
            start_char_anim(current_char_anim_idx);
        }
    } else {
        // Slave: Cyclable Icon (Y = 83)
        uint16_t actual_idx = get_my_icon_idx();
        const char* icon_str = nerd_icon_list[actual_idx];
        int16_t iw = qp_textwidth(nerd_32_font, icon_str);
        int16_t ix = (32 - iw) / 2;
        if (ix < 0) ix = 0;
        qp_drawtext(display, ix, 83, nerd_32_font, icon_str);
    }

    qp_flush(display);
}

// OLED Leader state updater
void oled_set_leader_state(bool active) {
    is_leader_active = active;
    display_override = active ? 2 : 0;
    if (active) {
        stop_char_anim_if_running();
    }
    print_status_wpm();
}

void cycle_nerd_icon(bool forward) {
    if (!is_shuffled) {
        shuffle_icons();
    }
    if (forward) {
        shuffle_deck_pos++;
        if (shuffle_deck_pos >= NUM_ICONS) {
            shuffle_icons(); // Deck exhausted: fresh non-repeating shuffle
        }
    } else {
        if (shuffle_deck_pos == 0) shuffle_deck_pos = NUM_ICONS - 1;
        else shuffle_deck_pos--;
    }
    current_icon_idx = shuffled_indices[shuffle_deck_pos % NUM_ICONS];
    print_status_wpm();
}



void qp_setup_oled(void) {
    display = qp_sh1106_make_i2c_device(128, 32, 0x3C);
    qp_init(display, QP_ROTATION_90);
    fool_16_font = qp_load_font_mem(font_fool_16);
    fool_32_font = qp_load_font_mem(font_fool_32);
    rubik_48_font = qp_load_font_mem(font_rubik_48);
    nerd_32_font = qp_load_font_mem(font_nerd_32);

    screensaver_imgs[0] = qp_load_image_mem(gfx_doom_fire);
    screensaver_imgs[1] = qp_load_image_mem(gfx_wireframe_cube);
    screensaver_imgs[2] = qp_load_image_mem(gfx_liquid_metaballs);
    screensaver_imgs[3] = qp_load_image_mem(gfx_game_of_life);
    screensaver_imgs[4] = qp_load_image_mem(gfx_starfield_warp);
    screensaver_imgs[5] = qp_load_image_mem(gfx_tornado_vortex);
    screensaver_imgs[6] = qp_load_image_mem(gfx_hurricane_eye);
    screensaver_imgs[7] = qp_load_image_mem(gfx_plasma_wave);
    screensaver_imgs[8] = qp_load_image_mem(gfx_lava_lamp);
    screensaver_imgs[9] = qp_load_image_mem(gfx_raindrop_ripples);
    screensaver_imgs[10] = qp_load_image_mem(gfx_wireframe_torus);
    screensaver_imgs[11] = qp_load_image_mem(gfx_matrix_rain);
    screensaver_imgs[12] = qp_load_image_mem(gfx_lorenz_attractor);

    last_activity_timer = timer_read32();

    // Register custom split transaction for OLED sync (master -> slave)
    transaction_register_rpc(SPLIT_OLED_SYNC, oled_sync_slave_handler);
}

// Slave-side handler: receives OLED sync data from master
void oled_sync_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    oled_sync_data_t data_copy;
    if (in_buflen != sizeof(oled_sync_data_t)) return; // Safety check
    memcpy(&data_copy, in_data, sizeof(oled_sync_data_t));
    
    const oled_sync_data_t* data = &data_copy;
    slave_oled_sync.display_override = data->display_override;
    slave_oled_sync.is_shuffled = data->is_shuffled;
    slave_oled_sync.activity_counter = data->activity_counter;
    
    // Write directly to the global variables on the slave side so they match the master
    dynamic_mouse_speed = data->dynamic_mouse_speed;
    dynamic_scroll_speed = data->dynamic_scroll_speed;
    speed_cycle_mode = data->speed_cycle_mode;
    screensaver_timeout = data->screensaver_timeout;
    screenoff_timeout = data->screenoff_timeout;
    dynamic_string_delay = data->dynamic_string_delay;
}

static void print_mouse_mode_screen(bool inverted) {
    char mode_str[4];
    if (speed_cycle_mode == 0) strcpy(mode_str, "LOW");
    else strcpy(mode_str, "MID");

    char speed_str[8];
    snprintf(speed_str, sizeof(speed_str), "M:%d", dynamic_mouse_speed);

    char scroll_str[8];
    snprintf(scroll_str, sizeof(scroll_str), "S:%d", dynamic_scroll_speed);

    int16_t mw = qp_textwidth(fool_16_font, mode_str);
    int16_t mx = (32 - mw) / 2; if (mx < 0) mx = 0;

    int16_t sw = qp_textwidth(fool_16_font, speed_str);
    int16_t sx = (32 - sw) / 2; if (sx < 0) sx = 0;

    int16_t scw = qp_textwidth(fool_16_font, scroll_str);
    int16_t scx = (32 - scw) / 2; if (scx < 0) scx = 0;

    int16_t lx_m = (32 - qp_textwidth(fool_32_font, "M")) / 2; if (lx_m < 0) lx_m = 0;
    int16_t lx_o = (32 - qp_textwidth(fool_32_font, "O")) / 2; if (lx_o < 0) lx_o = 0;
    int16_t lx_u = (32 - qp_textwidth(fool_32_font, "U")) / 2; if (lx_u < 0) lx_u = 0;

    qp_clear(display);
    if (inverted) {
        // Draw white background only for the 'MOU' area
        qp_rect(display, 0, 0, 31, 79, 0, 0, 255, true);
        qp_drawtext_recolor(display, lx_m, 4, fool_32_font, "M", 0,0,0, 0,0,255);
        qp_drawtext_recolor(display, lx_o, 28, fool_32_font, "O", 0,0,0, 0,0,255);
        qp_drawtext_recolor(display, lx_u, 52, fool_32_font, "U", 0,0,0, 0,0,255);
    } else {
        qp_drawtext(display, lx_m, 4, fool_32_font, "M");
        qp_drawtext(display, lx_o, 28, fool_32_font, "O");
        qp_drawtext(display, lx_u, 52, fool_32_font, "U");
    }

    // Always draw mode, speed, and scroll on black background at the bottom
    qp_drawtext(display, mx, 83, fool_16_font, mode_str);
    qp_drawtext(display, sx, 97, fool_16_font, speed_str);
    qp_drawtext(display, scx, 111, fool_16_font, scroll_str);

    qp_flush(display);
}

void housekeeping_task_user(void) {
    static bool was_mouse_mode = false;

    // --- Slave-side activity detection ---
    // On the slave, process_record_user never fires, so we detect activity here
    if (!is_keyboard_master()) {
        static uint8_t prev_synced_layer = 255;
        static uint8_t prev_synced_override = 255;
        static uint8_t prev_synced_activity = 255;

        uint8_t cur_layer = layer_state ? get_highest_layer(layer_state) : get_highest_layer(default_layer_state);
        uint8_t cur_override = slave_oled_sync.display_override;
        uint8_t cur_activity = slave_oled_sync.activity_counter;

        // If any synced data changed, it means the master is active
        if (cur_layer != prev_synced_layer || cur_override != prev_synced_override || cur_activity != prev_synced_activity) {
            prev_synced_layer = cur_layer;
            prev_synced_override = cur_override;
            prev_synced_activity = cur_activity;
            oled_reset_screensaver();
        }

        // Trigger reshuffle on slave if master shuffled
        if (slave_oled_sync.is_shuffled && !is_shuffled) {
            reshuffle_icons();
        }

        // Also detect local (right-side) keypresses via matrix activity
        if (last_matrix_activity_elapsed() < 1000) {
            oled_reset_screensaver();
        }
    }

    // --- Master: sync OLED data to slave ---
    if (is_keyboard_master()) {
        static uint32_t last_sync = 0;
        if (timer_elapsed32(last_sync) > 50) { // Sync every 50ms
            last_sync = timer_read32();
            oled_sync_data_t sync_data;
            sync_data.display_override = (display_override == 4) ? 0 : display_override;
            sync_data.is_shuffled = is_shuffled;
            sync_data.dynamic_mouse_speed = dynamic_mouse_speed;
            sync_data.dynamic_scroll_speed = dynamic_scroll_speed;
            sync_data.speed_cycle_mode = speed_cycle_mode;
            sync_data.screensaver_timeout = screensaver_timeout;
            sync_data.screenoff_timeout = screenoff_timeout;
            sync_data.dynamic_string_delay = dynamic_string_delay;
            sync_data.activity_counter = master_activity_counter;
            
            transaction_rpc_send(SPLIT_OLED_SYNC, sizeof(sync_data), &sync_data);
        }
    }

    // --- MOU mode: show on both sides (override = 1) ---
    // Skip screensaver/timeout on mouse layer (Layer 4) - user is actively mousing
    uint8_t active_layer = layer_state ? get_highest_layer(layer_state) : get_highest_layer(default_layer_state);
    if (active_layer == 4) {
        last_activity_timer = timer_read32();
        if (is_screen_off) {
            is_screen_off = false;
            qp_power(display, true);
        }
        if (is_screensaver_active) {
            is_screensaver_active = false;
            stop_screensaver_anim_if_running();
        }

        // Set override so both sides show MOU
        if (is_keyboard_master()) {
            display_override = 1;
        }

        static uint32_t mouse_flash_timer = 0;
        static bool mouse_inverted = false;
        static bool last_mouse_inverted = false;
        static uint8_t last_m_spd = 255;
        static uint8_t last_s_spd = 255;
        static uint8_t last_c_mod = 255;

        if (timer_elapsed32(mouse_flash_timer) > 500) { // flash every 500ms
            mouse_flash_timer = timer_read32();
            mouse_inverted = !mouse_inverted;
        }

        // Both sides draw the MOU screen (slave gets override=1 via sync)
        uint8_t my_override = get_my_display_override();
        if (my_override == 1) {
            if (!was_mouse_mode) {
                stop_char_anim_if_running();
            }
            if (!was_mouse_mode || last_mouse_inverted != mouse_inverted || last_m_spd != dynamic_mouse_speed || last_s_spd != dynamic_scroll_speed || last_c_mod != speed_cycle_mode) {
                print_mouse_mode_screen(mouse_inverted);
                last_mouse_inverted = mouse_inverted;
                last_m_spd = dynamic_mouse_speed;
                last_s_spd = dynamic_scroll_speed;
                last_c_mod = speed_cycle_mode;
            }
        }

        was_mouse_mode = true;
        return; // Skip normal UI updates
    }

    if (was_mouse_mode) {
        was_mouse_mode = false;
        if (is_keyboard_master()) {
            display_override = 0;
        }
        print_status_wpm(); // Force an immediate redraw of the normal UI
    }

    static bool was_slave_override = false;
    // --- Slave: MOU override without being on layer 4 locally ---
    // The slave might see override=1 from the synced data even though its local
    // layer_state hasn't changed to 4 yet (sync lag). Handle MOU drawing here too.
    if (!is_keyboard_master() && get_my_display_override() == 1) {
        last_activity_timer = timer_read32();
        if (is_screen_off) {
            is_screen_off = false;
            qp_power(display, true);
        }
        if (is_screensaver_active) {
            is_screensaver_active = false;
        }

        static uint32_t slave_mouse_flash_timer = 0;
        static bool slave_mouse_inverted = false;
        if (timer_elapsed32(slave_mouse_flash_timer) > 500) {
            slave_mouse_flash_timer = timer_read32();
            slave_mouse_inverted = !slave_mouse_inverted;
        }
        
        static bool last_slave_inverted = false;
        static uint8_t last_slv_m_spd = 255;
        static uint8_t last_slv_s_spd = 255;
        static uint8_t last_slv_c_mod = 255;

        if (!was_slave_override || last_slave_inverted != slave_mouse_inverted || last_slv_m_spd != dynamic_mouse_speed || last_slv_s_spd != dynamic_scroll_speed || last_slv_c_mod != speed_cycle_mode) {
            print_mouse_mode_screen(slave_mouse_inverted);
            last_slave_inverted = slave_mouse_inverted;
            last_slv_m_spd = dynamic_mouse_speed;
            last_slv_s_spd = dynamic_scroll_speed;
            last_slv_c_mod = speed_cycle_mode;
            was_slave_override = true;
        }
        return;
    }
    
    // Reset slave override tracker when leaving
    if (!is_keyboard_master() && get_my_display_override() != 1) {
        was_slave_override = false;
    }

    uint32_t elapsed = timer_elapsed32(last_activity_timer);

    // Turn off screen dynamically based on screenoff_timeout (screen saver stays on for screenoff_timeout before sleep)
    if (elapsed > (screensaver_timeout + screenoff_timeout)) {
        if (!is_screen_off) {
            is_screen_off = true;
            is_screensaver_active = false;
            stop_char_anim_if_running();
            stop_screensaver_anim_if_running();
            qp_clear(display);
            qp_flush(display);
            qp_power(display, false);
        }
        return; // Skip drawing when screen is off
    }

    // Screensaver idle check dynamically based on screensaver_timeout
    if (elapsed > screensaver_timeout) {
        if (!is_screensaver_active) {
            is_screensaver_active = true;
            screensaver_initialized = false;
            stop_char_anim_if_running();
            qp_clear(display);
            if (is_keyboard_master()) {
                current_screensaver_idx = pick_random_screensaver();
                screensaver_switch_timer = timer_read32();
                start_screensaver_anim(current_screensaver_idx);
            } else {
                print_screensaver();
            }
        } else {
            if (is_keyboard_master()) {
                if (timer_elapsed32(screensaver_switch_timer) > 5000) {
                    screensaver_switch_timer = timer_read32();
                    current_screensaver_idx = pick_random_screensaver();
                    start_screensaver_anim(current_screensaver_idx);
                }
            } else {
                static uint32_t screensaver_cycle_timer = 0;
                if (timer_elapsed32(screensaver_cycle_timer) > 80) {
                    screensaver_cycle_timer = timer_read32();
                    print_screensaver();
                }
            }
        }
        return; // Skip normal UI updates while screensaver is active
    }

    static uint8_t last_layer = 255;

    // Do not auto-update normal UI if leader screen is active (override = 2)
    uint8_t my_override = get_my_display_override();
    if (my_override == 2) {
        // If we haven't drawn the LDR screen yet, draw it now
        static bool ldr_drawn = false;
        if (!ldr_drawn) {
            print_status_wpm();
            ldr_drawn = true;
        }
        // Reset when override clears
        if (my_override != 2) ldr_drawn = false;
        return;
    }
    // Reset LDR drawn flag when not in LDR mode
    static bool ldr_was_active = false;
    if (my_override != 2 && ldr_was_active) {
        ldr_was_active = false;
        print_status_wpm();
    }
    if (my_override == 2) ldr_was_active = true;

    // Handle Settings Screens (override = 3 or 4)
    if (my_override == 3 || my_override == 4) {
        if (is_keyboard_master() && timer_elapsed32(timeout_setting_timer) > 1000) {
            // Revert back to normal screen
            display_override = 0;
            // Force redraw immediately
            print_status_wpm();
        }
        return;
    }

    uint8_t current_layer = layer_state ? get_highest_layer(layer_state) : get_highest_layer(default_layer_state);

    if (current_layer != last_layer) {
        print_status_wpm();
        last_layer = current_layer;
    }

    if (is_keyboard_master()) {
        // Master: randomly switch to another character animation every 2 seconds
        if (char_anim_token == INVALID_DEFERRED_TOKEN) {
            char_anim_switch_timer = timer_read32();
            current_char_anim_idx = rand() % NUM_CHAR_ANIMS;
            start_char_anim(current_char_anim_idx);
        } else if (timer_elapsed32(char_anim_switch_timer) > 2000) {
            char_anim_switch_timer = timer_read32();
            uint8_t next_idx;
            do {
                next_idx = rand() % NUM_CHAR_ANIMS;
            } while (next_idx == current_char_anim_idx && NUM_CHAR_ANIMS > 1);
            current_char_anim_idx = next_idx;
            start_char_anim(current_char_anim_idx);
        }
    } else {
        // Slave: Auto-cycle icon every 1.5 seconds to prevent burn-in, but ONLY if not actively typing
        static uint32_t cycle_timer = 0;
        if (timer_elapsed32(cycle_timer) > 1500 && timer_elapsed32(last_activity_timer) > 1000) {
            cycle_timer = timer_read32();
            cycle_nerd_icon(true);
        }
    }
}

bool shutdown_user(bool jump_to_bootloader) {
    if (jump_to_bootloader) {
        // Clear the screen to black
        qp_rect(display, 0, 0, 31, 127, 0, 0, 0, true);

        const char* text = is_keyboard_left() ? "LFT" : "RGT";

        int len = 0;
        while (text[len] != '\0') len++;

        // Draw "LFT" or "RGT" vertically (28px fool_32 glyphs)
        // Balanced 128px screen budget:
        // Top margin: 4px | Letters: 3 x 28px = 84px | Inter-letter gaps: 2 x 6px = 12px
        // Gap before BOOT badge: 12px | BOOT badge: 16px (112..127) => Total = 128px
        for (int i = 0; i < len; i++) {
            char c_str[2] = { text[i], '\0' };
            int16_t w = qp_textwidth(fool_32_font, c_str);
            int16_t x = (32 - w) / 2; 
            if (x < 0) x = 0;
            int16_t y = 4 + i * 34; // y = 4, 38, 72 (glyph ends at 100, leaving 12px before 112)
            qp_drawtext_recolor(display, x, y, fool_32_font, c_str, 0, 0, 255, 0, 0, 0);
        }

        // Draw a white rectangle block at the bottom for the BOOT text
        qp_rect(display, 0, 112, 31, 127, 0, 0, 255, true);

        // Draw "BOOT" horizontally in small text at the bottom (Black text on White bg)
        const char* boot_str = "BOOT";
        int16_t boot_w = qp_textwidth(fool_16_font, boot_str);
        int16_t boot_x = (32 - boot_w) / 2;
        if (boot_x < 0) boot_x = 0;
        qp_drawtext_recolor(display, boot_x, 115, fool_16_font, boot_str, 0, 0, 0, 0, 0, 255);

        qp_flush(display);
    }
    return true; // Let QMK continue with the normal shutdown
}
#endif
