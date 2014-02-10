#include "Score.h"

/* itoa:  convert n to characters in s */
char *itoa(int value, char *buf) {
    sprintf(buf, "%d", value);
    return buf;
}

char *secondsToTime(int seconds, char *buf) {
    sprintf(buf, "%d:%02d", seconds/60, seconds%60);
    return buf;
}

char renderScore (Score* s, alt_up_char_buffer_dev *char_buffer) {
    alt_up_char_buffer_string(char_buffer, "                ", 50, 1);
    alt_up_char_buffer_string(char_buffer, "                ", 50, 2);
    alt_up_char_buffer_string(char_buffer, itoa(s->scores[0], s->buffer), 50, 1);
    alt_up_char_buffer_string(char_buffer, itoa(s->scores[1], s->buffer), 50, 2);
    if (s->timeLeft >= 0) {
        alt_up_char_buffer_string(char_buffer, "                ", 20, 1);
        alt_up_char_buffer_string(char_buffer, secondsToTime(s->timeLeft, s->buffer), 20, 1);
        return 1;
    }
    return 0;
}

void gameOver(Score* s, alt_up_char_buffer_dev *char_buffer) {
    int i;
    int maxScore = s->scores[0];
    int bestPlayer = 0;
    for (i = 1; i < NUM_PLAYERS; i++) {
        if (s->scores[i] > maxScore) {
            maxScore = s->scores[i];
            bestPlayer = i;
        }
    }

    alt_up_char_buffer_string(char_buffer, "GAME OVER!", 34, 14);
    sprintf(s->buffer, "Player %d Won With %d Points", bestPlayer+1, maxScore);
    alt_up_char_buffer_string(char_buffer, s->buffer, 24, 15);

}

static void timer_ISR( void *arg)
{
    Score* score = (Score*)arg;
    score->timeLeft--;

    IOWR_16DIRECT(TIMER_0_BASE,0,0); //needed to show that interrupt finished executing
    IOWR_16DIRECT(TIMER_0_BASE,4,0x5); //restarts the hardware timer before exiting the isr
    return;

}

void initTimer(void* score, alt_up_char_buffer_dev *char_buffer)
{
	Score* s = (Score*) score;
	int i;
    for (i =  0; i < NUM_PLAYERS; i++) {
        s->scores[i] = 0;
    }
    int timer_period = 1 * 50000000;
    IOWR_16DIRECT(TIMER_0_BASE, 8, timer_period & 0xFFFF); //writes the period to the hardware timer
    IOWR_16DIRECT(TIMER_0_BASE, 12, timer_period >> 16);
    IOWR_16DIRECT(TIMER_0_BASE, 4, 1 << 3); //stop timer
    alt_irq_register(TIMER_0_IRQ,score,(void*)timer_ISR);//registers function to a specific IRQ
    IOWR_16DIRECT(TIMER_0_BASE, 4, 0x5); //start timer
    printf("past interrupt");
    alt_up_char_buffer_string(char_buffer, "Player 1", 40, 1);
    alt_up_char_buffer_string(char_buffer, "Player 2", 40, 2);
    alt_up_char_buffer_string(char_buffer, "Time Left", 10, 1);
    return;
}


