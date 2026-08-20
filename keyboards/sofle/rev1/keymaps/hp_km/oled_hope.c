/* Quantum Painter OLED Display Logic */
#include QMK_KEYBOARD_H
#include <stdio.h>
#include <stdbool.h>

#ifdef QUANTUM_PAINTER_ENABLE
#include <qp.h>
#include "split_common/transactions.h"
#include "fonts/rubik_24.qff.h"
#include "fonts/rubik_48.qff.h"
#include "fonts/nerd_32.qff.h"
#include "fonts/modernpixels_24.qff.h"
#include "fonts/modernpixels_12.qff.h"
#include "fonts/sfmono_16.qff.h"
#include "fonts/rubik_16.qff.h"

painter_device_t display;
painter_font_handle_t rubik_24_font;
painter_font_handle_t rubik_48_font;
painter_font_handle_t nerd_32_font;
painter_font_handle_t modernpixels_24_font;
painter_font_handle_t modernpixels_12_font;
painter_font_handle_t sfmono_16_font;
painter_font_handle_t rubik_16_font;

static const char *nerd_icon_list[] = {
    "\U000F0973", // 󰥳
    "\U0000F1FA", // 
    "\U000F05C0", // 󰗀
    "\U000F0582", // 󰖂
    "\U000F0721", // 󰜡
    "\U000F0519", // 󰔙
    "\U000F03D2", // 󰏒
    "\U000F1105", // 󱄅
    "\U000F02A2", // 󰊢
    "\U000F0684", // 󰚄
    "\U0000EE82", // 
    "\U0000EE7A", // 
    "\U0000ED43", // 
    "\U0000EFA7", // 
    "\U0000F4BC", // 
    "\U0000F428", // 
    "\U000F0346", // 󰍆
    "\U000F0303", // 󰌃
    "\U000F02DF", // 󰋟
    "\U000F124F", // 󱉏
    "\U000F15C6", // 󱗆
    "\U000F1183", // 󱆃
    "\U000F08C7", // 󰣇
    "\U0000F1E9", // 
    "\U0000EF6D", // 
    "\U0000F06C", // 
    "\U0000EF56", // 
    "\U0000F2D6", // 
    "\U0000F282", // 
    "\U0000F0E7", // 
    "\U0000EE99", // 
    "\U0000E782", // 
    "\U0000E7C5", // 
    "\U0000E893", // 
    "\U0000E6A4", // 
    "\U0000E620", // 
    "\U0000E007", // 
    "\U000F0451", // 󰑑
    "\U000F0D7C", // 󰵼
    "\U000F1510", // 󱔐
    "\U000F0D72", // 󰵲
    "\U0000E823", // 
    "\U0000E806", // 
    "\U0000E7FA", // 
    "\U0000E66C", // 
    "\U0000E666", // 
    "\U0000F490", // 
    "\U000F0CD7", // 󰳗
    "\U000F111B", // 󱄛
    "\U000F0A08", // 󰨈
    "\U000F0BAA", // 󰮪
    "\U000F01E4", // 󰇤
    "\U000F0816", // 󰠖
    "\U0000E214", // 
    "\U0000E26E", // 
    "\U0000EF10", // 
    "\U0000EEB0", // 
    "\U0000F135", // 
    "\U0000ED1B", // 
    "\U0000F1AA", // 
    "\U0000F296", // 
    "\U0000F299", // 
    "\U0000F1A9", // 
    "\U0000EEF8", // 
    "\U0000E894", // 
    "\U0000E80D", // 
    "\U000F1A82", // 󱪂
    "\U000F1545", // 󱕅
    "\U000F0421", // 󰐡
    "\U000F117E", // 󱅾
    "\U000F08E0", // 󰣠
    "\U0000EE83", // 
    "\U0000F1DF", // 
    "\U0000EEC9", // 
    "\U0000EDD5", // 
    "\U0000F17B", // 
    "\U0000E8DA", // 
    "\U0000E8D9", // 
    "\U0000E89F", // 
    "\U0000E87F", // 
    "\U0000E873", // 
    "\U0000E86E", // 
    "\U0000E7A1", // 
    "\U0000E872", // 
    "\U0000E867", // 
    "\U0000E7A5", // 
    "\U0000E82A", // 
    "\U0000E7D2", // 
    "\U0000E6B5", // 
    "\U0000EB73", // 
    "\U0000E661", // 
    "\U000F137F", // 󱍿
    "\U000F0967", // 󰥧
    "\U000F0204", // 󰈄
    "\U000F086A", // 󰡪
    "\U000F0674", // 󰙴
    "\U0000E284", // 
    "\U0000EDA7", // 
    "\U0000EF3E", // 
    "\U0000EE28", // 
    "\U0000F2D5", // 
    "\U0000F069", // 
    "\U0000E805", // 
    "\U0000E7F4", // 
    "\U0000E7D7", // 
    "\U0000E716", // 
    "\U0000EC10", // 
    "\U0000F4A1", // 
    "\U000F0064", // 󰁤
    "\U0000F337", // 
    "\U0000EF35", // 
    "\U0000F209", // 
    "\U0000E8A5", // 
    "\U0000E825", // 
    "\U0000E801", // 
    "\U0000E79F", // 
    "\U0000E752", // 
    "\U0000E701", // 
    "\U0000E370", // 
    "\U0000F366", // 
    "\U0000E28E", // 
    "\U0000ED28", // 
    "\U0000F230", // 
    "\U0000E72A", // 
    "\U0000F344", // 
    "\U0000F0A3", // 
    "\U0000F31D", // 
    "\U0000ED24", // 
    "\U000F0CD1", // 󰳑
    "\U000F047A", // 󰑺
    "\U000F04C8", // 󰓈
    "\U0000F2DC", // 
    "\U000F1950", // 󱥐
    "\U000F0907", // 󰤇
    "\U0000EEE6", // 
    "\U0000F197", // 
    "\U0000EF50", // 
    "\U0000F4F5", // 
    "\U000F0EA4", // 󰺤
    "\U000F156B", // 󱕫
    "\U0000F51B", // 
    "\U000F0787", // 󰞇
    "\U000F0BC9", // 󰯉
    "\U0000E721", // 
    "\U000F15C3", // 󱗃
    "\U000F0717", // 󰜗
    "\U000F0DF1", // 󰷱
    "\U000F0B62", // 󰭢
    "\U0000EDEC", // 
    "\U0000EFA0", // 
    "\U000F0383", // 󰎃
    "\U000F1303", // 󱌃
    "\U0000EED5", // 
    "\U000F0CED", // 󰳭
    "\U000F02C4", // 󰋄
    "\U000F0771", // 󰝱
    "\U000F07E1", // 󰟡
    "\U0000EDA0", // 
    "\U0000F27F", // 
    "\U000F0C93", // 󰲓
    "\U000F0B90", // 󰮐
    "\U000F0857", // 󰡗
    "\U000F0433", // 󰐳
    "\U000F0A11", // 󰨑
    "\U000F08B5", // 󰢵
    "\U000F0B84", // 󰮄
    "\U000F19A5", // 󱦥
    "\U0000EEF5", // 
    "\U000F0820", // 󰠠
    "\U000F15FC", // 󱗼
    "\U000F15FF", // 󱗿
    "\U000F15FE", // 󱗾
    "\U000F1617", // 󱘗
    "\U000F18A1", // 󱢡
    "\U000F08D1", // 󰣑
    "\U0000F25B", // 
    "\U000F0A50", // 󰩐
    "\U000F0A51", // 󰩑
};

#define NUM_ICONS (sizeof(nerd_icon_list) / sizeof(nerd_icon_list[0]))



static uint8_t current_icon_idx = 0;
bool is_leader_active = false;
static uint32_t last_activity_timer = 0;
bool is_screensaver_active = false;
bool is_screen_off = false;

static uint8_t shuffled_indices[NUM_ICONS];
static bool is_shuffled = false;

// Display override: 0 = normal (independent pages), 1 = MOU, 2 = LDR
static uint8_t display_override = 0;

uint32_t screensaver_timeout = 3000;
uint32_t screenoff_timeout = 30000;

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
} oled_sync_data_t;

// Slave-side copy of synced data
static oled_sync_data_t slave_oled_sync = {0, false, 0, 0, 0, 3000, 30000, 50};

static void print_status_wpm(void); // Forward declaration to fix compiler error
void oled_sync_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data); // Forward declaration

static uint8_t get_my_icon_idx(void) {
    return is_shuffled ? shuffled_indices[current_icon_idx] : current_icon_idx;
}


#define NUM_FALLING_ICONS 4
#define FALLING_ICON_SPACING 40  // Tighter spacing so more icons fit on screen

typedef struct {
    int16_t y;
    int16_t x;
    uint8_t icon_idx;
} falling_icon_t;

static void print_screensaver(void) {
    static falling_icon_t icons[NUM_FALLING_ICONS];
    static bool initialized = false;

    if (!initialized) {
        for (int i = 0; i < NUM_FALLING_ICONS; i++) {
            // Space them 64px apart so both are visible on the 128px screen
            icons[i].y = -32 - (i * FALLING_ICON_SPACING);
            icons[i].icon_idx = rand() % NUM_ICONS;

            const char* icon_str = nerd_icon_list[icons[i].icon_idx];
            int16_t iw = qp_textwidth(nerd_32_font, icon_str);
            icons[i].x = (iw < 32) ? ((32 - iw) / 2) : 0;
        }
        initialized = true;
    }

    qp_clear(display);

    for (int i = 0; i < NUM_FALLING_ICONS; i++) {
        const char* icon_str = nerd_icon_list[icons[i].icon_idx];
        qp_drawtext(display, icons[i].x, icons[i].y, nerd_32_font, icon_str);

        icons[i].y += 2; // Fall down by 2 pixels per frame

        // Once it falls completely off the bottom, wrap it around to the top
        if (icons[i].y > 128) {
            icons[i].y -= (FALLING_ICON_SPACING * NUM_FALLING_ICONS);

            icons[i].icon_idx = rand() % NUM_ICONS;
            icon_str = nerd_icon_list[icons[i].icon_idx];
            int16_t iw = qp_textwidth(nerd_32_font, icon_str);
            icons[i].x = (iw < 32) ? ((32 - iw) / 2) : 0;
        }
    }

    qp_flush(display);
}

void oled_reset_screensaver(void) {
    static bool is_seeded = false;
    if (!is_seeded) {
        srand(timer_read32());
        is_seeded = true;
    }
    last_activity_timer = timer_read32();
    if (is_screen_off) {
        is_screen_off = false;
        qp_power(display, true);
    }
    if (is_screensaver_active) {
        is_screensaver_active = false;
        print_status_wpm();
    }
}

void shuffle_icons(void) {
    if (is_shuffled) return;

    for (uint16_t i = 0; i < NUM_ICONS; i++) {
        shuffled_indices[i] = i;
    }

    // Fisher-Yates Shuffle
    for (uint16_t i = NUM_ICONS - 1; i > 0; i--) {
        uint16_t j = rand() % (i + 1);
        uint8_t temp = shuffled_indices[i];
        shuffled_indices[i] = shuffled_indices[j];
        shuffled_indices[j] = temp;
    }

    is_shuffled = true;
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
    }
    display_override = 3;
    timeout_setting_timer = timer_read32();
    extern void print_status_wpm(void);
    print_status_wpm();
}

void show_dly_setting_screen(void) {
    if (is_screen_off) {
        is_screen_off = false;
        qp_power(display, true);
    }
    if (is_screensaver_active) {
        is_screensaver_active = false;
    }
    display_override = 4;
    timeout_setting_timer = timer_read32();
    extern void print_status_wpm(void);
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

static void print_status_wpm(void) {
    uint8_t my_override = get_my_display_override();

    if (my_override == 2) {
        // LDR (Leader) override — show on both sides
        qp_rect(display, 0, 0, 31, 127, 0, 0, 255, true);

        const char* text_l = "L";
        const char* text_d = "D";
        const char* text_r = "R";

        int16_t lw = qp_textwidth(rubik_24_font, text_l);
        int16_t dw = qp_textwidth(rubik_24_font, text_d);
        int16_t rw = qp_textwidth(rubik_24_font, text_r);

        int16_t lx = (32 - lw) / 2; if (lx < 0) lx = 0;
        int16_t dx = (32 - dw) / 2; if (dx < 0) dx = 0;
        int16_t rx = (32 - rw) / 2; if (rx < 0) rx = 0;

        qp_drawtext_recolor(display, lx, 14, rubik_24_font, text_l, 0,0,0, 0,0,255);
        qp_drawtext_recolor(display, dx, 52, rubik_24_font, text_d, 0,0,0, 0,0,255);
        qp_drawtext_recolor(display, rx, 90, rubik_24_font, text_r, 0,0,0, 0,0,255);

        qp_flush(display);
        return;
    }

    if (my_override == 3) {
        // Timeout Settings Screen
        qp_clear(display);

        // Draw "SS"
        int16_t lw1 = qp_textwidth(sfmono_16_font, "SS");
        int16_t lx1 = (32 - lw1) / 2; if (lx1 < 0) lx1 = 0;
        qp_drawtext(display, lx1, 5, sfmono_16_font, "SS");

        char ss_str[16];
        snprintf(ss_str, sizeof(ss_str), "%ds", (int)(screensaver_timeout / 1000));
        int16_t vw1 = qp_textwidth(sfmono_16_font, ss_str);
        int16_t vx1 = (32 - vw1) / 2; if (vx1 < 0) vx1 = 0;
        qp_drawtext(display, vx1, 22, sfmono_16_font, ss_str);

        // Draw "OFF"
        int16_t lw2 = qp_textwidth(sfmono_16_font, "OFF");
        int16_t lx2 = (32 - lw2) / 2; if (lx2 < 0) lx2 = 0;
        qp_drawtext(display, lx2, 65, sfmono_16_font, "OFF");

        char off_str[16];
        snprintf(off_str, sizeof(off_str), "%ds", (int)(screenoff_timeout / 1000));
        int16_t vw2 = qp_textwidth(sfmono_16_font, off_str);
        int16_t vx2 = (32 - vw2) / 2; if (vx2 < 0) vx2 = 0;
        qp_drawtext(display, vx2, 82, sfmono_16_font, off_str);

        qp_flush(display);
        return;
    }

    if (my_override == 4) {
        // Delay Settings Screen
        qp_clear(display);

        // Draw "DLY"
        int16_t lw1 = qp_textwidth(rubik_16_font, "DLY");
        int16_t lx1 = (32 - lw1) / 2; if (lx1 < 0) lx1 = 0;
        qp_drawtext(display, lx1, 20, rubik_16_font, "DLY");

        char dly_str[16];
        extern uint8_t dynamic_string_delay;
        uint8_t current_dly = is_keyboard_master() ? dynamic_string_delay : slave_oled_sync.dynamic_string_delay;
        snprintf(dly_str, sizeof(dly_str), "%d", current_dly);
        int16_t vw1 = qp_textwidth(rubik_16_font, dly_str);
        int16_t vx1 = (32 - vw1) / 2; if (vx1 < 0) vx1 = 0;
        qp_drawtext(display, vx1, 60, rubik_16_font, dly_str);

        qp_flush(display);
        return;
    }

    // NORMAL SCREEN
    qp_clear(display);

    uint8_t layer = get_highest_layer(layer_state);
    uint8_t current_wpm = get_current_wpm();

            // Top: Layer (Rubik 48)
            extern bool one_handed_mode_enabled;
            const char* layer_str;
            if (one_handed_mode_enabled || layer == 4) layer_str = "4";
            else if (layer == 0) layer_str = "0";
            else if (layer == 1) layer_str = "1";
            else if (layer == 2) layer_str = "2";
            else layer_str = "+";

            int16_t lw = qp_textwidth(rubik_48_font, layer_str);
            int16_t lx = (32 - lw) / 2;
            if (lx < 0) lx = 0;

            if (one_handed_mode_enabled || layer == 4) {
                // Inverted background for One-Handed mode / Layer 4
                qp_rect(display, 0, 0, 32, 50, 0, 0, 255, true);
                qp_drawtext_recolor(display, lx, 0, rubik_48_font, layer_str, 0, 0, 0, 0, 0, 255);
            } else {
                qp_drawtext(display, lx, 0, rubik_48_font, layer_str);
            }

            // Middle: WPM (Rubik 24)
            char wpm_str[16];
            snprintf(wpm_str, sizeof(wpm_str), "%02d", current_wpm);
            int16_t ww = qp_textwidth(rubik_24_font, wpm_str);
            int16_t wx = (32 - ww) / 2;
            if (wx < 0) wx = 0;
            qp_drawtext(display, wx, 55, rubik_24_font, wpm_str);

            // Bottom: Nerd Font Cyclable Icon (Y = 90)
            uint8_t actual_idx = get_my_icon_idx();
            const char* icon_str = nerd_icon_list[actual_idx];
            int16_t iw = qp_textwidth(nerd_32_font, icon_str);
            int16_t ix = (32 - iw) / 2;
            if (ix < 0) ix = 0;
            qp_drawtext(display, ix, 90, nerd_32_font, icon_str);
    qp_flush(display);
}

// OLED Leader state updater
void oled_set_leader_state(bool active) {
    is_leader_active = active;
    display_override = active ? 2 : 0;
    print_status_wpm();
}

void cycle_nerd_icon(bool forward) {
    if (forward) {
        current_icon_idx = (current_icon_idx + 1) % NUM_ICONS;
    } else {
        if (current_icon_idx == 0) current_icon_idx = NUM_ICONS - 1;
        else current_icon_idx--;
    }
    // Force a redraw instantly when the encoder is turned
    print_status_wpm();
}



void qp_setup_oled(void) {
    display = qp_sh1106_make_i2c_device(128, 32, 0x3C);
    qp_init(display, QP_ROTATION_90);
    rubik_24_font = qp_load_font_mem(font_rubik_24);
    rubik_48_font = qp_load_font_mem(font_rubik_48);
    nerd_32_font = qp_load_font_mem(font_nerd_32);
    modernpixels_24_font = qp_load_font_mem(font_modernpixels_24);
    modernpixels_12_font = qp_load_font_mem(font_modernpixels_12);
    sfmono_16_font = qp_load_font_mem(font_sfmono_16);
    rubik_16_font = qp_load_font_mem(font_rubik_16);
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
    
    // Write directly to the global variables on the slave side so they match the master
    extern uint8_t dynamic_mouse_speed;
    extern uint8_t dynamic_scroll_speed;
    extern uint8_t speed_cycle_mode;
    extern uint8_t dynamic_string_delay;
    dynamic_mouse_speed = data->dynamic_mouse_speed;
    dynamic_scroll_speed = data->dynamic_scroll_speed;
    speed_cycle_mode = data->speed_cycle_mode;
    screensaver_timeout = data->screensaver_timeout;
    screenoff_timeout = data->screenoff_timeout;
    dynamic_string_delay = data->dynamic_string_delay;
}

static void print_mouse_mode_screen(bool inverted) {
    extern uint8_t dynamic_mouse_speed;
    extern uint8_t dynamic_scroll_speed;
    extern uint8_t speed_cycle_mode;

    char mode_str[4];
    if (speed_cycle_mode == 0) strcpy(mode_str, "LOW");
    else strcpy(mode_str, "MID");

    char speed_str[8];
    snprintf(speed_str, sizeof(speed_str), "M:%d", dynamic_mouse_speed);

    char scroll_str[8];
    snprintf(scroll_str, sizeof(scroll_str), "S:%d", dynamic_scroll_speed);

    int16_t mw = qp_textwidth(modernpixels_12_font, mode_str);
    int16_t mx = (32 - mw) / 2; if (mx < 0) mx = 0;

    int16_t sw = qp_textwidth(modernpixels_12_font, speed_str);
    int16_t sx = (32 - sw) / 2; if (sx < 0) sx = 0;

    int16_t scw = qp_textwidth(modernpixels_12_font, scroll_str);
    int16_t scx = (32 - scw) / 2; if (scx < 0) scx = 0;

    qp_clear(display);
    if (inverted) {
        // Draw white background only for the 'MOU' area
        qp_rect(display, 0, 0, 31, 79, 0, 0, 255, true);
        qp_drawtext_recolor(display, 6, 4, rubik_24_font, "M", 0,0,0, 0,0,255);
        qp_drawtext_recolor(display, 6, 28, rubik_24_font, "O", 0,0,0, 0,0,255);
        qp_drawtext_recolor(display, 6, 52, rubik_24_font, "U", 0,0,0, 0,0,255);
    } else {
        qp_drawtext(display, 6, 4, rubik_24_font, "M");
        qp_drawtext(display, 6, 28, rubik_24_font, "O");
        qp_drawtext(display, 6, 52, rubik_24_font, "U");
    }

    // Always draw mode, speed, and scroll on black background at the bottom
    qp_drawtext(display, mx, 83, modernpixels_12_font, mode_str);
    qp_drawtext(display, sx, 97, modernpixels_12_font, speed_str);
    qp_drawtext(display, scx, 111, modernpixels_12_font, scroll_str);

    qp_flush(display);
}

void housekeeping_task_user(void) {
    static bool was_mouse_mode = false;

    // --- Slave-side activity detection ---
    // On the slave, process_record_user never fires, so we detect activity here
    if (!is_keyboard_master()) {
        static uint8_t prev_synced_layer = 255;
        static uint8_t prev_synced_wpm = 255;
        static uint8_t prev_synced_override = 255;

        uint8_t cur_layer = get_highest_layer(layer_state);
        uint8_t cur_wpm = get_current_wpm();
        uint8_t cur_override = slave_oled_sync.display_override;

        // If any synced data changed, it means the master is active
        if (cur_layer != prev_synced_layer || cur_wpm != prev_synced_wpm || cur_override != prev_synced_override) {
            prev_synced_layer = cur_layer;
            prev_synced_wpm = cur_wpm;
            prev_synced_override = cur_override;
            oled_reset_screensaver();
        }

        // Trigger reshuffle on slave if master shuffled
        if (slave_oled_sync.is_shuffled && !is_shuffled) {
            void reshuffle_icons(void);
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
            
            extern uint8_t dynamic_mouse_speed;
            extern uint8_t dynamic_scroll_speed;
            extern uint8_t speed_cycle_mode;
            extern uint8_t dynamic_string_delay;
            sync_data.dynamic_mouse_speed = dynamic_mouse_speed;
            sync_data.dynamic_scroll_speed = dynamic_scroll_speed;
            sync_data.speed_cycle_mode = speed_cycle_mode;
            sync_data.screensaver_timeout = screensaver_timeout;
            sync_data.screenoff_timeout = screenoff_timeout;
            sync_data.dynamic_string_delay = dynamic_string_delay;
            
            transaction_rpc_send(SPLIT_OLED_SYNC, sizeof(sync_data), &sync_data);
        }
    }

    // --- MOU mode: show on both sides (override = 1) ---
    // Skip screensaver/timeout on mouse layer (Layer 3) - user is actively mousing
    if (get_highest_layer(layer_state) == 3) {
        last_activity_timer = timer_read32();
        if (is_screen_off) {
            is_screen_off = false;
            qp_power(display, true);
        }
        if (is_screensaver_active) {
            is_screensaver_active = false;
        }

        // Set override so both sides show MOU
        if (is_keyboard_master()) {
            display_override = 1;
        }

        static uint32_t mouse_flash_timer = 0;
        static bool mouse_inverted = false;
        static bool last_mouse_inverted = false;
        extern uint8_t dynamic_mouse_speed;
        extern uint8_t dynamic_scroll_speed;
        extern uint8_t speed_cycle_mode;
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
    // --- Slave: MOU override without being on layer 3 locally ---
    // The slave might see override=1 from the synced data even though its local
    // layer_state hasn't changed to 3 yet (sync lag). Handle MOU drawing here too.
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
        extern uint8_t dynamic_mouse_speed;
        extern uint8_t dynamic_scroll_speed;
        extern uint8_t speed_cycle_mode;
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

    // Turn off screen dynamically based on screenoff_timeout
    if (elapsed > screenoff_timeout) {
        if (!is_screen_off) {
            is_screen_off = true;
            qp_clear(display);
            qp_flush(display);
            qp_power(display, false);
        }
        return; // Skip drawing when screen is off
    }

    // Screensaver idle check dynamically based on screensaver_timeout
    if (elapsed > screensaver_timeout) {
        static uint32_t screensaver_cycle_timer = 0;
        if (!is_screensaver_active) {
            is_screensaver_active = true;
            screensaver_cycle_timer = timer_read32();
            print_screensaver();
        } else {
            // Update every 100ms for a smooth 10fps falling animation
            if (timer_elapsed32(screensaver_cycle_timer) > 100) {
                screensaver_cycle_timer = timer_read32();
                print_screensaver();
            }
        }
        return; // Skip normal UI updates while screensaver is active
    }

    static uint32_t last_draw = 0;
    static uint8_t last_wpm = 255;
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

    uint8_t current_wpm = get_current_wpm();
    uint8_t current_layer = get_highest_layer(layer_state);

    if (current_wpm != last_wpm || current_layer != last_layer) {
        if (timer_elapsed32(last_draw) > 100) {
            print_status_wpm();
            last_wpm = current_wpm;
            last_layer = current_layer;
            last_draw = timer_read32();
        }
    }

    // Auto-cycle icon every 1.5 seconds to prevent burn-in
    static uint32_t cycle_timer = 0;
    if (timer_elapsed32(cycle_timer) > 1500) {
        cycle_timer = timer_read32();
        cycle_nerd_icon(true);
    }
}

bool shutdown_user(bool jump_to_bootloader) {
    if (jump_to_bootloader) {
        // Clear the screen to black
        qp_rect(display, 0, 0, 31, 127, 0, 0, 0, true);

#ifdef IS_LEFT_SIDE
        const char* text = "LEFT";
#elif defined(IS_RIGHT_SIDE)
        const char* text = "RGHT";
#else
        const char* text = "X";
#endif

        int len = 0;
        while (text[len] != '\0') len++;
        
        // 112 pixels reserved for the vertical text, leaving bottom 16px for "BOOT"
        int spacing = 112 / len;

        // Draw "LEFT" or "RIGHT" vertically in large text (White text on Black bg)
        for (int i = 0; i < len; i++) {
            char c_str[2] = { text[i], '\0' };
            int16_t w = qp_textwidth(rubik_24_font, c_str);
            int16_t x = (32 - w) / 2; 
            if (x < 0) x = 0;
            // Center the character in its vertical slot
            int16_t y = i * spacing + (spacing - 24) / 2;
            if (y < 0) y = 0;
            qp_drawtext_recolor(display, x, y, rubik_24_font, c_str, 0, 0, 255, 0, 0, 0);
        }

        // Draw a white rectangle block at the bottom for the BOOT text
        qp_rect(display, 0, 112, 31, 127, 0, 0, 255, true);

        // Draw "BOOT" horizontally in small text at the bottom (Black text on White bg)
        const char* boot_str = "BOOT";
        int16_t boot_w = qp_textwidth(modernpixels_12_font, boot_str);
        int16_t boot_x = (32 - boot_w) / 2;
        if (boot_x < 0) boot_x = 0;
        qp_drawtext_recolor(display, boot_x, 114, modernpixels_12_font, boot_str, 0, 0, 0, 0, 0, 255);

        qp_flush(display);
    }
    return true; // Let QMK continue with the normal shutdown
}
#endif
