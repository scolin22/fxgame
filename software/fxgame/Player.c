
#include "Player.h"
#include "Map.h"

void handleEvents (Player* p, char switches)
{
    p->velX = 0;
    p->velY = 0;
    if (keyboard[get_ascii_code_index(p->rightKey)].pressed) {
        p->velX += VELOCITY;
    }
    if (keyboard[get_ascii_code_index(p->leftKey)].pressed) {
        p->velX -= VELOCITY;
    }
    if (keyboard[get_ascii_code_index(p->upKey)].pressed) {
        p->velY -= VELOCITY;
    }
    if (keyboard[get_ascii_code_index(p->downKey)].pressed) {
        p->velY += VELOCITY;
    }
    if (keyboard[get_ascii_code_index(p->fruitKey)].pressed) {
        p->dropBomb = 1;
    }
}

void move (Player* p, mapTile** map, FruitCtrl* fruitCtrl)
{
    int tempx = p->posX;
    int tempy = p->posY;
    p->posX += p->velX;
    if (checkCollision(p, map, (p->velX > 0 ? 1 : 0), 0, 1)) {
        p->posX -= p->velX;
    }

    p->posY += p->velY;
    if (checkCollision(p, map, 0, (p->velY > 0 ? 1 : 0), 0)) {
        p->posY -= p->velY;
    }

    if (p->dropBomb == 1) {
        dropFruit(fruitCtrl, p->id, p->posX, p->posY);
        p->dropBomb = 0;
    }
    set_db(map, tempx, tempy);
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

char checkCollision (Player* p, mapTile** map, int isRight, int isDown, int isHoriz)
{
    int x = p->posX;
    int y = p->posY;
    int height = p->height;
    int width = p->width;

    if (x < 0 || y < 0 || (x+width) >= SCREEN_WIDTH || (y+height) >= SCREEN_HEIGHT) {
        return 1;
    }
    tile_t tile1, tile2;
    if (isHoriz) {
        tile1 = checkType(map, x + isRight*width, y);
        if (tile1 == FRUIT && !isRight)
            tile1 = checkType(map, x - (width-1), y);
        tile2 = checkType(map, x + isRight*width, y + height);
        if (tile2 == FRUIT && !isRight)
            tile2 = checkType(map, x - (width-1), y + height);
    } else {
        tile1 = checkType(map, x, y + isDown*height);
        if (tile1 == FRUIT && !isDown)
            tile1 = checkType(map, x, y - (height-1));
        tile2 = checkType(map, x + width, y + isDown*height);
        if (tile2 == FRUIT && !isDown)
            tile2 = checkType(map, x + width, y - (height-1));
    }

    if ((tile1 == EXPLOSION || tile2 == EXPLOSION) && p->respawnTime == 0) {
        p->respawnTime = RESPAWN_TIME;
        p->lives--;
        return 0;
    } else if (tile1 == EXPLOSION || tile2 == EXPLOSION) {
        return 0;
    } else if (tile1 != GRASS || tile2 != GRASS) {
        return 1;
    }
    return 0;
}
