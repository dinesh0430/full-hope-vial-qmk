BOOTMAGIC_ENABLE = yes
CAPS_WORD_ENABLE = yes
COMBO_ENABLE = no
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
WPM_ENABLE = no

SRC += fonts/fool_16.qff.c fonts/fool_32.qff.c fonts/rubik_48.qff.c fonts/nerd_32.qff.c \
       gfx/wireframe_cube.qgf.c gfx/liquid_metaballs.qgf.c gfx/bouncing_balls.qgf.c \
       gfx/plasma_wave.qgf.c gfx/lava_lamp.qgf.c gfx/raindrop_ripples.qgf.c \
       gfx/wireframe_torus.qgf.c gfx/matrix_rain.qgf.c \
       gfx/viscous_dropper.qgf.c gfx/vortex_rings.qgf.c \
       gfx/wmp_spikes.qgf.c gfx/cyber_ribbon.qgf.c \
       gfx/wave_string.qgf.c gfx/moire_curtain.qgf.c gfx/coupled_oscillators.qgf.c \
       gfx/pendulum_wave.qgf.c gfx/pendulum_waves.qgf.c gfx/superformula.qgf.c gfx/kelvin_helmholtz.qgf.c \
       gfx/chladni_plate.qgf.c \
       gfx/warrior_idle.qgf.c gfx/warrior_run.qgf.c gfx/warrior_atk1.qgf.c gfx/warrior_atk2.qgf.c gfx/warrior_guard.qgf.c \
       gfx/monk_idle.qgf.c gfx/monk_run.qgf.c gfx/monk_heal.qgf.c gfx/monk_effect.qgf.c \
       gfx/archer_idle.qgf.c gfx/archer_run.qgf.c gfx/archer_shoot.qgf.c \
       gfx/pawn_idle.qgf.c \
       gfx/pxl_anims.c \
       gfx/pimen_anims.c
