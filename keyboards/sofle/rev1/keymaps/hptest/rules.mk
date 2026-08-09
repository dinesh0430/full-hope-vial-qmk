BOOTMAGIC_ENABLE = yes
CAPS_WORD_ENABLE = yes
COMBO_ENABLE = yes
COMMAND_ENABLE = yes
CONSOLE_ENABLE = yes
DYNAMIC_MACRO_ENABLE = yes
ENCODER_ENABLE = yes
ENCODER_MAP_ENABLE = yes
EXTRAKEY_ENABLE = yes
KEY_OVERRIDE_ENABLE = yes
LAYER_LOCK_ENABLE = no
LEADER_ENABLE = yes
LTO_ENABLE = no
MOUSEKEY_ENABLE = yes
OLED_ENABLE = no
QUANTUM_PAINTER_ENABLE = yes
# We use sh1106_i2c (even for SSD1306 screens) because generic SSD1306 OLEDs often
# use the cheaper SH1106 controller under the hood. Using the strict ssd1306 driver
# in QP can cause static or shifted pixels, whereas sh1106_i2c works flawlessly for both.
QUANTUM_PAINTER_DRIVERS = sh1106_i2c
QMK_SETTINGS = yes
REPEAT_KEY_ENABLE = no
RGBLIGHT_ENABLE = yes
VIA_ENABLE = yes
VIAL_ENABLE = yes
VIAL_INSECURE = yes
TAP_DANCE_ENABLE = yes       # Keeps QMK's core Tap Dance engine on
WPM_ENABLE = yes

SRC += fonts/rubik_24.qff.c fonts/rubik_48.qff.c fonts/nerd_32.qff.c fonts/modernpixels_24.qff.c fonts/modernpixels_12.qff.c fonts/sfmono_16.qff.c fonts/rubik_16.qff.c
