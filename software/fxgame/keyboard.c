#include "keyboard.h"

alt_up_ps2_dev *ps2;

static void keyboard_ISR( void* arg)
{
	Players* players = (Players*)arg;
    KB_CODE_TYPE decode_mode;
    alt_u8 buffer;
    char ascii;
    if ((decode_scancode(ps2, &decode_mode, &buffer, &ascii)==0))
    {
    	if (decode_mode == KB_ASCII_MAKE_CODE) {
    		movePress(players->list[0],ascii);
    		movePress(players->list[1],ascii);
    	} else if (decode_mode == KB_BREAK_CODE) {

    	}
    }
    alt_up_ps2_clear_fifo(ps2);
}

void kbd_init(void* arg)
{
    ps2 = alt_up_ps2_open_dev("/dev/ps2"); //open PS/2 device
    alt_up_ps2_init(ps2);
    alt_up_ps2_clear_fifo(ps2);
    alt_irq_register(PS2_IRQ, arg, keyboard_ISR);
    alt_up_ps2_enable_read_interrupt(ps2);
}

void kbd_clear(void)
{
    alt_up_ps2_clear_fifo(ps2);
}
