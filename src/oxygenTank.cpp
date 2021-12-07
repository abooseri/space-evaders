#include "header/oxygenTank.h"

#include "raylib.h"

tank::tank(int x, int y) {
    this->posX = x;
    this->posY = y;
    this->collected = false;
    this->width = 30;
    this->height = 50;
}

tank::tank() {
    this->posX = 1;
    this->posY = 1;
    this->collected = false;
    this->width = 30;
    this->height = 50;
}

bool tank::getCollected() { return this->collected; }

Rectangle tank::GetRecBounds() {
    return (Rectangle){ (float)this->posX, (float)this->posY, this->width, this->height };
}

void tank::showTank() {
    if (!this->getCollected()){
        DrawRectangleRec(this->GetRecBounds(), GRAY);
        DrawText("O2", this->posX + 3, this->posY + 15, 20, BLACK);
    }
}

void tank::setCollected(bool collect) { this->collected = collect; }
void tank::isCollected() { this->collected = true; }
int tank::getX() { return this->posX; }
int tank::getY() { return this->posY; }
