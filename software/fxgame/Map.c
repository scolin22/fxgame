#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#include "Map.h"

mapTile** allocateMap ()
{
    mapTile** d = (mapTile**)malloc(NTILEY*sizeof(mapTile*));
    int i;
    for (i = 0; i < NTILEY; i++) {
        d[i] = (mapTile*)malloc(NTILEX*sizeof(mapTile));
    }

    return d;
}

void initMap (mapTile** d)
{
    int y;
    for (y = 0; y < NTILEY; y++) {
        int x;
        for (x = 0; x < NTILEX; x++) {
            d[y][x].t = maplayout[y*NTILEX+x] - '0';
            d[y][x].db = 2;
            d[y][x].playerOn = 0;
            d[y][x].owner = 0;
            if (d[y][x].t == 6) {
                //give it a random value of GRASS or CRATE
                if (rand() % 2 == 0) {
                    d[y][x].t = CRATE;
                } else {
                    d[y][x].t = GRASS;
                }
            }
            d[y][x].type = normal;
            d[y][x].hasPowerUp = 0;
            d[y][x].lastTile = d[y][x].t;
        }
    }
}

void destroyMap (mapTile** d)
{
    int i;
    for (i = 0; i < NTILEY; i++) {
        free (d[i]);
    }
    free (d);
}

tile_t checkType (mapTile** d, int x, int y)
{
    if (y < SCREEN_HEIGHT && x < SCREEN_WIDTH) {
        x = x_to_tx (x);
        y = y_to_ty (y);
        return d[y][x].t;
    } else {
        return END;
    }
}

tile_t changeTileWithOwner(mapTile** d, int x, int y, tile_t tile, char owner, fruitType type) {
	set_db(d, x, y);
	x = x_to_tx(x);
    y = y_to_ty(y);
    if (tile != FRUIT)
    	d[y][x].lastTile = tile;
    if (tile == FRUIT && d[y][x].t == BLOCK)
    	d[y][x].lastTile = BLOCK;
    if (tile == FRUIT && d[y][x].t == CRATE)
    	d[y][x].lastTile = CRATE;
    tile_t temp = d[y][x].t;
    d[y][x].t = tile;
    d[y][x].db = 2;
    d[y][x].owner = owner;
    d[y][x].type = type;
    return temp;
}

void tilePowerUp(mapTile** d, int x, int y, int on) {
	x = x_to_tx(x);
    y = y_to_ty(y);
    d[y][x].hasPowerUp = on;
    //printf("%d,%d has power up\n", x,y);
}

tile_t changeTile(mapTile** d, int x, int y, tile_t tile) {
    return changeTileWithOwner(d, x, y, tile, -1, normal);
}

char x_to_tx (int x)
{
    return x / TILE_SIZE;
}

char y_to_ty (int y)
{
    return y / TILE_SIZE;
}

void renderMap (mapTile** d, alt_up_pixel_buffer_dma_dev *pixel_buffer)
{
    int i, j, x, y;
    for (j = 0; j < NTILEY; j++)
    {
        for (i = 0; i < NTILEX; i++) {
            x = i * TILE_SIZE;
            y = j * TILE_SIZE;
            if (d[j][i].db > 0) {
                switch(d[j][i].t){
                    case GRASS:
                    //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x07c0,1);
                    draw_screen_from_bmp(pixel_buffer, booted_bmps, 1, x, y);
                    break;
                    case FRUIT:
                    //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0xF00F,1);
                    if (d[j][i].type == orange)
                    	switch (d[j][i].lastTile) {
                    	case(BLOCK):
							draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->OWALL_pixel_map, x, y);
                    		break;
                    	case(CRATE):
							draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->OCRATE_pixel_map, x, y);
                    		break;
                    	default:
                    		draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->OGRASS_pixel_map, x, y);
                    		break;
                    	}
                    else if (d[j][i].type == watermelon)
                    	switch (d[j][i].lastTile) {
                    	case(BLOCK):
							draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->WWALL_pixel_map, x, y);
                    		break;
                    	case(CRATE):
							draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->WCRATE_pixel_map, x, y);
                    		break;
                    	default:
                    		draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->WGRASS_pixel_map, x, y);
                    		break;
                    	}
                    else if (d[j][i].type == banana)
                    	switch (d[j][i].lastTile) {
                    	case(BLOCK):
							draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->BWALL_pixel_map, x, y);
                    		break;
                    	case(CRATE):
							draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->BCRATE_pixel_map, x, y);
                    		break;
                    	default:
                    		draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->BGRASS_pixel_map, x, y);
                    		break;
                    	}
                    else if (d[j][i].type == cherry)
                    	switch (d[j][i].lastTile) {
                    	case(BLOCK):
							draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->CWALL_pixel_map, x, y);
                    		break;
                    	case(CRATE):
							draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->CCRATE_pixel_map, x, y);
                    		break;
                    	default:
                    		draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->CGRASS_pixel_map, x, y);
                    		break;
                    	}
                    break;
                    case EXPLOSION:
                    //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x2900,1);
                        if (d[j][i].type == orange)
                        	draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->OX_pixel_map, x, y);
                        else if (d[j][i].type == watermelon)
                        	draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->WX_pixel_map, x, y);
                        else if (d[j][i].type == banana)
                        	draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->BX_pixel_map, x, y);
                        else if (d[j][i].type == cherry)
                        	draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->CX_pixel_map, x, y);
                    break;
                    case CRATE:
                    //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x0502,1);
                    draw_screen_from_bmp(pixel_buffer, booted_bmps, 0, x, y);
                    break;
                    case BLOCK:
                    //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x2010,1);
                    draw_screen_from_bmp(pixel_buffer, booted_bmps, 5, x, y);
                    break;
                    case END:
                    //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x2010,1);
                    draw_screen_from_bmp(pixel_buffer, booted_bmps, 5, x, y);
                    break;
                    case POWERUP_FRUITS:
                    //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0xF000,1);
                    draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->PLUS_pixel_map, x, y);
                    break;
                    case POWERUP_RADIUS:
                    //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0xF00F,1);
                    draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->RAD_pixel_map, x, y);
                    break;
                    case POWERUP_KICK:
                    //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0xFF00,1);
                    draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->KICK_pixel_map, x, y);
                    break;
                    case POWERUP_TOSS:
                    //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x0FF0,1);
                    draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->GLVE_pixel_map, x, y);
                    break;
                    case POWERUP_INVINCIBLE:
                    //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x00FF,1);
                    draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->STAR_pixel_map, x, y);
                    break;
                    case POWERUP_BULLDOZER:
                    //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x000F,1);
                    draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->BULL_pixel_map, x, y);
                    break;
                    case COLLECTABLE_1:
                    //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x0000,1);
                    draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->BRNZ_pixel_map, x, y);
                    break;
                    case COLLECTABLE_2:
                    //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0xF0F0,1);
                    draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->SILV_pixel_map, x, y);
                    break;
                    case COLLECTABLE_3:
                    //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0xFFFF,1);
                    draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->GOLD_pixel_map, x, y);
                    break;
                }
                d[j][i].db--;
            }
        }
    }
}

void debugPrint (mapTile** d)
{
    int y;
    for (y = 0; y < NTILEY; y++) {
        int x;
        for (x = 0; x < NTILEX; x++) {
            printf("%i", d[y][x].t);
        }
        printf("\n");
    }
}

void set_db(mapTile** d, int x, int y) {
    x = x_to_tx(x);
    y = y_to_ty(y);
    d[y][x].db = 2;
}
