#ifndef SCORE_H
#define SCORE_H

#include "animations.h"
#include "sys/alt_irq.h"
#include "io.h"
#include "Types.h"
#include "Map.h"

typedef struct Score {
    int scores[NUM_PLAYERS];
    char buffer [32];
    int timeLeft;
    int spawnRateTotal;
    mapTile** map;
} Score;

extern int *scores[2];

char renderScore (Score* p, alt_up_char_buffer_dev *char_buffer);
void gameOver(Score* s, alt_up_char_buffer_dev *char_buffer);
void initTimer(void* score, alt_up_char_buffer_dev *char_buffer);

#endif //SCORE_H
