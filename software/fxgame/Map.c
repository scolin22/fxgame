#include <stdlib.h>
#include <stdio.h>

#include "Map.h"

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
                        //TODO COLIN: green box
                        //draw a grass at ,
                        //GRASS == 0
                        //TODO COLIN: create a function that retrieves the pixel_map_16 for grass sprite
                        //draw this pixel_map_16 at x,y
                        //draw_bmp()
                        // alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x0007c0,1);
                    draw_screen_from_bmp(pixel_buffer, booted_bmps, 1, x, y);
                    break;
                    case FRUIT:
                        //TODO COLIN: red box
                        //draw a FRUIT at x = i*TILE_SIZE, y = j*TILE_SIZE
                        //FRUIT == 1
                        //TODO COLIN: create a function that retrieves the pixel_map_16 for FRUIT sprite
                        //draw this pixel_map_16 at x,y
                        // alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x1F0000,1);
                    draw_screen_from_bmp(pixel_buffer, booted_bmps, 2, x, y);
                    break;
                    case EXPLOSION:
                        //TODO COLIN: orange box
                        //draw a EXPLOSION at x = i*TILE_SIZE, y = j*TILE_SIZE
                        //EXPLOSION == 2
                        //TODO COLIN: create a function that retrieves the pixel_map_16 for EXPLOSION sprite
                        //draw this pixel_map_16 at x,y
                        // alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x1F2900,1);
                    draw_screen_from_bmp(pixel_buffer, booted_bmps, 2, x, y);
                    break;
                    case CRATE:
                        //TODO COLIN: brown box
                        //draw a CRATE at x = i*TILE_SIZE, y = j*TILE_SIZE
                        //CRATE == 3
                        //TODO COLIN: create a function that retrieves the pixel_map_16 for CRATE sprite
                        //draw this pixel_map_16 at x,y
                        // alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x0A0502,1);
                    draw_screen_from_bmp(pixel_buffer, booted_bmps, 0, x, y);
                    break;
                    case BLOCK:
                        //TODO COLIN: grey box
                        //draw a BLOCK at x = i*TILE_SIZE, y = j*TILE_SIZE
                        //BLOCK == 4
                        //TODO COLIN: create a function that retrieves the pixel_map_16 for BLOCK sprite
                        //draw this pixel_map_16 at x,y
                        // alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0x102010,1);
                    draw_screen_from_bmp(pixel_buffer, booted_bmps, 5, x, y);
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
