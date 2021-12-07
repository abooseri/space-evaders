#ifndef PLAYER_H
#define PLAYER_H
#include "character.h"
#include <vector>
#include "entity.h"
#include "animator.h"

class Player : public Character{
    public:
        // player constructor takes in the player position and render layer.
        Player(Vector2, int);

        void update(Game *) override;
        void OnEndState() override;
        float GetCurrentVerticalSpeed();

        constexpr static const float blackHoleCoeff = 20.0f;
Rectangle GetCollisionBounds();
        int getScore();
        void setScore(int newScore);
        float dir;
        float currentVerticalSpeed;

    private:
        // attr
        Game* game;
        
        const float horSpeed = 600.0f;
        const float verSpeed = 310.0f;
        bool inAir;
        bool runFlag;
        int flipMultiplier;
        int score;
        
        Rectangle _collisionBounds;
        
        
        // The run function for the player updates the horizontal movement of the player based on frame deltaTime
        // and the direction of running.
        void run(float, float);
        // Calling the run function will make initiate a player jump sequence.
        void jump();
        // gravity flip function, triggered by pressing f
        void gravityFlip();
        
        void InitializeAnimations() override;
        void SetTransitions() override;
        void SetConditions() override;
};

#endif
