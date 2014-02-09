#include "Player.h"

Players *players;

void handleEvents (Player* p)
{
    checkCollision(p, none);
}

//void move (Player* p)
//{
//    set_db(map, p->posX, p->posY);
//    map[p->posY][p->posX].playerOn = 0;
//    if (keyboard[get_ascii_code_index(p->fruitKey)].pressed == 1) {
//        printf("dropping bomb!\n");
//        dropFruit(p->fruitCtrl, p->id, checkPowerUps(p, toss), p->dir, p->posX, p->posY);
//        return;
//    }
//    else if (keyboard[get_ascii_code_index(p->rightKey)].pressed == 1) {
//        p->posX += TILE_SIZE;
//        p->dir = right;
//        if (checkCollision(p, map, right))
//            p->posX -= TILE_SIZE;
//    }
//    else if (keyboard[get_ascii_code_index(p->leftKey)].pressed == 1) {
//        p->posX -= TILE_SIZE;
//        p->dir = left;
//        if (checkCollision(p, map, left))
//            p->posX += TILE_SIZE;
//    }
//    else if (keyboard[get_ascii_code_index(p->upKey)].pressed == 1) {
//        p->posY -= TILE_SIZE;
//        p->dir = up;
//        if (checkCollision(p, map, up))
//            p->posY += TILE_SIZE;
//    }
//    else if (keyboard[get_ascii_code_index(p->downKey)].pressed == 1) {
//        p->posY += TILE_SIZE;
//        p->dir = down;
//        if (checkCollision(p, map, down))
//            p->posY -= TILE_SIZE;
//    }
//    map[p->posY][p->posX].playerOn = 1;
//}

void movePress (Player* p, char ascii) {

    set_db(p->map, p->posX, p->posY);
    p->map[y_to_ty(p->posY)][x_to_tx(p->posX)].playerOn = 0;
    if (p->fruitKey == ascii && (p->respawnTime <= 0 || checkPowerUps(p, invincible))) {
        dropFruit(p->fruitCtrl, p->id, checkPowerUps(p, toss), p->dir, p->posX, p->posY);
        return;
    }
    else if (p->rightKey == ascii) {
        p->posX += TILE_SIZE;
        p->dir = right;
        if (checkCollision(p, right))
            p->posX -= TILE_SIZE;
    }
    else if (p->leftKey == ascii) {
        p->posX -= TILE_SIZE;
        p->dir = left;
        if (checkCollision(p, left))
            p->posX += TILE_SIZE;
    }
    else if (p->upKey == ascii) {
        p->posY -= TILE_SIZE;
        p->dir = up;
        if (checkCollision(p, up))
            p->posY += TILE_SIZE;
    }
    else if (p->downKey == ascii) {
        p->posY += TILE_SIZE;
        p->dir = down;
        if (checkCollision(p, down))
            p->posY -= TILE_SIZE;
    }
    p->map[y_to_ty(p->posY)][x_to_tx(p->posX)].playerOn = 1;
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
        }
        else {
            //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0x003F,1);
            draw_screen_from_bmp(pixel_buffer, booted_bmps, 3, x, y);
        }
    else if (p->respawnTime % 2 == 1){
        	// alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0x003F00,1);
    }
    else if (p->respawnTime % 2 == 0){
        if(p->id == 0) {
            //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0xFFFF,1);
            draw_screen_from_bmp(pixel_buffer, booted_bmps, 4, x, y);
        }
        else {
            //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0x003F,1);
            draw_screen_from_bmp(pixel_buffer, booted_bmps, 3, x, y);
        }
    }
}

void updatePlayer(Player* p)
{
    if(p->respawnTime > 0)
        p->respawnTime--;
    else if (checkPowerUps(p, invincible))
    	togglePowerUp(p, invincible);
}

char checkCollision (Player* p, direction dir)
{
	mapTile** map = p->map;
    tile_t tile = checkType(map, p->posX, p->posY);
    if (p->posX < 0 || p->posY < 0 || (p->posX+p->width) >= SCREEN_WIDTH || (p->posY+p->height) >= SCREEN_HEIGHT) {
        return 1;
    } else if (tile == EXPLOSION && p->respawnTime == 0) {
        p->respawnTime = RESPAWN_TIME;
        if(map[y_to_ty(p->posY)][x_to_tx(p->posX)].owner != p->id)
        	players->list[map[y_to_ty(p->posY)][x_to_tx(p->posX)].owner]->score += 100;
        p->score -= 100;
        return 0;
    } else if (tile == EXPLOSION) {
        return 0;
    } else if (tile == FRUIT && checkPowerUps(p,kick)) {
        Fruit* fruit = checkForFruitAtPosition(p->fruitCtrl, p->posX, p->posY);
        if(dir == right)
            fruit->velX = 1;
        else if(dir == left)
            fruit->velX = -1;
        else if(dir == up)
            fruit->velY = -1;
        else if(dir == down)
            fruit->velY = 1;
        return 1;
    } else if (tile == POWERUP_FRUITS) {
        if(p->fruitCtrl->maxFruits[p->id] < FRUITS_PER_PLAYER)
            p->fruitCtrl->maxFruits[p->id]++;
        changeTile(map, p->posX, p->posY, GRASS);
    } else if (tile == POWERUP_RADIUS) {
        increaseFruitRadius(p->fruitCtrl, p->id);
        changeTile(map, p->posX, p->posY, GRASS);
    } else if (tile == POWERUP_KICK) {
        setPowerUps(p, kick);
        changeTile(map, p->posX, p->posY, GRASS);
    } else if (tile == POWERUP_TOSS) {
        setPowerUps(p, toss);
        changeTile(map, p->posX, p->posY, GRASS);
    } else if (tile == POWERUP_INVINCIBLE) {
        setPowerUps(p, invincible);
        p->respawnTime = RESPAWN_TIME*5;
        changeTile(map, p->posX, p->posY, GRASS);
    } else if (tile == POWERUP_BULLDOZER) {
        setPowerUps(p, bulldozer);
        changeTile(map, p->posX, p->posY, GRASS);
    } else if ((tile == BLOCK || tile == CRATE ) && checkPowerUps(p,bulldozer)) {
    	togglePowerUp(p, bulldozer);
    	changeTile(map, p->posX, p->posY, GRASS);
        return 0;
    } else if (tile == BLOCK || tile == CRATE || tile == FRUIT || tile == END) {
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

void togglePowerUp(Player *p, powerUps pwrUp) {
    p->pwrUps = p->pwrUps ^ pwrUp;
}

