#include "Player.h"

Player* p1;
Player* p2;

#include "Map.h"

void handleEvents (Player* p)
{
	checkCollision(p, p->map, none);
}

void move (Player* p)
{
    int tempx = p->posX;
    int tempy = p->posY;
    p->posX += p->velX;
    if (checkCollision(p, p->map, none)) {
        p->posX -= p->velX;
    }

    p->posY += p->velY;
    if (checkCollision(p, p->map, none)) {
        p->posY -= p->velY;
    }
    set_db(p->map, tempx, tempy);
}

void movePress (Player* p, char ascii) {

	 set_db(p->map, p->posX, p->posY);
    if (p->fruitKey == ascii) {
        dropFruit(p->fruitCtrl, p->id, p->posX, p->posY);
        return;
    }
    else if (p->rightKey == ascii) {
        p->posX += TILE_SIZE;
        if (checkCollision(p, p->map, right))
            p->posX -= TILE_SIZE;
    }
    else if (p->leftKey == ascii) {
        p->posX -= TILE_SIZE;
        if (checkCollision(p, p->map, left))
            p->posX += TILE_SIZE;
    }
    else if (p->upKey == ascii) {
        p->posY -= TILE_SIZE;
        if (checkCollision(p, p->map, up))
            p->posY += TILE_SIZE;
    }
    else if (p->downKey == ascii) {
        p->posY += TILE_SIZE;
        if (checkCollision(p, p->map, down))
            p->posY -= TILE_SIZE;
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
    if(!p->respawnTime)
        if(p->id == 0) {
            //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0xFFFF,1);
            draw_screen_from_bmp(pixel_buffer, booted_bmps, 4, x, y);
            // draw_screen_from_bmp(pixel_buffer, booted_bmps, 1, oldx, oldy);
        }
        else {
            //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0x003F,1);
            draw_screen_from_bmp(pixel_buffer, booted_bmps, 3, x, y);
            // draw_screen_from_bmp(pixel_buffer, booted_bmps, 1, oldx, oldy);
        }
    else if (p->respawnTime % 2 == 1){
        // alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0x003F00,1);
        // draw_screen_from_bmp(pixel_buffer, booted_bmps, 1, oldx, oldy);
    }
    else if (p->respawnTime % 2 == 0){
        if(p->id == 0) {
            //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0xFFFF,1);
            draw_screen_from_bmp(pixel_buffer, booted_bmps, 4, x, y);
            // draw_screen_from_bmp(pixel_buffer, booted_bmps, 1, oldx, oldy);
        }
        else {
            //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0x003F,1);
            draw_screen_from_bmp(pixel_buffer, booted_bmps, 3, x, y);
            // draw_screen_from_bmp(pixel_buffer, booted_bmps, 1, oldx, oldy);
        }
    }
}

void updatePlayer(Player* p)
{
    if(p->respawnTime > 0)
        p->respawnTime--;
}

char checkCollision (Player* p, mapTile** map, direction dir)
{
	tile_t currentTile = checkType(map, p->posX, p->posY);
    if (p->posX < 0 || p->posY < 0 || (p->posX+p->width) >= SCREEN_WIDTH || (p->posY+p->height) >= SCREEN_HEIGHT) {
        return 1;
    } else if (currentTile == EXPLOSION && p->respawnTime == 0) {
        p->respawnTime = RESPAWN_TIME;
        p->lives--;
        return 0;
    } else if (currentTile == EXPLOSION) {
        return 0;
    } else if (currentTile == FRUIT && dir != none) {
    	Fruit* currentFruit = checkForFruitAtPosition(p->fruitCtrl, p->posX, p->posY);
    	printf("Address of fruit is %x\n", currentFruit);
    	switch(dir) {
    	case left:
    		currentFruit->velX = -1;
    		break;
    	case right:
    		currentFruit->velX = 1;
    		break;
    	case up:
    		currentFruit->velY = -1;
    		break;
    	case down:
    		currentFruit->velY = 1;
    		break;
    	default:
    		break;
    	}
        return 1;
    } else if (checkType(map, p->posX, p->posY) != GRASS) {
        return 1;
    }
    return 0;
}
