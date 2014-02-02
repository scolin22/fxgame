
#include "Player.h"
#include "Map.h"

void handleEvents (Player* p, char switches)
{
    p->velX = 0;
    p->velY = 0;
    if (switches & 1) {
        p->velX = TILE_SIZE;
    }
    if (switches & 2) {
        p->velY = -TILE_SIZE;
    }
    if (switches & 4) {
        p->velY = TILE_SIZE;
    }
    if (switches & 8) {
        p->velX = -TILE_SIZE;
    }
}

void move (Player* p, char** map)
{
	p->posX += p->velX;
	if (checkCollision(p, map)) {
		p->posX -= p->velX;
	}

	p->posY += p->velY;
	if (checkCollision(p, map)) {
		p->posY -= p->velY;
	}
}

void renderPlayer (Player* p, alt_up_pixel_buffer_dma_dev *pixel_buffer)
{
    ///TODO COLIN: green box
    int x = p->posX;
    int y = p->posY;
    int h = p->height;
    int w = p->width;
    //TODO COLIN: create a function that retrieves the pixel_map_16 for grass sprite for this p->id;
    //draw this pixel_map_16 at x,y
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0xFFFF,1);
}

char checkCollision (Player* p, char** map)
{
    if (p->posX < 0 || p->posY < 0 || (p->posX+p->width) >= SCREEN_WIDTH || (p->posY+p->height) >= SCREEN_HEIGHT) {
        return 1;
    }
    return 0;
}
