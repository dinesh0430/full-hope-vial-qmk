/* Quantum Painter OLED Display Logic */
#include QMK_KEYBOARD_H
#include <stdio.h>
#include <stdbool.h>

#ifdef QUANTUM_PAINTER_ENABLE
#include <qp.h>
#include "fonts/rubik_24.qff.h"
#include "fonts/rubik_48.qff.h"
#include "fonts/nerd_32.qff.h"
#include "fonts/modernpixels_24.qff.h"
#include "fonts/modernpixels_12.qff.h"

painter_device_t display;
painter_font_handle_t rubik_24_font;
painter_font_handle_t rubik_48_font;
painter_font_handle_t nerd_32_font;
painter_font_handle_t modernpixels_24_font;
painter_font_handle_t modernpixels_12_font;

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

#define NUM_OLED_PAGES 4
uint8_t current_oled_page = 0;

static uint8_t current_icon_idx = 0;
bool is_leader_active = false;
static uint32_t last_activity_timer = 0;
bool is_screensaver_active = false;
bool is_screen_off = false;

static uint8_t shuffled_indices[NUM_ICONS];
static bool is_shuffled = false;

static void print_status_wpm(void); // Forward declaration to fix compiler error

#define NUM_FALLING_ICONS 2
#define FALLING_ICON_SPACING 80  // Total travel is 160px (-32 to 128), split across 2 icons

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

static void print_status_wpm(void) {
    if (is_leader_active) {
        // Draw a solid white rectangle over the entire screen (val=255 means white)
        qp_rect(display, 0, 0, 31, 127, 0, 0, 255, true);

        // Draw "LDR" vertically using black text (fg=0) on white background (bg=255)
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

    // NORMAL SCREEN
    qp_clear(display);

    uint8_t layer = get_highest_layer(layer_state);
    uint8_t current_wpm = get_current_wpm();

    switch (current_oled_page) {
        case 0: {
            // Top: Layer (Rubik 48)
            const char* layer_str;
            if (layer == 0) layer_str = "0";
            else if (layer == 1) layer_str = "1";
            else if (layer == 2) layer_str = "2";
            else layer_str = "+";

            int16_t lw = qp_textwidth(rubik_48_font, layer_str);
            int16_t lx = (32 - lw) / 2;
            if (lx < 0) lx = 0;
            qp_drawtext(display, lx, 0, rubik_48_font, layer_str);

            // Middle: WPM (Rubik 24)
            char wpm_str[16];
            snprintf(wpm_str, sizeof(wpm_str), "%02d", current_wpm);
            int16_t ww = qp_textwidth(rubik_24_font, wpm_str);
            int16_t wx = (32 - ww) / 2;
            if (wx < 0) wx = 0;
            qp_drawtext(display, wx, 55, rubik_24_font, wpm_str);

            // Bottom: Nerd Font Cyclable Icon (Y = 90)
            uint8_t actual_idx = is_shuffled ? shuffled_indices[current_icon_idx] : current_icon_idx;
            const char* icon_str = nerd_icon_list[actual_idx];
            int16_t iw = qp_textwidth(nerd_32_font, icon_str);
            int16_t ix = (32 - iw) / 2;
            if (ix < 0) ix = 0;
            qp_drawtext(display, ix, 90, nerd_32_font, icon_str);
            break;
        }
        case 1: {
            // Page 2: WPM Battery Bar & Layer
            
            // Top: 2 Horizontal Bars (Y=8 to 22)
            // Bar 1 (Fills Left to Right)
            qp_rect(display, 2, 8, 29, 13, 0,0,255, true); // Frame
            qp_rect(display, 3, 9, 28, 12, 0,0,0, true);   // Empty
            
            // Bar 2 (Fills Right to Left)
            qp_rect(display, 2, 17, 29, 22, 0,0,255, true); // Frame
            qp_rect(display, 3, 18, 28, 21, 0,0,0, true);   // Empty
            
            // Calculate fill based on WPM (Max 120, inner width is 26 pixels)
            uint8_t wpm_cap = current_wpm > 120 ? 120 : current_wpm;
            uint8_t fill_w = (wpm_cap * 26) / 120;
            
            if (fill_w > 0) {
                // Top fill (L -> R)
                qp_rect(display, 3, 9, 2 + fill_w, 12, 0,0,255, true);
                // Bottom fill (R -> L)
                qp_rect(display, 29 - fill_w, 18, 28, 21, 0,0,255, true);
            }
            
            // Middle: Layer Indicator (Huge font)
            const char* layer_str;
            if (layer == 0) layer_str = "0";
            else if (layer == 1) layer_str = "1";
            else if (layer == 2) layer_str = "2";
            else layer_str = "+";
            
            int16_t lw = qp_textwidth(rubik_48_font, layer_str);
            int16_t lx = (32 - lw) / 2;
            if (lx < 0) lx = 0;
            qp_drawtext(display, lx, 35, rubik_48_font, layer_str);
            
            // Ending: Changing Icon
            uint8_t actual_idx = is_shuffled ? shuffled_indices[current_icon_idx] : current_icon_idx;
            const char* icon_str = nerd_icon_list[actual_idx];
            int16_t iw = qp_textwidth(nerd_32_font, icon_str);
            int16_t ix = (32 - iw) / 2;
            if (ix < 0) ix = 0;
            qp_drawtext(display, ix, 90, nerd_32_font, icon_str);
            break;
        }
        case 2: {
            // Page 2: The Equalizer (WPM Audio Visualizer) (Old 4)
            // Top: Layer in small text
            const char* layer_str;
            if (layer == 0) layer_str = "L0";
            else if (layer == 1) layer_str = "L1";
            else if (layer == 2) layer_str = "L2";
            else layer_str = "L+";
            
            int16_t lw = qp_textwidth(rubik_24_font, layer_str);
            int16_t lx = (32 - lw) / 2;
            if (lx < 0) lx = 0;
            qp_drawtext(display, lx, 5, rubik_24_font, layer_str);
            
            // 3 EQ Bars based on WPM (Restricted max height to 40)
            uint8_t h1 = (current_wpm * 3) % 30;
            uint8_t h2 = current_wpm > 40 ? 40 : current_wpm; 
            uint8_t h3 = (current_wpm * 5) % 35;
            
            if (h1 < 2) h1 = 2;
            if (h2 < 2) h2 = 2;
            if (h3 < 2) h3 = 2;
            
            // Draw bars starting from Y=85 to leave room for the icon
            qp_rect(display, 4, 85 - h1, 10, 85, 0,0,255, true);
            qp_rect(display, 13, 85 - h2, 19, 85, 0,0,255, true);
            qp_rect(display, 22, 85 - h3, 28, 85, 0,0,255, true);
            
            // Floating caps
            qp_rect(display, 4, 85 - h1 - 4, 10, 85 - h1 - 3, 0,0,255, true);
            qp_rect(display, 13, 85 - h2 - 4, 19, 85 - h2 - 3, 0,0,255, true);
            qp_rect(display, 22, 85 - h3 - 4, 28, 85 - h3 - 3, 0,0,255, true);
            
            // Separator line
            qp_rect(display, 6, 92, 25, 93, 0,0,255, true);
            
            // Small changing icon section at the bottom (Y=95)
            uint8_t actual_idx = is_shuffled ? shuffled_indices[current_icon_idx] : current_icon_idx;
            const char* icon_str = nerd_icon_list[actual_idx];
            int16_t iw = qp_textwidth(nerd_32_font, icon_str);
            int16_t ix = (32 - iw) / 2;
            if (ix < 0) ix = 0;
            qp_drawtext(display, ix, 95, nerd_32_font, icon_str);
            break;
        }
        case 3: {
            // Page 3: Modern Typography Poster (Old 8)
            qp_rect(display, 0, 0, 31, 35, 0,0,255, true);
            
            const char* layer_str;
            if (layer == 0) layer_str = "L0";
            else if (layer == 1) layer_str = "L1";
            else if (layer == 2) layer_str = "L2";
            else layer_str = "L+";
            
            int16_t lw = qp_textwidth(rubik_24_font, layer_str);
            qp_drawtext_recolor(display, (32-lw)/2, 6, rubik_24_font, layer_str, 0,0,0, 0,0,255);
            
            int16_t w_lbl = qp_textwidth(modernpixels_12_font, "WPM");
            qp_drawtext(display, 30 - w_lbl, 40, modernpixels_12_font, "WPM");
            
            char wpm_str[16];
            snprintf(wpm_str, sizeof(wpm_str), "%d", current_wpm);
            qp_drawtext(display, 2, 58, modernpixels_24_font, wpm_str);
            
            qp_rect(display, 2, 85, 30, 88, 0,0,255, true);
            
            uint8_t actual_idx = is_shuffled ? shuffled_indices[current_icon_idx] : current_icon_idx;
            const char* icon_str = nerd_icon_list[actual_idx];
            int16_t iw = qp_textwidth(nerd_32_font, icon_str);
            qp_drawtext(display, (32-iw)/2, 95, nerd_32_font, icon_str);
            break;
        }
    }

    qp_flush(display);
}

// OLED Leader state updater
void oled_set_leader_state(bool active) {
    is_leader_active = active;
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

void cycle_oled_page(bool forward) {
    if (forward) {
        current_oled_page = (current_oled_page + 1) % NUM_OLED_PAGES;
    } else {
        if (current_oled_page == 0) current_oled_page = NUM_OLED_PAGES - 1;
        else current_oled_page--;
    }
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
    last_activity_timer = timer_read32();
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

        static uint32_t mouse_flash_timer = 0;
        static bool mouse_inverted = false;
        if (timer_elapsed32(mouse_flash_timer) > 500) { // flash every 500ms
            mouse_flash_timer = timer_read32();
            mouse_inverted = !mouse_inverted;
            print_mouse_mode_screen(mouse_inverted);
        }
        
        was_mouse_mode = true;
        return; // Skip normal UI updates
    }

    if (was_mouse_mode) {
        was_mouse_mode = false;
        print_status_wpm(); // Force an immediate redraw of the normal UI
    }

    uint32_t elapsed = timer_elapsed32(last_activity_timer);

    // Turn off screen after 3 minutes of inactivity (180,000 ms)
    if (elapsed > 180000) {
        if (!is_screen_off) {
            is_screen_off = true;
            qp_clear(display);
            qp_flush(display);
            qp_power(display, false);
        }
        return; // Skip drawing when screen is off
    }

    // Screensaver idle check (30 seconds = 30,000 ms)
    if (elapsed > 30000) {
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

    // Do not auto-update normal UI if leader screen is active
    if (is_leader_active) return;

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
        // Fill the screen with white to make it super obvious
        qp_rect(display, 0, 0, 31, 127, 0, 0, 255, true);

        const char* text_b = "B";
        const char* text_o = "O";
        const char* text_t = "T";

        int16_t bw = qp_textwidth(rubik_24_font, text_b);
        int16_t ow = qp_textwidth(rubik_24_font, text_o);
        int16_t tw = qp_textwidth(rubik_24_font, text_t);

        int16_t bx = (32 - bw) / 2; if (bx < 0) bx = 0;
        int16_t ox = (32 - ow) / 2; if (ox < 0) ox = 0;
        int16_t tx = (32 - tw) / 2; if (tx < 0) tx = 0;

        // Draw "BOOT" vertically in black text
        qp_drawtext_recolor(display, bx, 10, rubik_24_font, text_b, 0, 0, 0, 0, 0, 255);
        qp_drawtext_recolor(display, ox, 40, rubik_24_font, text_o, 0, 0, 0, 0, 0, 255);
        qp_drawtext_recolor(display, ox, 70, rubik_24_font, text_o, 0, 0, 0, 0, 0, 255);
        qp_drawtext_recolor(display, tx, 100, rubik_24_font, text_t, 0, 0, 0, 0, 0, 255);

        qp_flush(display);
    }
    return true; // Let QMK continue with the normal shutdown
}
#endif
