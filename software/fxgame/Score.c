#include "Score.h"

/* itoa:  convert n to characters in s */
char *itoa(int value, char *buf) {
    sprintf(buf, "%d", value);
    return buf;
}

void renderScore (Score* s, alt_up_char_buffer_dev *char_buffer) {
    alt_up_char_buffer_string(char_buffer, "          ", 50, 1);
    alt_up_char_buffer_string(char_buffer, "          ", 50, 2);
    itoa(s->scores[0], s->buffer);
    alt_up_char_buffer_string(char_buffer, s->buffer, 50, 1);
    itoa(s->scores[1], s->buffer);
    alt_up_char_buffer_string(char_buffer, s->buffer, 50, 2);
}
