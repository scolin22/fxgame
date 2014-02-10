#ifndef SCORE_H
#define SCORE_H

#include "animations.h"
#include "sys/alt_irq.h"
#include "io.h"
#include "Types.h"

typedef struct Score {
    int scores[NUM_PLAYERS];
    char buffer [32];
    int timeLeft;
} Score;

char renderScore (Score* p, alt_up_char_buffer_dev *char_buffer);
void gameOver(Score* s, alt_up_char_buffer_dev *char_buffer);
void initTimer(void* score, alt_up_char_buffer_dev *char_buffer);

#endif //SCORE_H
