#include "Player.h"

Player* p1;
Player* p2;

void handleEvents (Player* p)
{
	checkCollision(p, map, none);
}

void move (Player* p)
{
    int tempx = p->posX;
    int tempy = p->posY;
    p->posX += p->velX;
    if (checkCollision(p, map, none)) {
        p->posX -= p->velX;
    }

    p->posY += p->velY;
    if (checkCollision(p, map, none)) {
        p->posY -= p->velY;
    }
    set_db(map, tempx, tempy);
}

void movePress (Player* p, char ascii) {

	set_db(map, p->posX, p->posY);
	map[p->posY][p->posX].playerOn = 0;
    if (ascii_codes[get_ascii_code_index(p->fruitKey)] == ascii) {
    	printf("dropping bomb!\n");
        dropFruit(p->fruitCtrl, p->id, 1/*checkPowerUps(p, toss)*/, p->dir, p->posX, p->posY);
        return;
    }
    else if (ascii_codes[get_ascii_code_index(p->rightKey)] == ascii) {
        p->posX += TILE_SIZE;
        p->dir = right;
        if (checkCollision(p, map, right))
            p->posX -= TILE_SIZE;
    }
    else if (ascii_codes[get_ascii_code_index(p->leftKey)] == ascii) {
        p->posX -= TILE_SIZE;
        p->dir = left;
        if (checkCollision(p, map, left))
            p->posX += TILE_SIZE;
    }
    else if (ascii_codes[get_ascii_code_index(p->upKey)] == ascii) {
        p->posY -= TILE_SIZE;
        p->dir = up;
        if (checkCollision(p, map, up))
            p->posY += TILE_SIZE;
    }
    else if (ascii_codes[get_ascii_code_index(p->downKey)] == ascii) {
        p->posY += TILE_SIZE;
        p->dir = down;
        if (checkCollision(p, map, down))
            p->posY -= TILE_SIZE;
    }
    map[p->posY][p->posX].playerOn = 1;
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
            alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0xFFFF,1);
            //draw_screen_from_bmp(pixel_buffer, booted_bmps, 4, x, y);
        }
        else {
            alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0x003F,1);
            //draw_screen_from_bmp(pixel_buffer, booted_bmps, 3, x, y);
        }
    else if (p->respawnTime % 2 == 1){
        // alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0x003F00,1);
    }
    else if (p->respawnTime % 2 == 0){
        if(p->id == 0) {
            alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0xFFFF,1);
            //draw_screen_from_bmp(pixel_buffer, booted_bmps, 4, x, y);
        }
        else {
            alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0x003F,1);
            //draw_screen_from_bmp(pixel_buffer, booted_bmps, 3, x, y);
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
	tile_t mapTile = checkType(map, p->posX, p->posY);
    if (p->posX < 0 || p->posY < 0 || (p->posX+p->width) >= SCREEN_WIDTH || (p->posY+p->height) >= SCREEN_HEIGHT) {
        return 1;
    } else if (mapTile == EXPLOSION && p->respawnTime == 0) {
        p->respawnTime = RESPAWN_TIME;
        p->lives--;
        return 0;
    } else if (mapTile == EXPLOSION) {
        return 0;
    } else if (mapTile == FRUIT /*&& checkPowerUps(p,kick)*/) {
    	Fruit* fruit = checkForFruitAtPosition(p->posX, p->posY);
    	if(dir == right)
    		fruit->velX = 1;
    	else if(dir == left)
    		fruit->velX = -1;
    	else if(dir == up)
    		fruit->velY = -1;
    	else if(dir == down)
    		fruit->velY = 1;
    	return 1;
    } else if (mapTile == BLOCK || mapTile == CRATE || mapTile == FRUIT || mapTile == END) {
        return 1;
    }
    return 0;
}

char checkPowerUps(Player *p, powerUps pwrUp) {
	if (!(p->pwrUps & pwrUp))
		return 0;
	return 1;
}

void setPowerUps(Player *p, powerUps pwrUp) {
	p->pwrUps = p->pwrUps | pwrUp;
}

