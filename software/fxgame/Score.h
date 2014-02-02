#ifndef SCORE_H
#define SCORE_H

#include "animations.h"
#include "Fruits.h"

typedef struct Score {
    int scores[NUM_PLAYERS];
    char buffer [32];
} Score;

void renderScore (Score* p, alt_up_char_buffer_dev *char_buffer);

#endif //SCORE_H
