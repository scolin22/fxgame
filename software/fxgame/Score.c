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

void renderScore (Score* s, alt_up_char_buffer_dev *char_buffer) {
	alt_up_char_buffer_string(char_buffer, "Time Left", 10, 1);
	alt_up_char_buffer_string(char_buffer, secondsToTime(s->timeLeft, s->buffer), 20, 1);
	alt_up_char_buffer_string(char_buffer, "Player 1", 40, 1);
	alt_up_char_buffer_string(char_buffer, "Player 2", 40, 2);
	alt_up_char_buffer_string(char_buffer, "          ", 50, 1);
	alt_up_char_buffer_string(char_buffer, "          ", 50, 2);
	itoa(s->scores[0], s->buffer);
	alt_up_char_buffer_string(char_buffer, s->buffer, 50, 1);
	itoa(s->scores[1], s->buffer);
	alt_up_char_buffer_string(char_buffer, s->buffer, 50, 2);
}

static void timer_ISR( void *arg)
{
	Score* score = (Score*)arg;
	score->timeLeft--;

	IOWR_16DIRECT(TIMER_0_BASE,0,0); //needed to show that interrupt finished executing
	IOWR_16DIRECT(TIMER_0_BASE,4,0x5); //restarts the hardware timer before exiting the isr
	return;

}

void initTimer(void* arg)
{
	int timer_period = 1 * 50000000;
	IOWR_16DIRECT(TIMER_0_BASE, 8, timer_period & 0xFFFF); //writes the period to the hardware timer
	IOWR_16DIRECT(TIMER_0_BASE, 12, timer_period >> 16);
	IOWR_16DIRECT(TIMER_0_BASE, 4, 1 << 3); //stop timer
	alt_irq_register(TIMER_0_IRQ,arg,(void*)timer_ISR);//registers function to a specific IRQ
	IOWR_16DIRECT(TIMER_0_BASE, 4, 0x5); //start timer
	printf("past interrupt");
	return;
}


