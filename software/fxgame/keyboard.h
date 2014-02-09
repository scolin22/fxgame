#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "altera_up_avalon_ps2.h"
#include "altera_up_ps2_keyboard.h"
#include "system.h"
#include "alt_types.h"
#include "stdlib.h"
#include "string.h"
#include "sys/alt_irq.h"
#include "io.h"
#include "sys/alt_timestamp.h"
#include <system.h>
#include "Player.h"

#define SCAN_CODE_NUM  102

typedef struct keys {
    int pressed;
    int elapsedTime;
}keys;

int get_ascii_code_index(char code);

char kbd_input();
void kbd_init(void* arg);
void kbd_clear(void);

#endif
