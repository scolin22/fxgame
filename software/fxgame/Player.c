
#include "Player.h"
#include "Map.h"

void handleEvents (Player* p, char switches)
{
    p->velX = 0;
    p->velY = 0;
    if (keyboard[get_ascii_code_index(p->rightKey)].pressed || (switches & 1) > 0) {
        p->velX += TILE_SIZE;
    }
    if (keyboard[get_ascii_code_index(p->leftKey)].pressed || (switches & 8) > 0) {
        p->velX -= TILE_SIZE;
    }
    if (keyboard[get_ascii_code_index(p->upKey)].pressed || (switches & 4) > 0) {
        p->velY -=TILE_SIZE;
    }
    if (keyboard[get_ascii_code_index(p->downKey)].pressed || (switches & 2) > 0) {
        p->velY += TILE_SIZE;
    }
    if (keyboard[get_ascii_code_index(p->fruitKey)].pressed || (switches & 16) > 0) {
        p->dropBomb = 1;
    }
}

void move (Player* p, mapTile** map, FruitCtrl* fruitCtrl)
{
    int tempx = p->posX;
    int tempy = p->posY;
    p->posX += p->velX;
    if (checkCollision(p, map)) {
        p->posX -= p->velX;
    }

    p->posY += p->velY;
    if (checkCollision(p, map)) {
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
            alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0xFFFF,1);
            //draw_screen_from_bmp(pixel_buffer, booted_bmps, 4, x, y);
            // draw_screen_from_bmp(pixel_buffer, booted_bmps, 1, oldx, oldy);
        }
        else {
            alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0x003F,1);
            //draw_screen_from_bmp(pixel_buffer, booted_bmps, 3, x, y);
            // draw_screen_from_bmp(pixel_buffer, booted_bmps, 1, oldx, oldy);
        }
    else if (p->respawnTime % 2 == 1){
        // alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0x003F00,1);
        // draw_screen_from_bmp(pixel_buffer, booted_bmps, 1, oldx, oldy);
    }
    else if (p->respawnTime % 2 == 0){
        if(p->id == 0) {
            alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0xFFFF,1);
            //draw_screen_from_bmp(pixel_buffer, booted_bmps, 4, x, y);
            // draw_screen_from_bmp(pixel_buffer, booted_bmps, 1, oldx, oldy);
        }
        else {
            alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0x003F,1);
            //draw_screen_from_bmp(pixel_buffer, booted_bmps, 3, x, y);
            // draw_screen_from_bmp(pixel_buffer, booted_bmps, 1, oldx, oldy);
        }
    }
}

void updatePlayer(Player* p)
{
    if(p->respawnTime > 0)
        p->respawnTime--;
}

char checkCollision (Player* p, mapTile** map)
{
    if (p->posX < 0 || p->posY < 0 || (p->posX+p->width) >= SCREEN_WIDTH || (p->posY+p->height) >= SCREEN_HEIGHT) {
        return 1;
    } else if (checkType(map, p->posX, p->posY) == EXPLOSION && p->respawnTime == 0) {
        p->respawnTime = RESPAWN_TIME;
        p->lives--;
        return 0;
    } else if (checkType(map, p->posX, p->posY) == EXPLOSION) {
        return 0;
    } else if (checkType(map, p->posX, p->posY) != GRASS) {
        return 1;
    }
    return 0;
}
