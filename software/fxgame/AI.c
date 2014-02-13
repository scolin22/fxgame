
#include "AI.h"
#include "Map.h"
#include "Fruits.h"
#include "Types.h"
#include "Player.h"

//TODO: what if player too far, no crates????

void decide (AI* a, Player* p)
{
    int x = x_to_tx(a->posX);
    int y = y_to_ty(a->posY);

    preExplodeMap(a);

    //Can increase AI reaction time by adding decide(a,p) after every state change
    if (a->state == IDLE) {
        if (checkSafe(a, x, y) == 0) {
            a->state = AVOID_FRUIT;
        } else if (targetPowerUp(a, 6, INVINCIBLE) == 1) {
            a->state = HUNT_GOOD_POWERUP;
        } else if (targetPowerUp(a, 6, FRUITS) == 1) {
            a->state = HUNT_DECENT_POWERUP;
        } else if (targetPlayer (a, p) != -1) {
            a->state = HUNT_PLAYER;
        } else if (targetPowerUp(a, 2, NONE) == 1) {
            a->state = HUNT_BAD_POWERUP;
        } else {
            a->state = FIND_CRATE;
        }
    } else if (a->state == AVOID_FRUIT) {
        if (checkSafe (a, x, y) == 0) {
            targetClosestSafeSpot(a);
        } else {
            a->state = IDLE;
        }
    } else if (a->state == FIND_CRATE) {
        if (checkSafe (a, x, y) == 0) {
            a->state = AVOID_FRUIT;
        } else if (targetPowerUp(a, 6, INVINCIBLE) == 1) {
            a->state = HUNT_GOOD_POWERUP;
        } else if (targetPowerUp(a, 6, FRUITS) == 1) {
            a->state = HUNT_DECENT_POWERUP;
        } else if (targetPlayer(a, p) != -1) {
            a->state = HUNT_PLAYER;
        } else {
            int r = targetMostCrates(a, 3);
            if (r == -1) {
                a->state = IDLE;
                // Need something to do if no crates nearby
            } else if (r == 0) {
                // Keep moving to crate
            } else if (r == 1) {
                a->state = DESTORY;
            }
        }
    } else if (a->state == HUNT_PLAYER) {
        if (checkSafe (a, x, y) == 0) {
            a->state = AVOID_FRUIT;
        } else if (targetPowerUp(a, 6, INVINCIBLE) == 1) {
            a->state = HUNT_GOOD_POWERUP;
        } else if (targetPowerUp(a, 6, FRUITS) == 1) {
            a->state = HUNT_DECENT_POWERUP;
        } else {
            int r = targetPlayer(a, p);
            if (r == -1) {
                // Player too far away
                a->state = IDLE;
            } else if (r == 0) {
                // Keep moving
            } else if (r == 1) {
                a->state = DESTORY;
            }
        }
    } else if (a->state == DESTORY) {
        a->dropBomb = 1;
        a->bombTicks = FRUIT_TIMEOUT + EXPLOSION_TIMEOUT + 4;
        a->state = HIDE;
    } else if (a->state == HIDE) {
        // Skip one frame before hiding
        if (a->bombTicks < FRUIT_TIMEOUT + EXPLOSION_TIMEOUT + 1 && checkSafe (a, x, y) == 0) {
            targetClosestSafeSpot(a);
        }
        a->bombTicks--;
        if (a->bombTicks <= 0) {
            a->state = IDLE;
        }
    } else if (a->state == HUNT_GOOD_POWERUP) {
        if(targetPowerUp(a, 6, INVINCIBLE) == 1) {

        } else {
            a->state = IDLE;
        }
    } else if (a->state == HUNT_DECENT_POWERUP) {
        if(targetPowerUp(a, 6, INVINCIBLE) == 1) {
            a->state = HUNT_GOOD_POWERUP;
        } else if(checkSafe(a, x, y) == 0) {
            a->state = AVOID_FRUIT;
        } else if(targetPowerUp(a, 6, FRUITS) == 1) {

        } else {
            a->state = IDLE;
        }
    } else if (a->state = HUNT_BAD_POWERUP) {
        if(targetPowerUp(a, 6, INVINCIBLE) == 1) {
            a->state = HUNT_GOOD_POWERUP;
        } else if(checkSafe(a, x, y) == 0) {
            a->state = AVOID_FRUIT;
        } else if(targetPowerUp(a, 6, FRUITS) == 1) {
            a->state = HUNT_DECENT_POWERUP;
        } else if (targetPlayer (a, p) != -1) {
            a->state = HUNT_PLAYER;
        } else if (targetPowerUp(a, 2, NONE) == 1) {

        } else {
            a->state = IDLE;
        }
    }
}

// return -1 if no power ups, 1 if invincible or better (always take),
// 2 if bronze or better (only take if idle), 3 if garbage (only take if idle).
char targetPowerUp (AI* a, char d, ai_priority w)
{
    Path path[PATH_ARRAY_SIZE];
    char path_bit[NTILEY][NTILEX] = {{0}};

    path[0].x = x_to_tx(a->posX);
    path[0].y = y_to_ty(a->posY);
    path[0].length = 0;
    path[0].prevIndex = -1;
    path_bit[path[0].y][path[0].x] = 1;

    ai_priority best_powerup = NONE;
    char best_powerup_index = -1;
    int i, e;
    for (i = 0, e = 1; i < e && e < PATH_ARRAY_SIZE; i++) {
        Path p = path[i];
        int x = p.x;
        int y = p.y;
        //TODO: we should probably reduce the longest_path for certain situations
        if (w <= INVINCIBLE && d > LONGEST_GOODPICKUP_PATH) {
            printf("THIS 1\n");
            return -1;
        } else if (w > INVINCIBLE && w <= FRUITS && d > LONGEST_DECENTPICKUP_PATH) {
            printf("THIS 2\n");
            return -1;
        } else if (w > FRUITS && d > LONGEST_BADPICKUP_PATH) {
            printf("THIS 3\n");
            return -1;
        } else if (p.length > d) {
            if (best_powerup_index != -1) {
                break;
            } else {
                d++;
            }
        }
        if (p.length <= d) {
            // If gold and best power up found is worse than gold (greater than the enum)
            if (a->fruitCtrl->map[y][x].t == COLLECTABLE_3 && best_powerup > GOLD) {
                best_powerup = GOLD;
                best_powerup_index = i;
                break; // Might as well break since nothing is better than gold
            } else if (a->fruitCtrl->map[y][x].t == COLLECTABLE_2 && best_powerup > SILVER) {
                best_powerup = SILVER;
                best_powerup_index = i;
            } else if (a->fruitCtrl->map[y][x].t == COLLECTABLE_1 && best_powerup > BRONZE) {
                best_powerup = BRONZE;
                best_powerup_index = i;
            } else if (a->fruitCtrl->map[y][x].t == POWERUP_INVINCIBLE && best_powerup > INVINCIBLE) {
                best_powerup = INVINCIBLE;
                best_powerup_index = i;
            } else if (a->fruitCtrl->map[y][x].t == POWERUP_RADIUS && best_powerup > RADIUS) {
                best_powerup = RADIUS;
                best_powerup_index = i;
            } else if (a->fruitCtrl->map[y][x].t == POWERUP_FRUITS && best_powerup > FRUITS) {
                best_powerup = FRUITS;
                best_powerup_index = i;
            } else if (a->fruitCtrl->map[y][x].t == POWERUP_KICK && best_powerup > KICK) {
                best_powerup = KICK;
                best_powerup_index = i;
            } else if (a->fruitCtrl->map[y][x].t == POWERUP_BULLDOZER && best_powerup > BULLDOZER) {
                best_powerup = BULLDOZER;
                best_powerup_index = i;
            } else if (a->fruitCtrl->map[y][x].t == POWERUP_TOSS && best_powerup > TOSS) {
                best_powerup = TOSS;
                best_powerup_index = i;
            }
            if (e < PATH_ARRAY_SIZE && checkSafe(a, x, y-1) && path_bit[y-1][x] == 0) {
                path[e].y = y-1;
                path[e].x = x;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
                path_bit[y-1][x] = 1;
            }
            if (e < PATH_ARRAY_SIZE && checkSafe(a, x, y+1) && path_bit[y+1][x] == 0) {
                path[e].y = y+1;
                path[e].x = x;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
                path_bit[y+1][x] = 1;
            }
            if (e < PATH_ARRAY_SIZE && checkSafe(a, x-1, y) && path_bit[y][x-1] == 0) {
                path[e].y = y;
                path[e].x = x-1;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
                path_bit[y][x-1] = 1;
            }
            if (e < PATH_ARRAY_SIZE && checkSafe(a, x+1, y) && path_bit[y][x+1] == 0) {
                path[e].y = y;
                path[e].x = x+1;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
                path_bit[y][x+1] = 1;
            }
        }
    }
    if (best_powerup <= w && best_powerup_index != -1) {
        int i = best_powerup_index;
        Path p = path[i];
        int l = p.length - 1;
        if (l == -1) {
            printf("targetPowerUp::WTFFFFF\n");
            return -1; // SHOULDN'T HAPPEN
        }
        a->next = 0;
        a->end = l + 1;
        while (l >= 0) {
            p = path[i];
            if (p.length - 1 != l || i == -1) {
                printf("targetPowerUp::CRITICAL AI ERROR\n");
            }
            a->destX[l] = p.x;
            a->destY[l] = p.y;
            i = p.prevIndex;
            l--;
        }
        return 1;
    }
    printf("END\n");
    return -1;
}

char targetPlayer (AI* a, Player* pl)
{
    if(abs(pl->posX - a->posX) <= 6*TILE_SIZE && abs(pl->posY - a->posY) <= 6*TILE_SIZE) {
        Path path[PATH_ARRAY_SIZE];
        char path_bit[NTILEY][NTILEX] = {{0}};

        path[0].x = x_to_tx(a->posX);
        path[0].y = y_to_ty(a->posY);
        path[0].length = 0;
        path[0].prevIndex = -1;
        path_bit[path[0].y][path[0].x] = 1;

        int i;
        int e;

        for (i = 0, e = 1; i < e && e < PATH_ARRAY_SIZE; i++) {
            Path p = path[i];
            int x = p.x;
            int y = p.y;

            if (p.length > LONGEST_PLAYER_PATH) {
                return -1;
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
                if (e < PATH_ARRAY_SIZE && checkSafe(a, x, y-1) && path_bit[y-1][x] == 0) {
                    path[e].y = y-1;
                    path[e].x = x;
                    path[e].length = p.length+1;
                    path[e].prevIndex = i;
                    e++;
                    path_bit[y-1][x] = 1;
                }
                if (e < PATH_ARRAY_SIZE && checkSafe(a, x, y+1) && path_bit[y+1][x] == 0) {
                    path[e].y = y+1;
                    path[e].x = x;
                    path[e].length = p.length+1;
                    path[e].prevIndex = i;
                    e++;
                    path_bit[y+1][x] = 1;
                }
                if (e < PATH_ARRAY_SIZE && checkSafe(a, x-1, y) && path_bit[y][x-1] == 0) {
                    path[e].y = y;
                    path[e].x = x-1;
                    path[e].length = p.length+1;
                    path[e].prevIndex = i;
                    e++;
                    path_bit[y][x-1] = 1;
                }
                if (e < PATH_ARRAY_SIZE && checkSafe(a, x+1, y) && path_bit[y][x+1] == 0) {
                    path[e].y = y;
                    path[e].x = x+1;
                    path[e].length = p.length+1;
                    path[e].prevIndex = i;
                    e++;
                    path_bit[y][x+1] = 1;
                }
            }
        }
    }
    return -1;
}

char targetClosestSafeSpot (AI* a)
{
    Path path[PATH_ARRAY_SIZE];
    char path_bit[NTILEY][NTILEX] = {{0}};

    path[0].x = x_to_tx(a->posX);
    path[0].y = y_to_ty(a->posY);
    path[0].length = 0;
    path[0].prevIndex = -1;
    path_bit[path[0].y][path[0].x] = 1;

    int i;
    int e;

    for (i = 0, e = 1; i < e && e < PATH_ARRAY_SIZE; i++) {
        Path p = path[i];
        int x = p.x;
        int y = p.y;

        if (p.length > LONGEST_PLAYER_PATH) {
            return -1;
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
            if (e < PATH_ARRAY_SIZE && checkSteppable(a, x, y-1) && path_bit[y-1][x] == 0) {
                path[e].y = y-1;
                path[e].x = x;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
                path_bit[y-1][x] = 1;
            }
            if (e < PATH_ARRAY_SIZE && checkSteppable(a, x, y+1) && path_bit[y+1][x] == 0) {
                path[e].y = y+1;
                path[e].x = x;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
                path_bit[y+1][x] = 1;
            }
            if (e < PATH_ARRAY_SIZE && checkSteppable(a, x-1, y) && path_bit[y][x-1] == 0) {
                path[e].y = y;
                path[e].x = x-1;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
                path_bit[y][x-1] = 1;
            }
            if (e < PATH_ARRAY_SIZE && checkSteppable(a, x+1, y) && path_bit[y][x+1] == 0) {
                path[e].y = y;
                path[e].x = x+1;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
                path_bit[y][x+1] = 1;
            }
        }
    }
    return -1;
}

char targetMostCrates (AI* a, char d)
{
    Path path[PATH_ARRAY_SIZE];
    char path_bit[NTILEY][NTILEX] = {{0}};

    path[0].x = x_to_tx(a->posX);
    path[0].y = y_to_ty(a->posY);
    path[0].length = 0;
    path[0].prevIndex = -1;
    path_bit[path[0].y][path[0].x] = 1;

    char max_crates = 0, max_crates_index = -1;
    int i, e;
    for (i = 0, e = 1; i < e && e < PATH_ARRAY_SIZE; i++) {
        Path p = path[i];
        int x = p.x;
        int y = p.y;

        if (d > LONGEST_CRATE_PATH) {
            return -1;
        } else if (p.length > d) {
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
            if (e < PATH_ARRAY_SIZE && checkSafe(a, x, y-1) && path_bit[y-1][x] == 0) {
                path[e].y = y-1;
                path[e].x = x;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
                path_bit[y-1][x] = 1;
            }
            if (e < PATH_ARRAY_SIZE && checkSafe(a, x, y+1) && path_bit[y+1][x] == 0) {
                path[e].y = y+1;
                path[e].x = x;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
                path_bit[y+1][x] = 1;
            }
            if (e < PATH_ARRAY_SIZE && checkSafe(a, x-1, y) && path_bit[y][x-1] == 0) {
                path[e].y = y;
                path[e].x = x-1;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
                path_bit[y][x-1] = 1;
            }
            if (e < PATH_ARRAY_SIZE && checkSafe(a, x+1, y) && path_bit[y][x+1] == 0) {
                path[e].y = y;
                path[e].x = x+1;
                path[e].length = p.length+1;
                path[e].prevIndex = i;
                e++;
                path_bit[y][x+1] = 1;
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

// Return 1 if the spot can be stepped on (it is grass on real map)
char checkSteppable (AI* a, int x, int y)
{
    tile_t t = a->fruitCtrl->map[y][x].t;
    if (t == GRASS || t >= POWERUP_FRUITS) {
        return 1;
    } else {
        return 0;
    }
}

// Return 1 is safe to stand on (it is grass on pre-exploded map)
char checkSafe (AI* a, int x, int y)
{
    tile_t t = a->map[y][x];
    if (t == GRASS || t >= POWERUP_FRUITS) {
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
            for (i = 1; i <= getFruitRadius(a->fruitCtrl, a->id); i++) {
                if (y - i == ty) {
                    return 1;
                }
                if (preCheckExplosion(a, x, y-i, a->id) == 1) {
                    break;
                } else if (preCheckExplosion(a, x, y-i, a->id) == 2) {
                    break;
                }
            }
        } else if (y < ty) {
            for (i = 1; i <= getFruitRadius(a->fruitCtrl, a->id); i++) {
                if (y + i == ty) {
                    return 1;
                }
                if (preCheckExplosion(a, x, y+i, a->id) == 1) {
                    break;
                } else if (preCheckExplosion(a, x, y+i, a->id) == 2) {
                    break;
                }
            }
        }
    } else if (y == ty) {
        if (x > tx) {
            for (i = 1; i <= getFruitRadius(a->fruitCtrl, a->id); i++) {
                if (x - i == tx) {
                    return 1;
                }
                if (preCheckExplosion(a, x-i, y, a->id) == 1) {
                    break;
                } else if (preCheckExplosion(a, x-i, y, a->id) == 2) {
                    break;
                }
            }
        } else if (x < tx) {
            for (i = 1; i <= getFruitRadius(a->fruitCtrl, a->id); i++) {
                if (x + i == tx) {
                    return 1;
                }
                if (preCheckExplosion(a, x+i, y, a->id) == 1) {
                    break;
                } else if (preCheckExplosion(a, x+i, y, a->id) == 2) {
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
    for (i = 1; i <= getFruitRadius(a->fruitCtrl, a->id); i++) {
        if (preCheckExplosion(a, x-i, y, a->id) == 1) {
            break;
        } else if (preCheckExplosion(a, x-i, y, a->id) == 2) {
            c++;
            break;
        } else if (preCheckExplosion(a, x-i, y, a->id) == 3) {
            continue;
        }
    }
    //Check right
    for (i = 1; i <= getFruitRadius(a->fruitCtrl, a->id); i++) {
        if (preCheckExplosion(a, x+i, y, a->id) == 1) {
            break;
        } else if (preCheckExplosion(a, x+i, y, a->id) == 2) {
            c++;
            break;
        } else if (preCheckExplosion(a, x+i, y, a->id) == 3) {
            continue;
        }
    }
    //Check up
    for (i = 1; i <= getFruitRadius(a->fruitCtrl, a->id); i++) {
        if (preCheckExplosion(a, x, y-i, a->id) == 1) {
            break;
        } else if (preCheckExplosion(a, x, y-i, a->id) == 2) {
            c++;
            break;
        } else if (preCheckExplosion(a, x, y-i, a->id) == 3) {
            continue;
        }
    }
    //Check down
    for (i = 1; i <= getFruitRadius(a->fruitCtrl, a->id); i++) {
        if (preCheckExplosion(a, x, y+i, a->id) == 1) {
            break;
        } else if (preCheckExplosion(a, x, y+i, a->id) == 2) {
            c++;
            break;
        } else if (preCheckExplosion(a, x, y+i, a->id) == 3) {
            continue;
        }
    }
    return c;
}

void preExplodeMap (AI* a)
{
    int y;
    for (y = 0; y < NTILEY; y++) {
        int x;
        for (x = 0; x < NTILEX; x++) {
            a->map[y][x] = a->fruitCtrl->map[y][x].t;
        }
    }

    int i;
    for (i = 0; i < NUM_PLAYERS*FRUITS_PER_PLAYER; i++) {
        Fruit fruit = a->fruitCtrl->fruits[i];
        if (fruit.status == active) {
            preExplodeFruit (a, fruit);
        }
    }
}

void preExplodeFruit (AI* a, Fruit fruit)
{
    int x = x_to_tx(fruit.posX);
    int y = y_to_ty(fruit.posY);
    a->map[y][x] = EXPLOSION;
    int i;
    //Check left
    for (i = 1; i <= fruit.radius; i++) {
        if (preCheckExplosion(a, x-i, y, fruit.owner) == 1) {
            break;
        } else if (preCheckExplosion(a, x-i, y, fruit.owner) == 2) {
            a->map[y][x-i] = EXPLOSION;
            break;
        } else if (preCheckExplosion(a, x-i, y, fruit.owner) == 3) {
            continue;
        } else {
            a->map[y][x-i] = EXPLOSION;
        }
    }
    //Check right
    for (i = 1; i <= fruit.radius; i++) {
        if (preCheckExplosion(a, x+i, y, fruit.owner) == 1) {
            break;
        } else if (preCheckExplosion(a, x+i, y, fruit.owner) == 2) {
            a->map[y][x+i] = EXPLOSION;
            break;
        } else if (preCheckExplosion(a, x+i, y, fruit.owner) == 3) {
            continue;
        } else {
            a->map[y][x+i] = EXPLOSION;
        }
    }
    //Check up
    for (i = 1; i <= fruit.radius; i++) {
        if (preCheckExplosion(a, x, y-i, fruit.owner) == 1) {
            break;
        } else if (preCheckExplosion(a, x, y-i, fruit.owner) == 2) {
            a->map[y-i][x] = EXPLOSION;
            break;
        } else if (preCheckExplosion(a, x, y-i, fruit.owner) == 3) {
            continue;
        } else {
            a->map[y-i][x] = EXPLOSION;
        }
    }
    //Check down
    for (i = 1; i <= fruit.radius; i++) {
        if (preCheckExplosion(a, x, y+i, fruit.owner) == 1) {
            break;
        } else if (preCheckExplosion(a, x, y+i, fruit.owner) == 2) {
            a->map[y+i][x] = EXPLOSION;
            break;
        } else if (preCheckExplosion(a, x, y+i, fruit.owner) == 3) {
            continue;
        } else {
            a->map[y+i][x] = EXPLOSION;
        }
    }
}

// Return 1 to stop explode, 2 to explode then stop, 3 to not explode but step over
char preCheckExplosion (AI* a, int x, int y, int owner)
{
    if (a->map[y][x] == BLOCK && getFruitType(a->fruitCtrl, owner) == watermelon) {
        return 3;
    } else if (a->map[y][x] == BLOCK || a->map[y][x] == END || a->map[y][x] == FRUIT) {
        return 1;
    } else if (a->map[y][x] == CRATE) {
        return 2;
    } else if (a->map[y][x] == EXPLOSION) {
        return 3;
    }
    return 0;
}

void handleAI (AI* a, Player* p, char switches)
{
    checkCollisionAI(a);
    if (switches > 0) {
        a->velX = 0;
        a->velY = 0;
        return;
    }
    if (a->stunnedTime < 1) {
        decide (a, p);

        int destX;
        int destY;
        if (a->next != a->end) {
            destX = a->destX[a->next];
            destY = a->destY[a->next];
        } else {
            a->velX = 0;
            a->velY = 0;
            moveAI (a);
            return;
        }

        if (a->move == 3) {
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
                handleAI (a, p, switches);
                return;
            }
            a->move = 0;
            moveAI (a);
        } else {
            a->velX = 0;
            a->velY = 0;
            a->move++;
            moveAI (a);
        }
    }
}

void checkCollisionAI (AI* a)
{
    int x = a->posX;
    int y = a->posY;
    tile_t tile = a->fruitCtrl->map[y_to_ty(y)][x_to_tx(x)].t;
    if (tile == EXPLOSION && a->respawnTime == 0) {
        a->respawnTime = RESPAWN_TIME;
        if(a->fruitCtrl->map[y_to_ty(y)][x_to_tx(x)].t == banana)
            a->stunnedTime = STUNNED_TIME;
        if(a->fruitCtrl->map[y_to_ty(y)][x_to_tx(x)].owner != a->id)
            *(players->list[a->fruitCtrl->map[y_to_ty(y)][x_to_tx(x)].owner]->score) += 100;
        *(a->score) -= 100;
    } else if (tile == COLLECTABLE_1) {
        *(a->score) += 100;
        changeTile(a->fruitCtrl->map, x, y, GRASS);
    } else if (tile == COLLECTABLE_2) {
        *(a->score) += 500;
        changeTile(a->fruitCtrl->map, x, y, GRASS);
    } else if (tile == COLLECTABLE_3) {
        *(a->score) += 1000;
        changeTile(a->fruitCtrl->map, x, y, GRASS);
    } else if (tile == POWERUP_FRUITS) {
        increaseFruitCount(a->fruitCtrl, a->id);
        changeTile(a->fruitCtrl->map, x, y, GRASS);
    } else if (tile == POWERUP_RADIUS) {
        increaseFruitRadius(a->fruitCtrl, a->id);
        changeTile(a->fruitCtrl->map, x, y, GRASS);
    } else if (tile == POWERUP_KICK) {
//        setPowerUps(p, kick);
        changeTile(a->fruitCtrl->map, x, y, GRASS);
    } else if (tile == POWERUP_TOSS) {
//        setPowerUps(p, toss);
        changeTile(a->fruitCtrl->map, x, y, GRASS);
    } else if (tile == POWERUP_INVINCIBLE) {
//        setPowerUps(p, invincible);
//        p->respawnTime = RESPAWN_TIME*5;
        changeTile(a->fruitCtrl->map, x, y, GRASS);
    } else if (tile == POWERUP_BULLDOZER) {
//        setPowerUps(p, bulldozer);
//        p->bullCount++;
        changeTile(a->fruitCtrl->map, x, y, GRASS);
    }
}

void moveAI (AI* a)
{
    a->fruitCtrl->map[y_to_ty(a->posY)][x_to_tx(a->posX)].playerOn = 0;

    int tempx = a->posX;
    int tempy = a->posY;
    a->posX += a->velX;
    if (a->velX > 0) {
        a->dir = right;
    } else if (a->velX < 0) {
        a->dir = left;
    }

    a->posY += a->velY;
    if (a->velY > 0) {
        a->dir = down;
    } else if (a->velY < 0) {
        a->dir = up;
    }

    if (a->dropBomb == 1) {
        dropFruit(a->fruitCtrl, a->id, 0, a->dir, a->posX, a->posY);
        a->dropBomb = 0;
    }
    set_db(a->fruitCtrl->map, tempx, tempy);
    checkCollisionAI(a);

    a->fruitCtrl->map[y_to_ty(a->posY)][x_to_tx(a->posX)].playerOn = 1;
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
    if(a->stunnedTime > 0)
        a->stunnedTime--;
}

void chooseFruitForAI(AI* p, FruitCtrl* fruitCtrl, fruitType type) {
    fruitCtrl->types[p->id] = type;
    if (type == cherry)
        fruitCtrl->maxFruits[p->id]++;
}
