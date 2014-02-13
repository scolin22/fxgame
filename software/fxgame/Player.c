#include "Player.h"

Players *players;

void resetPlayer1(Player* p) {
    p->posX = 16;
    p->posY = 16;
    p->height = TILE_SIZE-2;
    p->width = TILE_SIZE-2;
    p->respawnTime = 0;
    p->stunnedTime = 0;
    p->dropBomb = 0;
    p->id = 0;
    p->leftKey = 'A';
    p->rightKey = 'D';
    p->upKey = 'W';
    p->downKey = 'S';
    p->fruitKey = 'Q';
    p->velX = 0;
    p->velY = 0;
    p->dir = left;
    p->pwrUps = 0;
    p->bullCount = 0;
}

void resetPlayer2(Player* p){
    p->posX = 16+TILE_SIZE*17;
    p->posY = 16+TILE_SIZE*12;
    p->height = TILE_SIZE-2;
    p->width = TILE_SIZE-2;
    p->dropBomb = 0;
    p->respawnTime = 0;
    p->stunnedTime = 0;
    p->id = 1;
    p->leftKey = '4';
    p->rightKey = '6';
    p->upKey = '8';
    p->downKey = '5';
    p->fruitKey = '7';
    p->velX = 0;
    p->velY = 0;
    p->dir = left;
    p->pwrUps = 0;
    p->bullCount = 0;
}

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
    printf("Player is walking on %d\n", checkType(p->map, p->posX, p->posY));
}


void chooseFruitForPlayer(Player* p, FruitCtrl* fruitCtrl, fruitType type) {
    p->type = type;
    fruitCtrl->types[p->id] = type;
    if (type == cherry)
        fruitCtrl->maxFruits[p->id]++;
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
    if(p->respawnTime % 2 == 0)
        if(p->type == banana) {
        	switch (p->dir) {
        	case (left):
				draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->BLEFT_pixel_map, x, y);
        		break;
        	case (right):
				draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->BRIGHT_pixel_map, x, y);
        		break;
        	case (down):
				draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->BDOWN_pixel_map, x, y);
        		break;
        	case (up):
				draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->BUP_pixel_map, x, y);
        		break;
        	}
        }
        else if (p->type == orange){
        	switch (p->dir) {
        	case (left):
				draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->OLEFT_pixel_map, x, y);
        		break;
        	case (right):
				draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->ORIGHT_pixel_map, x, y);
        		break;
        	case (down):
				draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->ODOWN_pixel_map, x, y);
        		break;
        	case (up):
				draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->OUP_pixel_map, x, y);
        		break;
        	}
        }
        else if (p->type == watermelon){
        	switch (p->dir) {
        	case (left):
				draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->WLEFT_pixel_map, x, y);
        		break;
        	case (right):
				draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->WRIGHT_pixel_map, x, y);
        		break;
        	case (down):
				draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->WDOWN_pixel_map, x, y);
        		break;
        	case (up):
				draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->WUP_pixel_map, x, y);
        		break;
        	}
        }
        else if (p->type == cherry){
        	switch (p->dir) {
        	case (left):
				draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->CLEFT_pixel_map, x, y);
        		break;
        	case (right):
				draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->CRIGHT_pixel_map, x, y);
        		break;
        	case (down):
				draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->CDOWN_pixel_map, x, y);
        		break;
        	case (up):
				draw_screen_from_pixel_map_16(pixel_buffer,  booted_bmps->CUP_pixel_map, x, y);
        		break;
        	}
        }
    else if (p->respawnTime % 2 == 1){
        	// alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0x003F00,1);
    }
    else if (p->respawnTime % 2 == 0){

    }
}

void updatePlayer(Player* p)
{
    if(p->respawnTime > 0)
        p->respawnTime--;
    else if (checkPowerUps(p, invincible))
    	togglePowerUp(p, invincible);
    if(p->stunnedTime > 0)
    	p->stunnedTime--;
}

char checkCollision (Player* p, direction dir)
{
	mapTile** map = p->map;
    tile_t tile = checkType(map, p->posX, p->posY);
	if (tile == END) {
    	return 1;
	} else if (p->posX < 0 || p->posY < 0 || (p->posX+p->width) >= SCREEN_WIDTH || (p->posY+p->height) >= SCREEN_HEIGHT) {
        return 1;
    } else if (tile == EXPLOSION && p->respawnTime == 0) {
        p->respawnTime = RESPAWN_TIME;
        if( map[y_to_ty(p->posY)][x_to_tx(p->posX)].type == banana)
            p->stunnedTime = STUNNED_TIME;
        if(map[y_to_ty(p->posY)][x_to_tx(p->posX)].owner != p->id)
        	*(players->list[map[y_to_ty(p->posY)][x_to_tx(p->posX)].owner]->score) += 200;
        *(p->score) -= 200;
        return 0;
    } else if (tile == EXPLOSION) {
        return 0;
    } else if (tile == COLLECTABLE_1) {
    	*(p->score) += 50;
    	changeTile(map, p->posX, p->posY, GRASS);
        return 0;
    } else if (tile == COLLECTABLE_2) {
    	*(p->score) += 200;
    	changeTile(map, p->posX, p->posY, GRASS);
        return 0;
    } else if (tile == COLLECTABLE_3) {
    	*(p->score) += 500;
    	changeTile(map, p->posX, p->posY, GRASS);
        return 0;
    } else if (tile == FRUIT && checkPowerUps(p,kick)) {
        Fruit* fruit = checkForFruitAtPosition(p->fruitCtrl, p->posX, p->posY);
        if (fruit->tileOn == BLOCK)
        	return 1;
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
    	*(p->score) += 50;
    	increaseFruitCount(p->fruitCtrl, p->id);
        changeTile(map, p->posX, p->posY, GRASS);
    } else if (tile == POWERUP_RADIUS) {
    	*(p->score) += 50;
        increaseFruitRadius(p->fruitCtrl, p->id);
        changeTile(map, p->posX, p->posY, GRASS);
    } else if (tile == POWERUP_KICK) {
    	*(p->score) += 50;
        setPowerUps(p, kick);
        changeTile(map, p->posX, p->posY, GRASS);
    } else if (tile == POWERUP_TOSS) {
    	*(p->score) += 50;
        setPowerUps(p, toss);
        changeTile(map, p->posX, p->posY, GRASS);
    } else if (tile == POWERUP_INVINCIBLE) {
    	*(p->score) += 50;
        setPowerUps(p, invincible);
        p->respawnTime = RESPAWN_TIME*5;
        changeTile(map, p->posX, p->posY, GRASS);
    } else if (tile == POWERUP_BULLDOZER) {
    	*(p->score) += 50;
        setPowerUps(p, bulldozer);
        p->bullCount++;
        changeTile(map, p->posX, p->posY, GRASS);
    } else if ((tile == BLOCK || tile == CRATE ) && checkPowerUps(p,bulldozer)) {
    	p->bullCount--;
    	if(p->bullCount <= 0)
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

