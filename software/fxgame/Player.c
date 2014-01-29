
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

}

char checkCollision (Player* p)
{
	//check all 4 corners and check what the tile type is, how do I access the map??
}
