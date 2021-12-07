#ifndef GROUND_H
#define GROUND_H

#include "entity.h"

class Ground : public RenderableEntity{
    public:
        // Constructors
        Ground();
        // Building a ground object takes in the position of the ground object as well as the collision layer.
        
        /*
        Ground(Vector2, int);
        Ground(Vector2, int, float, float, int, int);
        */

        // Also takes in the horizontal and vertical speed.
        Ground(Vector2, int, float, float);
        // This ground constructor takes in the position, collision layer, width and height.
        Ground(Vector2, int, int, int);
        void update(Game *) override;
        // TouchGround returns true if the target entity is touching this ground entity, otherwise TouchGround returns false.
        // sets the player position to valid location.
        bool TouchGround(Player*, float, Vector2 *);
        void SetMovable(bool isMovable, float dirx, float diry, float osx, float osy);
        bool IsMovable() {return this->movable;};
        void ApplyForce(Vector2, float); // applies a force to the ground entity with the given delta time.
        constexpr static const float airFriction = 10.0f; // friction formula being used is airFriction * v, giving a force.
        float getWidth();
        float getHeight();
        float GetCurrentOscillationX();
        float GetCurrentOscillationY();

    private:
        bool movable = false;
        int counter = 0;
        float dirx;
        float diry;
        float oscillationX; // speed at which to oscillate in the X direction
        float oscillationY; // speed at which to oscillate in the Y direction
        Vector2 currentVelocity = (Vector2){0.0f, 0.0f}; // velocity of platform (not including horiztonal or vertical oscillations)
        int width;
        int height;
        float mass = 1; // defaults to 1 kg.
};

#endif
