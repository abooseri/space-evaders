#include "header/entity.h"

// Constructors
Entity::Entity() {
    this->pos = Vector2({0.0f,0.0f});
}
Entity::Entity(Vector2 p, int layer){
    this->pos = p;
    this->collisionLayer = layer;
}
// Deconstructor
Entity::~Entity(){
}
// Set, get
Vector2 Entity::GetPos() {
    return this->pos;
}
void Entity::SetPos(Vector2 p){
    this->pos = p;
}
int Entity::GetLayer(){
    return this->collisionLayer;
}
// public functions
void Entity::update(Game *game){}




// Constructors
RenderableEntity::RenderableEntity(){}
RenderableEntity::RenderableEntity(Vector2 v, int layer) : Entity(v, layer){} 

// public functions
void RenderableEntity::update(Game *game){}
void RenderableEntity::InitializeAnimations(){}
void RenderableEntity::SetConditions(){}
void RenderableEntity::SetTransitions(){}
void RenderableEntity::OnEndState(){}

RenderableEntity::~RenderableEntity(){
    if (!this->animations.empty()) {
        for (auto& anim : this->animations) {
            UnloadTexture(anim.second.sprite);
        }
    }
}
