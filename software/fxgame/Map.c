#include <stdlib.h>
#include <stdio.h>

#include "Map.h"

mapTile** map;

mapTile** initMap ()
{
    mapTile** d = (mapTile**)malloc(NTILEY*sizeof(mapTile*));
    int i;
    for (i = 0; i < NTILEY; i++) {
        d[i] = (mapTile*)malloc(NTILEX*sizeof(mapTile));
    }

    int y;
    for (y = 0; y < NTILEY; y++) {
        int x;
        for (x = 0; x < NTILEX; x++) {
            d[y][x].t = maplayout[y*NTILEX+x] - '0';
            d[y][x].db = 2;
        }
    }

    return d;
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

tile_t changeTile(mapTile** d, int x, int y, tile_t tile) {
    x = x_to_tx(x);
    y = y_to_ty(y);
    tile_t temp = d[y][x].t;
    d[y][x].t = tile;
    d[y][x].db = 2;
    return temp;
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
                    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x07c0,1);
                    //draw_screen_from_bmp(pixel_buffer, booted_bmps, 1, x, y);
                    break;
                    case FRUIT:
                    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0xF00F,1);
                    //draw_screen_from_bmp(pixel_buffer, booted_bmps, 2, x, y);
                    break;
                    case EXPLOSION:
                    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x2900,1);
                    //draw_screen_from_bmp(pixel_buffer, booted_bmps, 6, x, y);
                    break;
                    case CRATE:
                    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x0502,1);
                    //draw_screen_from_bmp(pixel_buffer, booted_bmps, 0, x, y);
                    break;
                    case BLOCK:
                    // alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x2010,1);
                    draw_screen_from_bmp(pixel_buffer, booted_bmps, 5, x, y);
                    break;
                    case END:
                    // alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x2010,1);
                    draw_screen_from_bmp(pixel_buffer, booted_bmps, 5, x, y);
                    break;
                    case POWERUP_FRUITS:
                    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0xF000,1);
                    break;
                    case POWERUP_RADIUS:
                    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0xF00F,1);
                    break;
                    case POWERUP_KICK:
                    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0xFF00,1);
                    break;
                    case POWERUP_TOSS:
                    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x0FF0,1);
                    break;
                    case POWERUP_INVINCIBLE:
                    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x00FF,1);
                    break;
                    case POWERUP_BULLDOZER:
                    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x000F,1);
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
