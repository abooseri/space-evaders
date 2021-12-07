
#include "header/coin.h"
#include "raylib.h"

coin::coin(int x, int y) {
    this->posX = x;
    this->posY = y;
    this->collected = false;
}

coin::coin() {
    this->posX = 1;
    this->posY = 1;
    this->collected = false;
}

bool coin::getCollected() { return this->collected; }

void coin::showCoin() {
    if (!this->getCollected()){
        DrawCircle(this->posX, this->posY, 15, GOLD);
        DrawText("$", this->posX - 5, this->posY - 9, 20, BLACK);
    }

}

void coin::setCollected(bool collect) { this->collected = collect; }
void coin::isCollected() { this->collected = true; }
int coin::getX() { return this->posX; }
int coin::getY() { return this->posY; }

