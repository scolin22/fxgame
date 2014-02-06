#ifndef POWERUPS_H
#define POWERUPS_H

#include "Player.h"

typedef enum {
    kick = 1 << 0,
    toss = 1 << 1,
    bulldozer = 1 << 2,
    invincible = 1 << 3
} powerUps;

#endif
