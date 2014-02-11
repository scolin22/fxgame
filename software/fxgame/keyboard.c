#include "keyboard.h"
#include "Player.h"

////////////////////////////////////////////////////////////////////
// Table of scan code, make code and their corresponding values
// These data are useful for developing more features for the keyboard
//
char *key_table[SCAN_CODE_NUM] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "`", "-", "=", "\\", "BKSP", "SPACE", "TAB", "CAPS", "L SHFT", "L CTRL", "L GUI", "L ALT", "R SHFT", "R CTRL", "R GUI", "R ALT", "APPS", "ENTER", "ESC", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "SCROLL", "[", "INSERT", "HOME", "PG UP", "DELETE", "END", "PG DN", "U ARROW", "L ARROW", "D ARROW", "R ARROW", "NUM", "KP /", "KP *", "KP -", "KP +", "KP ENTER", "KP .", "KP 0", "KP 1", "KP 2", "KP 3", "KP 4", "KP 5", "KP 6", "KP 7", "KP 8", "KP 9", "]", ";", "'", ",", ".", "/" };

char ascii_codes[SCAN_CODE_NUM] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '`', '-', '=', 0, 0x08, 0, 0x09, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0A, 0x1B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '[', 0, 0, 0, 0x7F, 0, 0, 0, 0, 0, 0, 0, '/', '*', '-', '+', 0x0A, '.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ']', ';', '\'', ',', '.', '/' };

alt_u8 single_byte_make_code[SCAN_CODE_NUM] = { 0x1C, 0x32, 0x21, 0x23, 0x24, 0x2B, 0x34, 0x33, 0x43, 0x3B, 0x42, 0x4B, 0x3A, 0x31, 0x44, 0x4D, 0x15, 0x2D, 0x1B, 0x2C, 0x3C, 0x2A, 0x1D, 0x22, 0x35, 0x1A, 0x45, 0x16, 0x1E, 0x26, 0x25, 0x2E, 0x36, 0x3D, 0x3E, 0x46, 0x0E, 0x4E, 0x55, 0x5D, 0x66, 0x29, 0x0D, 0x58, 0x12, 0x14, 0, 0x11, 0x59, 0, 0, 0, 0, 0x5A, 0x76, 0x05, 0x06, 0x04, 0x0C, 0x03, 0x0B, 0x83, 0x0A, 0x01, 0x09, 0x78, 0x07, 0x7E, 0x54, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x77, 0, 0x7C, 0x7B, 0x79, 0, 0x71, 0x70, 0x69, 0x72, 0x7A, 0x6B, 0x73, 0x74, 0x6C, 0x75, 0x7D, 0x5B, 0x4C, 0x52, 0x41, 0x49, 0x4A };

alt_u8 multi_byte_make_code[SCAN_CODE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x1F, 0, 0, 0x14, 0x27, 0x11, 0x2F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x70, 0x6C, 0x7D, 0x71, 0x69, 0x7A, 0x75, 0x6B, 0x72, 0x74, 0, 0x4A, 0, 0, 0, 0x5A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
////////////////////////////////////////////////////////////////////

alt_up_ps2_dev *ps2;
KB_CODE_TYPE decode_mode;
char ascii;
alt_u8 buf[4];

keys keyboard[SCAN_CODE_NUM] = {0};

int get_ascii_code_index(char code)
{
    unsigned i;
    for (i = 0; i < SCAN_CODE_NUM; i++ )
    {
        if ( ascii_codes[i] == code )
            return i;
    }
    return SCAN_CODE_NUM;
}

unsigned get_single_byte_make_code_index(alt_u8 code)
{
    unsigned i;
    for (i = 0; i < SCAN_CODE_NUM; i++ )
    {
        if ( single_byte_make_code[i] == code )
            return i;
    }
    return SCAN_CODE_NUM;
}

static void keyboard_ISR( void *arg)
{//keyboard interrupt handler
	Players* players = (Players*)arg;
    KB_CODE_TYPE decode_mode;
    alt_u8 buffer;
    alt_u8 byte;
    char ascii;
    unsigned idx;
    int released = 0;

    alt_up_ps2_read_data_byte(ps2, &byte);
    if(byte != 0xef){
        //      if(byte) printf("The byte read is %x\n", byte);
        while(byte == 0xf0) {
            alt_up_ps2_read_data_byte(ps2, &byte);
            released = 1;
        }
        idx = get_single_byte_make_code_index(byte);
        if ( (idx < 40 || idx == 68 || idx > 79) && ( idx != 102 ) )
        {
            decode_mode = KB_ASCII_MAKE_CODE;
            ascii = ascii_codes[idx];
            buffer = byte;
            if (released && keyboard[idx].pressed == 1){
                printf("Released %c!\n", ascii);
                keyboard[idx].pressed = 0;
            }else if (!released && keyboard[idx].pressed == 0){
                printf("Pressed %c!\n", ascii);
                keyboard[idx].pressed = 1;
                if (players->list[0]->stunnedTime < 1)
                	movePress(players->list[0], ascii);
                if (players->list[1]->stunnedTime < 1)
                	movePress(players->list[1], ascii);
            }
        }
        else
        {
            decode_mode = KB_BINARY_MAKE_CODE;
            buffer = byte;
        }
    }
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
