#include "star.h"

float randf() {
    return (rand() % 1000) / 1000.0f;
}

Star::Star() {
	this->x = GetRandomValue(0, 2000);
	this->y = GetRandomValue(0, 1000);
	this->z = randf();
}
