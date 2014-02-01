
#include "Player.h"

void handleEvents (Player* p)
{

}

void move (Player* p)
{
	p->posX += p->velX;
	if (checkCollision(p)) {
		p->posX -= p->velX;
	}

	p->posY += p->velY;
	if (checkCollision(p)) {
		p->posY -= p->velY;
	}
}

void renderPlayer (Player* p)
{
    ///TODO COLIN: green box
    int x = p->posX;
    int y = p->posY;
    //TODO COLIN: create a function that retrieves the pixel_map_16 for grass sprite for this p->id;
    //draw this pixel_map_16 at x,y
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1, 0xFFFF,1);
}

char checkCollision (Player* p)
{
	//check all 4 corners and check what the tile type is, how do I access the map??
}
