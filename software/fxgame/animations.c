#include "animations.h"

Pixel_Map* booted_bmps;

//alt_up_char_buffer_dev *char_buffer = init_char_stuff("/dev/char_drawer");
alt_up_char_buffer_dev *init_char_stuff(char *location) {
    //Character Buffer
    alt_up_char_buffer_dev *char_buffer;
    char_buffer = alt_up_char_buffer_open_dev(location);
    alt_up_char_buffer_init(char_buffer);

    return char_buffer;
}

//alt_up_pixel_buffer_dma_dev *pixel_buffer = init_pixel_stuff("/dev/pixel_buffer_dma");
alt_up_pixel_buffer_dma_dev *init_pixel_stuff(char *location){
    //Pixel Buffer
    alt_up_pixel_buffer_dma_dev *pixel_buffer;

    //Opens the pixel buffer device specified by /dev/pixel_buffer_dma
    pixel_buffer = alt_up_pixel_buffer_dma_open_dev(location);

    unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
    unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + PIXEL_BUFFER_SPAN / 2;

    //Change the 1st buffer's start address
    alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr1);

    //Swap background and foreground buffers
    alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);

    //Wait for the swap to complete
    while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

    //Change the 1st buffer's start address
    alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr2);

    //Clear the screen
    alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
    alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);

    return pixel_buffer;
}


int refresh(alt_up_pixel_buffer_dma_dev *pixel_buffer) {
    alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
    while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer)){
    	//refreshSoundBG(sb);
    }
    //usleep(66666);
    return 1;
}

int draw_screen_from_pixel_map_16(alt_up_pixel_buffer_dma_dev *pixel_buffer, Pixel* pixel_map_16, int x0, int y0) {
    unsigned int i, j = 0, x, y;
    uint16_t color;
    for (i = 0; i < PS; i++) {
        x = i % 16;
        // Go down one line, except if it's the first line
        if (i % 16 == 0 && i != 0){
            j++;
        }
        y = j;

        color = pixel_map_16[i].red << 11 | pixel_map_16[i].green << 5 | pixel_map_16[i].blue;
        if (color != 0x7C0) {
            //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x + x0, y + y0, x + x0, y + y0, color, 1);
            draw_pixel_fast(pixel_buffer, color, x + x0, y + y0);
        }
    }
    return 1;
}

int draw_screen_from_bmp(alt_up_pixel_buffer_dma_dev *pixel_buffer, Pixel_Map* booted_bmps, int filename, int x, int y) {
    //printf("READING BMP\n");
    Pixel* pixel_map_16 = NULL;
    if (filename == 0) {
        pixel_map_16 = booted_bmps->CRATE_pixel_map;
    } else if (filename == 1) {
        pixel_map_16 = booted_bmps->GRASS_pixel_map;
    } else if (filename == 2) {
        pixel_map_16 = booted_bmps->BOMB_pixel_map;
    } else if (filename == 3) {
        pixel_map_16 = booted_bmps->VOLT_pixel_map;
    } else if (filename == 4) {
        pixel_map_16 = booted_bmps->PIKA_pixel_map;
    } else if (filename == 5) {
        pixel_map_16 = booted_bmps->WALL_pixel_map;
    } else if (filename == 6) {
        pixel_map_16 = booted_bmps->EXPL_pixel_map;
    }

    //printf("Finished Converting 24 to 16\n");
    draw_screen_from_pixel_map_16(pixel_buffer, pixel_map_16, x, y);

    return 1;
}

int boot_bmps(Pixel_Map* booted_bmps) {
    booted_bmps->TEST1_pixel_map = init_pixel_map_16_from_bmp("TEST1.BMP");
    booted_bmps->GRASS_pixel_map = init_pixel_map_16_from_bmp("GRASS.BMP");
    booted_bmps->FB_pixel_map = init_pixel_map_16_from_bmp("FB.BMP");
    booted_bmps->VOLT_pixel_map = init_pixel_map_16_from_bmp("VOLT.BMP");
    booted_bmps->PIKA_pixel_map = init_pixel_map_16_from_bmp("PIKA.BMP");
    booted_bmps->WALL_pixel_map = init_pixel_map_16_from_bmp("WALL.BMP");
    booted_bmps->O_pixel_map = init_pixel_map_16_from_bmp("O.bmp");
    booted_bmps->OX_pixel_map = init_pixel_map_16_from_bmp("OX.bmp");
    booted_bmps->C_pixel_map = init_pixel_map_16_from_bmp("C.bmp");
    booted_bmps->CX_pixel_map = init_pixel_map_16_from_bmp("CX.bmp");
    booted_bmps->B_pixel_map = init_pixel_map_16_from_bmp("B.bmp");
    booted_bmps->BX_pixel_map = init_pixel_map_16_from_bmp("BX.bmp");
    booted_bmps->W_pixel_map = init_pixel_map_16_from_bmp("W.bmp");
    booted_bmps->WX_pixel_map = init_pixel_map_16_from_bmp("WX.bmp");
    booted_bmps->PLUS_pixel_map = init_pixel_map_16_from_bmp("PLUS.bmp");
    booted_bmps->KICK_pixel_map = init_pixel_map_16_from_bmp("KICK.bmp");
    booted_bmps->RAD_pixel_map = init_pixel_map_16_from_bmp("RAD.bmp");
    booted_bmps->GLVE_pixel_map = init_pixel_map_16_from_bmp("GLVE.bmp");
    booted_bmps->STAR_pixel_map = init_pixel_map_16_from_bmp("STAR.bmp");
    booted_bmps->BULL_pixel_map = init_pixel_map_16_from_bmp("BULL.bmp");
    booted_bmps->BRNZ_pixel_map = init_pixel_map_16_from_bmp("BRNZ.bmp");
    booted_bmps->SILV_pixel_map = init_pixel_map_16_from_bmp("SILV.bmp");
    booted_bmps->GOLD_pixel_map = init_pixel_map_16_from_bmp("GOLD.bmp");
    booted_bmps->CRATE_pixel_map = init_pixel_map_16_from_bmp("CRATE.BMP");

    booted_bmps->BUP_pixel_map = init_pixel_map_16_from_bmp("BUP.bmp");
    booted_bmps->BDOWN_pixel_map = init_pixel_map_16_from_bmp("BDOWN.bmp");
    booted_bmps->BLEFT_pixel_map = init_pixel_map_16_from_bmp("BLEFT.bmp");
    booted_bmps->BRIGHT_pixel_map = init_pixel_map_16_from_bmp("BRIGHT.BMP");

    booted_bmps->CUP_pixel_map = init_pixel_map_16_from_bmp("CUP.bmp");
    booted_bmps->CDOWN_pixel_map = init_pixel_map_16_from_bmp("CDOWN.bmp");
    booted_bmps->CLEFT_pixel_map = init_pixel_map_16_from_bmp("CLEFT.bmp");
    booted_bmps->CRIGHT_pixel_map = init_pixel_map_16_from_bmp("CRIGHT.BMP");

    booted_bmps->WUP_pixel_map = init_pixel_map_16_from_bmp("WUP.bmp");
    booted_bmps->WDOWN_pixel_map = init_pixel_map_16_from_bmp("WDOWN.bmp");
    booted_bmps->WLEFT_pixel_map = init_pixel_map_16_from_bmp("WLEFT.bmp");
    booted_bmps->WRIGHT_pixel_map = init_pixel_map_16_from_bmp("WRIGHT.BMP");

    booted_bmps->OUP_pixel_map = init_pixel_map_16_from_bmp("OUP.bmp");
    booted_bmps->ODOWN_pixel_map = init_pixel_map_16_from_bmp("ODOWN.bmp");
    booted_bmps->OLEFT_pixel_map = init_pixel_map_16_from_bmp("OLEFT.bmp");
    booted_bmps->ORIGHT_pixel_map = init_pixel_map_16_from_bmp("ORIGHT.BMP");

    booted_bmps->BCRATE_pixel_map = init_pixel_map_16_from_bmp("BCRATE.bmp");
    booted_bmps->BWALL_pixel_map = init_pixel_map_16_from_bmp("BWALL.bmp");
    booted_bmps->BGRASS_pixel_map = init_pixel_map_16_from_bmp("BGRASS.BMP");

    booted_bmps->CCRATE_pixel_map = init_pixel_map_16_from_bmp("CCRATE.bmp");
    booted_bmps->CWALL_pixel_map = init_pixel_map_16_from_bmp("CWALL.bmp");
    booted_bmps->CGRASS_pixel_map = init_pixel_map_16_from_bmp("CGRASS.BMP");

    booted_bmps->OCRATE_pixel_map = init_pixel_map_16_from_bmp("OCRATE.bmp");
    booted_bmps->OWALL_pixel_map = init_pixel_map_16_from_bmp("OWALL.bmp");
    booted_bmps->OGRASS_pixel_map = init_pixel_map_16_from_bmp("OGRASS.BMP");

    booted_bmps->WCRATE_pixel_map = init_pixel_map_16_from_bmp("WCRATE.bmp");
    booted_bmps->WWALL_pixel_map = init_pixel_map_16_from_bmp("WWALL.bmp");
    booted_bmps->WGRASS_pixel_map = init_pixel_map_16_from_bmp("WGRASS.BMP");

    return booted_bmps;
}

int draw_pixel_fast(alt_up_pixel_buffer_dma_dev *pixel_buffer,
        unsigned int color, unsigned int x, unsigned int y) {
    unsigned int addr = 0;

    addr |= ((x & pixel_buffer->x_coord_mask) << pixel_buffer->x_coord_offset);
    addr |= ((y & pixel_buffer->y_coord_mask) << pixel_buffer->y_coord_offset);

    IOWR_16DIRECT(pixel_buffer->back_buffer_start_address, addr, color);

    return 0;
}
