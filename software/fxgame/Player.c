
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
    //draw a player at x = p->posX, y = p->posY
    //TODO COLIN: create a function that retrieves the pixel_map_16 for grass sprite for this p->id;
    //draw this pixel_map_16 at x,y
}

char checkCollision (Player* p)
{
	//check all 4 corners and check what the tile type is, how do I access the map??
}
