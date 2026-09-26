// Auto-generated header for 18 character animations
#pragma once

#include <qp.h>

#include "gfx/warrior_idle.qgf.h"
#include "gfx/warrior_run.qgf.h"
#include "gfx/warrior_atk1.qgf.h"
#include "gfx/warrior_atk2.qgf.h"
#include "gfx/warrior_guard.qgf.h"
#include "gfx/monk_idle.qgf.h"
#include "gfx/monk_run.qgf.h"
#include "gfx/monk_heal.qgf.h"
#include "gfx/monk_effect.qgf.h"
#include "gfx/archer_idle.qgf.h"
#include "gfx/archer_run.qgf.h"
#include "gfx/archer_shoot.qgf.h"
#include "gfx/pawn_idle.qgf.h"

#define NUM_CHAR_ANIMS 13

typedef struct {
    const uint8_t *data;
    uint8_t width;
    uint8_t height;
    const char *name;
} char_anim_info_t;

static const char_anim_info_t char_anim_list[NUM_CHAR_ANIMS] = {
    { gfx_warrior_idle, 32, 33, "Warrior Idle" },
    { gfx_warrior_run, 32, 30, "Warrior Run" },
    { gfx_warrior_atk1, 32, 30, "Warrior Attack 1" },
    { gfx_warrior_atk2, 32, 28, "Warrior Attack 2" },
    { gfx_warrior_guard, 32, 38, "Warrior Guard" },
    { gfx_monk_idle, 32, 40, "Monk Idle" },
    { gfx_monk_run, 32, 27, "Monk Run" },
    { gfx_monk_heal, 32, 18, "Monk Heal" },
    { gfx_monk_effect, 32, 41, "Monk Heal Effect" },
    { gfx_archer_idle, 32, 37, "Archer Idle" },
    { gfx_archer_run, 32, 36, "Archer Run" },
    { gfx_archer_shoot, 32, 29, "Archer Shoot" },
    { gfx_pawn_idle, 32, 36, "Pawn Idle" },
};
