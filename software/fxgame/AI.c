
#include "AI.h"
#include "Map.h"
#include "Fruits.h"
#include "Types.h"

//TODO: what if player too far, no crates????

void decide (AI* a, FruitCtrl* f, Player* p)
{
    int x = x_to_tx(a->posX);
    int y = y_to_ty(a->posY);

    preExplodeMap(a, f);

    if (a->state == IDLE) {
        if (checkSafe(a, x, y) == 0) {
            a->state = RUN_FRUIT;
        } else if (targetPlayer (a, f, p) == 1) {
            a->state = HUNT_PLAYER;
        } else {
            a->state = FIND_CRATE;
        }
        decide (a, f, p);
    } else if (a->state == RUN_FRUIT) {
        if (checkSafe (a, x, y) == 0) {
            targetClosestSafeSpot(a, f);
        } else {
            a->state = IDLE;
            decide (a, f, p);
        }
    } else if (a->state == FIND_CRATE) {
        if (targetPlayer(a, f, p) == 1) {
            a->state = HUNT_PLAYER;
        } else {
            int r = targetMostCrates(a, f, 3);
            if (r == -1) {
                a->state = IDLE;
                // Need something to do if no crates nearby
            } else if (r == 0) {
                // Keep moving to crate
            } else if (r == 1) {
                a->state = DESTORY_CRATE;
                decide(a, f, p);
            }
        }
    } else if (a->state == DESTORY_CRATE) {
        a->dropBomb = 1;
        a->bombTicks = FRUIT_TIMEOUT + EXPLOSION_TIMEOUT + 4;
        a->state = HIDE_FRUIT_CRATE;
    } else if (a->state == HIDE_FRUIT_CRATE) {
        int r = targetClosestSafeSpot(a, f);
        if (r == -1) {
            // No safe spots, accept the inevitable death
        } else if (r == 0) {
            // Keep moving to safe spot
            a->bombTicks--;
            if (a->bombTicks <= 0) {
                a->state = IDLE;
                decide(a, f, p);
            }
        } else if (r == 1) {
            a->bombTicks--;
            if (a->bombTicks <= 0) {
                a->state = IDLE;
                decide(a, f, p);
            }
        }
    } else if (a->state == HUNT_PLAYER) {
        int r = targetPlayer(a, f, p);
        if (r == -1) {
            // Player too far away
            a->state = IDLE;
        } else if (r == 0) {
            // Keep moving
        } else if (r == 1) {
            a->state = DESTORY_PLAYER;
            decide(a, f, p);
        }
    } else if (a->state == DESTORY_PLAYER) {
        a->dropBomb = 1;
        a->bombTicks = FRUIT_TIMEOUT + EXPLOSION_TIMEOUT + 4;
        a->state = HIDE_FRUIT_PLAYER;
    } else if (a->state == HIDE_FRUIT_PLAYER) {
        int r = targetClosestSafeSpot(a, f);
        if (r == -1) {
            // No safe spots, accept the inevitable death
        } else if (r == 0) {
            // Keep moving to safe spot
            a->bombTicks--;
            if (a->bombTicks <= 0) {
                a->state = IDLE;
                decide(a, f, p);
            }
        } else if (r == 1) {
            a->bombTicks--;
            if (a->bombTicks <= 0) {
                a->state = IDLE;
                decide(a, f, p);
            }
        }
    }
}

char targetPlayer (AI* a, FruitCtrl* f, Player* pl)
{
    if(abs(pl->posX - a->posX) <= 5*TILE_SIZE && abs(pl->posY - a->posY) <= 5*TILE_SIZE) {
        Path path[LONGEST_PATH*3];
        path[0].x = x_to_tx(a->posX);
        path[0].y = y_to_ty(a->posY);
        path[0].d = none;
        path[0].length = 0;
        path[0].prevIndex = -1;

        int i;
        int e;

        for (i = 0, e = 1; i < e && e < LONGEST_PATH*3; i++) {
            Path p = path[i];
            int x = p.x;
            int y = p.y;

            if (p.length > LONGEST_PATH) {
                continue;
            } else if (preExplodePlayerHit(a, x, y, pl) == 1) {
                int l = p.length - 1;
                if (l == -1) {
                    return 1;
                }
                a->next = 0;
                a->end = l + 1;
                while (l >= 0) {
                    p = path[i];
                    if (p.length - 1 != l || i == -1) {
                        printf("targetPlayer::CRITICAL AI ERROR\n");
                    }
                    a->destX[l] = p.x;
                    a->destY[l] = p.y;
                    i = p.prevIndex;
                    l--;
                }
                return 0;
            } else {
                if (e < LONGEST_PATH*3 && p.d != up && checkSafe(a, x, y-1)) {
                    path[e].d = down;
                    path[e].y = y-1;
                    path[e].x = x;
                    path[e].length = p.length+1;
                    path[e].prevIndex = i;
                    e++;
                }
                if (e < LONGEST_PATH*3 && p.d != down && checkSafe(a, x, y+1)) {
                    path[e].d = up;
                    path[e].y = y+1;
                    path[e].x = x;
                    path[e].length = p.length+1;
                    path[e].prevIndex = i;
                    e++;
                }
                if (e < LONGEST_PATH*3 && p.d != left && checkSafe(a, x-1, y)) {
                    path[e].d = right;
                    path[e].y = y;
                    path[e].x = x-1;
                    path[e].length = p.length+1;
                    path[e].prevIndex = i;
                    e++;
                }
                if (e < LONGEST_PATH*3 && p.d != right && checkSafe(a, x+1, y)) {
                    path[e].d = left;
                    path[e].y = y;
                    path[e].x = x+1;
                    path[e].length = p.length+1;
                    path[e].prevIndex = i;
                    e++;
                }
            }
        }
    }
    return -1;
}

char targetClosestSafeSpot (AI* a, FruitCtrl* f)
{
    Path path[LONGEST_PATH*3];

    path[0].x = x_to_tx(a->posX);
    path[0].y = y_to_ty(a->posY);
    path[0].d = none;
    path[0].length = 0;
    path[0].prevIndex = -1;

    int i;
    int e;

    for (i = 0, e = 1; i < e && e < LONGEST_PATH*3; i++) {
        Path p = path[i];
        int x = p.x;
        int y = p.y;

        if (p.length > LONGEST_PATH) {
            continue;
        } else if (checkSafe (a, x, y) == 1) {
            int l = p.length - 1;
            if (l == -1) {
                return 1;
            }
            a->next = 0;
            a->end = l + 1;
            while (l >= 0) {
				p = path[i];
                if (p.length - 1 != l || i == -1) {
                    printf("targetClosestSafeSpot::CRITICAL AI ERROR\n");
                }
                a->destX[l] = p.x;
                a->destY[l] = p.y;
                i = p.prevIndex;
                l--;
            }
            return 0;
        } else {
            if (e < LONGEST_PATH*3 && p.d != up && checkSteppable(a, f, x, y-1)) {
                path[e].d = down;
                path[e].y = y-1;
                path[e].x = x;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
            }
            if (e < LONGEST_PATH*3 && p.d != down && checkSteppable(a, f, x, y+1)) {
                path[e].d = up;
                path[e].y = y+1;
                path[e].x = x;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
            }
            if (e < LONGEST_PATH*3 && p.d != left && checkSteppable(a, f, x-1, y)) {
                path[e].d = right;
                path[e].y = y;
                path[e].x = x-1;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
            }
            if (e < LONGEST_PATH*3 && p.d != right && checkSteppable(a, f, x+1, y)) {
                path[e].d = left;
                path[e].y = y;
                path[e].x = x+1;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
            }
        }
    }
    return -1;
}

char targetMostCrates (AI* a, FruitCtrl* f, char d)
{
    Path path[FIND_CRATE_ARRAY_SIZE];
    path[0].x = x_to_tx(a->posX);
    path[0].y = y_to_ty(a->posY);
    path[0].d = none;
    path[0].length = 0;
    path[0].prevIndex = -1;

    char max_crates = 0, max_crates_index = -1;
    int i, e;
    for (i = 0, e = 1; i < e && e < FIND_CRATE_ARRAY_SIZE; i++) {
        Path p = path[i];
        int x = p.x;
        int y = p.y;

        if (p.length > d) {
            if (max_crates_index != -1) {
                break;
            } else {
                d++;
            }
        }
        if (p.length <= d){
            char r = preExplodeCrateCount(a, x, y);
            if (r > max_crates) {
                max_crates = r;
                max_crates_index = i;
            }
            if (e < FIND_CRATE_ARRAY_SIZE && p.d != up && checkSafe(a, x, y-1)) {
                path[e].d = down;
                path[e].y = y-1;
                path[e].x = x;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
            }
            if (e < FIND_CRATE_ARRAY_SIZE && p.d != down && checkSafe(a, x, y+1)) {
                path[e].d = up;
                path[e].y = y+1;
                path[e].x = x;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
            }
            if (e < FIND_CRATE_ARRAY_SIZE && p.d != left && checkSafe(a, x-1, y)) {
                path[e].d = right;
                path[e].y = y;
                path[e].x = x-1;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
            }
            if (e < FIND_CRATE_ARRAY_SIZE && p.d != right && checkSafe(a, x+1, y)) {
                path[e].d = left;
                path[e].y = y;
                path[e].x = x+1;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
            }
        }
    }
    if (max_crates_index != -1) {
        int i = max_crates_index;
        Path p = path[i];
        int l = p.length - 1;
        if (l == -1) {
            return 1;
        }
        a->next = 0;
        a->end = l + 1;
        while (l >= 0) {
            p = path[i];
            if (p.length - 1 != l || i == -1) {
                printf("targetMostCrates::CRITICAL AI ERROR\n");
            }
            a->destX[l] = p.x;
            a->destY[l] = p.y;
            i = p.prevIndex;
            l--;
        }
        return 0;
    }
    return -1;
}

// return 1 indicates that a crate is near, 0 indicates moving to crate, -1 indicates no crates in sight
char targetClosestCrate (AI* a)
{
    Path path[LONGEST_PATH*3];
    path[0].x = x_to_tx(a->posX);
    path[0].y = y_to_ty(a->posY);
    path[0].d = none;
    path[0].length = 0;
    path[0].prevIndex = -1;

    int i;
    int e;

    for (i = 0, e = 1; i < e && e < LONGEST_PATH*3; i++) {
        Path p = path[i];
        int x = p.x;
        int y = p.y;

        if (p.length > LONGEST_PATH) {
            continue;
        } else if (a->map[y-1][x] == CRATE || a->map[y+1][x] == CRATE || a->map[y][x-1] == CRATE || a->map[y][x+1] == CRATE) {
            int l = p.length - 1;
            if (l == -1) {
                return 1;
            }
            a->next = 0;
            a->end = l + 1;
            while (l >= 0) {
                p = path[i];
                if (p.length - 1 != l || i == -1) {
                    printf("targetClosestCrate::CRITICAL AI ERROR\n");
                }
                a->destX[l] = p.x;
                a->destY[l] = p.y;
                i = p.prevIndex;
                l--;
            }
            return 0;
        } else {
            if (e < LONGEST_PATH*3 && p.d != up && a->map[y-1][x] == GRASS) {
                path[e].d = down;
                path[e].y = y-1;
                path[e].x = x;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
            }
            if (e < LONGEST_PATH*3 && p.d != down && a->map[y+1][x] == GRASS) {
                path[e].d = up;
                path[e].y = y+1;
                path[e].x = x;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
            }
            if (e < LONGEST_PATH*3 && p.d != left && a->map[y][x-1] == GRASS) {
                path[e].d = right;
                path[e].y = y;
                path[e].x = x-1;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
            }
            if (e < LONGEST_PATH*3 && p.d != right && a->map[y][x+1] == GRASS) {
                path[e].d = left;
                path[e].y = y;
                path[e].x = x+1;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
            }
        }
    }
    return -1;
}

// Return 1 if the spot can be stepped on (it is grass on real map)
char checkSteppable (AI* a, FruitCtrl* f, int x, int y)
{
    if (a->map[y][x] == GRASS || (a->map[y][x] == EXPLOSION && f->map[y][x].t == GRASS)) {
        return 1;
    } else {
        return 0;
    }
}

// Return 1 is safe to stand on (it is grass on pre-exploded map)
char checkSafe (AI* a, int x, int y)
{
    if (a->map[y][x] == GRASS) {
        return 1;
    } else {
        return 0;
    }
}

char preExplodePlayerHit (AI* a, int x, int y, Player* p)
{
    int tx = x_to_tx(p->posX);
    int ty = y_to_ty(p->posY);
    int i;
    if (x != tx && y != ty) {
        return 0;
    } else if (x == tx && y == ty) {
        return 1;
    } else if (x == tx) {
        if (y > ty) {
            for (i = 1; i <= DEFAULT_RADIUS; i++) {
                if (y - i == ty) {
                    return 1;
                }
                if (preCheckExplosion(a, x, y-i) == 1) {
                    break;
                } else if (preCheckExplosion(a, x, y-i) == 2) {
                    break;
                }
            }
        } else if (y < ty) {
            for (i = 1; i <= DEFAULT_RADIUS; i++) {
                if (y + i == ty) {
                    return 1;
                }
                if (preCheckExplosion(a, x, y+i) == 1) {
                    break;
                } else if (preCheckExplosion(a, x, y+i) == 2) {
                    break;
                }
            }
        }
    } else if (y == ty) {
        if (x > tx) {
            for (i = 1; i <= DEFAULT_RADIUS; i++) {
                if (x - i == tx) {
                    return 1;
                }
                if (preCheckExplosion(a, x-i, y) == 1) {
                    break;
                } else if (preCheckExplosion(a, x-i, y) == 1) {
                    break;
                }
            }
        } else if (x < tx) {
            for (i = 1; i <= DEFAULT_RADIUS; i++) {
                if (x + i == tx) {
                    return 1;
                }
                if (preCheckExplosion(a, x+i, y) == 1) {
                    break;
                } else if (preCheckExplosion(a, x+i, y) == 1) {
                    break;
                }
            }
        }
    }
    return 0;
}

char preExplodeCrateCount (AI* a, int x, int y)
{
    int c = 0;
    int i;
    //Check left
    for (i = 1; i <= DEFAULT_RADIUS; i++) {
        if (preCheckExplosion(a, x-i, y) == 1) {
            break;
        } else if (preCheckExplosion(a, x-i, y) == 2) {
            c++;
            break;
        } else if (preCheckExplosion(a, x-i, y) == 3) {
            continue;
        }
    }
    //Check right
    for (i = 1; i <= DEFAULT_RADIUS; i++) {
        if (preCheckExplosion(a, x+i, y) == 1) {
            break;
        } else if (preCheckExplosion(a, x+i, y) == 2) {
            c++;
            break;
        } else if (preCheckExplosion(a, x+i, y) == 3) {
            continue;
        }
    }
    //Check up
    for (i = 1; i <= DEFAULT_RADIUS; i++) {
        if (preCheckExplosion(a, x, y-i) == 1) {
            break;
        } else if (preCheckExplosion(a, x, y-i) == 2) {
            c++;
            break;
        } else if (preCheckExplosion(a, x, y-i) == 3) {
            continue;
        }
    }
    //Check down
    for (i = 1; i <= DEFAULT_RADIUS; i++) {
        if (preCheckExplosion(a, x, y+i) == 1) {
            break;
        } else if (preCheckExplosion(a, x, y+i) == 2) {
            c++;
            break;
        } else if (preCheckExplosion(a, x, y+i) == 3) {
            continue;
        }
    }
    return c;
}

void preExplodeMap (AI* a, FruitCtrl* f)
{
    int y;
    for (y = 0; y < NTILEY; y++) {
        int x;
        for (x = 0; x < NTILEX; x++) {
            a->map[y][x] = f->map[y][x].t;
        }
    }

    int i;
    for (i = 0; i < NUM_PLAYERS*FRUITS_PER_PLAYER; i++) {
        Fruit fruit = f->fruits[i];
        if (fruit.status == active) {
            preExplodeFruit (a, f, fruit);
        }
    }
}

void preExplodeFruit (AI* a, FruitCtrl* f, Fruit fruit)
{
    int x = x_to_tx(fruit.posX);
    int y = y_to_ty(fruit.posY);
    a->map[y][x] = EXPLOSION;
    int i;
    //Check left
    for (i = 1; i <= fruit.radius; i++) {
        if (preCheckExplosion(a, x-i, y) == 1) {
            break;
        } else if (preCheckExplosion(a, x-i, y) == 2) {
            a->map[y][x-i] = EXPLOSION;
            break;
        } else if (preCheckExplosion(a, x-i, y) == 3) {
            continue;
        } else {
            a->map[y][x-i] = EXPLOSION;
        }
    }
    //Check right
    for (i = 1; i <= fruit.radius; i++) {
        if (preCheckExplosion(a, x+i, y) == 1) {
            break;
        } else if (preCheckExplosion(a, x+i, y) == 2) {
            a->map[y][x+i] = EXPLOSION;
            break;
        } else if (preCheckExplosion(a, x+i, y) == 3) {
            continue;
        } else {
            a->map[y][x+i] = EXPLOSION;
        }
    }
    //Check up
    for (i = 1; i <= fruit.radius; i++) {
        if (preCheckExplosion(a, x, y-i) == 1) {
            break;
        } else if (preCheckExplosion(a, x, y-i) == 2) {
            a->map[y-i][x] = EXPLOSION;
            break;
        } else if (preCheckExplosion(a, x, y-i) == 3) {
            continue;
        } else {
            a->map[y-i][x] = EXPLOSION;
        }
    }
    //Check down
    for (i = 1; i <= fruit.radius; i++) {
        if (preCheckExplosion(a, x, y+i) == 1) {
            break;
        } else if (preCheckExplosion(a, x, y+i) == 2) {
            a->map[y+i][x] = EXPLOSION;
            break;
        } else if (preCheckExplosion(a, x, y+i) == 3) {
            continue;
        } else {
            a->map[y+i][x] = EXPLOSION;
        }
    }
}

char preCheckExplosion (AI* a, int x, int y)
{
    if (a->map[y][x] == BLOCK || a->map[y][x] == END || a->map[y][x] == FRUIT) {
        return 1;
    } else if (a->map[y][x] == CRATE) {
        return 2;
    } else if (a->map[y][x] == EXPLOSION) {
        return 3;
    } else if (a->map[y][x] == GRASS) {
        return 0;
    }
    return 0;
}

void handleAI (AI* a, FruitCtrl* f, Player* p)
{
    decide (a, f, p);

    int destX;
    int destY;
    if (a->next != a->end) {
        destX = a->destX[a->next];
        destY = a->destY[a->next];
    } else {
        a->velX = 0;
        a->velY = 0;
        return;
    }

    if (a->move == 1) {
        if (x_to_tx(a->posX) == destX && y_to_ty(a->posY) != destY) {
            a->velX = 0;
            if (y_to_ty(a->posY) > destY) {
                a->velY = -TILE_SIZE;
            } else {
                a->velY = TILE_SIZE;
            }
        } else if (x_to_tx(a->posX) != destX && y_to_ty(a->posY) == destY) {
            a->velY = 0;
            if (x_to_tx(a->posX) > destX) {
                a->velX = -TILE_SIZE;
            } else {
                a->velX = TILE_SIZE;
            }
        } else if (x_to_tx(a->posX) == destX && y_to_ty(a->posY) == destY) {
            a->velX = 0;
            a->velY = 0;
            a->next++;
            handleAI (a, f, p);
            return;
        }
        a->move = 0;
    } else {
        a->velX = 0;
        a->velY = 0;
        a->move++;
    }
    moveAI (a, f);
}

void moveAI (AI* a, FruitCtrl* fruitCtrl)
{
    int tempx = a->posX;
    int tempy = a->posY;
    a->posX += a->velX;

    a->posY += a->velY;

    if (a->dropBomb == 1) {
        //dropFruit(fruitCtrl, a->id, a->posX, a->posY);
        a->dropBomb = 0;
    }
    set_db(fruitCtrl->map, tempx, tempy);
}

void renderAI (AI* a, alt_up_pixel_buffer_dma_dev *pixel_buffer)
{
    ///TODO COLIN: green box
    int x = a->posX;
    int y = a->posY;
    int h = a->height;
    int w = a->width;
    //TODO COLIN: create a function that retrieves the pixel_map_16 for grass sprite for this p->id;
    //draw this pixel_map_16 at x,y
    if(!a->respawnTime)
        if(a->id == 0) {
            //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0xFFFF,1);
            draw_screen_from_bmp(pixel_buffer, booted_bmps, 4, x, y);
            // draw_screen_from_bmp(pixel_buffer, booted_bmps, 1, oldx, oldy);
        }
        else {
            //alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0x003F,1);
            draw_screen_from_bmp(pixel_buffer, booted_bmps, 3, x, y);
            // draw_screen_from_bmp(pixel_buffer, booted_bmps, 1, oldx, oldy);
        }
    else if (a->respawnTime % 2 == 1){
        // alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x + h - 1, y + w - 1, 0x003F00,1);
        // draw_screen_from_bmp(pixel_buffer, booted_bmps, 1, oldx, oldy);
    }
    else if (a->respawnTime % 2 == 0){
        if(a->id == 0) {
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

void updateAI(AI* a)
{
    if (a->respawnTime > 0)
        a->respawnTime--;
}
