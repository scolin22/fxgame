#ifndef SCORE_H
#define SCORE_H

#include "animations.h"
#include "Fruits.h"
#include "sys/alt_irq.h"
#include "io.h"

typedef struct Score {
    int scores[NUM_PLAYERS];
    char buffer [32];
    int timeLeft;
} Score;

char renderScore (Score* p, alt_up_char_buffer_dev *char_buffer);

void initTimer(void* arg);

#endif //SCORE_H
