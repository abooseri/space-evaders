#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"
#include <vector>

class Enemy : public Character {
    public:
        // Constructor
        Enemy();
        // Building a enemy object takes in the position of the enemy object as well as the collision layer.
        Enemy(Vector2, int);
        // Returns true if the player has touched enemy, otherwise will returns false.
        bool EnemyCollide(Entity*);
        Enemy(Vector2, int, float, float);
        // This ground constructor takes in the position, collision layer, width and height.
        Enemy(Vector2, int, int, int);
        void update(Game *) override;
        void SetMovable(bool isMovable, float dirx, float diry, float osx, float osy);
        bool IsMovable() {return this->movable;};
        void ApplyForce(Vector2, float); // applies a force to the ground entity with the given delta time.
        constexpr static const float airFriction = 10.0f; // friction formula being used is airFriction * v, giving a force.


    private:
        bool movable = false;
        int counter = 0;
        float dirx;
        float diry;
        float oscillationX; // speed at which to oscillate in the X direction
        float oscillationY; // speed at which to oscillate in the Y direction
        Vector2 currentVelocity = (Vector2){0.0f, 0.0f}; // velocity of enemy (not including horiztonal or vertical oscillations)
        int width;
        int height;
        float mass = 1; // defaults to 1 kg.
};

#endif
