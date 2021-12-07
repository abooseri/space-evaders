#include "header/rocket.h"
#include "raylib.h"

Rocket::Rocket(float x, float y) {
    this->posX = x;
    this->posY = y;
    this->touched = false;
    this->rocketTexture = LoadTexture("../arts/escape_rocket.png");
}

Rocket::Rocket() {
    this->posX = 1;
    this->posY = 1;
    this->touched = false;
}

bool Rocket::getComplete() { return this->touched; }

void Rocket::displayRocket() {
	DrawTexture(this->rocketTexture, this->posX, this->posY, RAYWHITE);
}

void Rocket::setComplete(bool touched) { this->touched = touched; }
void Rocket::isComplete() { this->touched = true; }
int Rocket::getX() { return this->posX; }
int Rocket::getY() { return this->posY; }
