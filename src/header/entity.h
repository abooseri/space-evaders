#ifndef ENTITY_H
#define ENTITY_H

#include "raylib_wrapped.h"
#include "animator.h"
#include "resource.h"
#include <unordered_map>

class Game;

class Entity {
    public:
        Entity();
        virtual ~Entity();
        // Build entity with position and collision layer 
        Entity(Vector2, int);

        Vector2 GetPos();
        void SetPos(Vector2);
        int GetLayer();

        // Update is a function that should be called every frame for all entities.
        // Entities are passed the main game object.
        virtual void update(Game *);

    protected: 
        Vector2 pos;
        static const int gravity = 4000; // NOTE(Noah): Should use this type of initialization over gravity{400}
        int collisionLayer;
};

class RenderableEntity : public Entity {
    public:
        RenderableEntity();
        RenderableEntity(Vector2, int);
        virtual ~RenderableEntity();
    
        virtual void update(Game *);
        // Function called by the End-Type Animation
        virtual void OnEndState();

    protected:
        Animator* myAnimator;
        std::unordered_map<std::string, Animation> animations;
        // Initialize All Animations and put into animations.
        virtual void InitializeAnimations();
        // Initialize all transitions between animations
        virtual void SetTransitions();
        // Initialize all conditions
        virtual void SetConditions();
};
#endif