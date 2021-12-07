#include "header/ground.h"

/*
Ground::Ground(Vector2 v, int layer, float hor, float vert, int dirx, int diry) : RenderableEntity(v, layer){
    this->collisionLayer = layer;
    this->oscillationX = hor;
    this->oscillationY = vert;
    this->dirx = dirx;
    this->diry = diry;
}
*/

Ground::Ground() {
    this->collisionLayer = 0;
    this->oscillationX = 0;
    this->oscillationY = 0;
    this->width = 100;
    this->height = 100;
    this->currentVelocity = Vector2Zero();
    this->movable = false;
    this->mass = 1;
    this->dirx = 0;
    this->diry = 0;
}

/*
Ground::Ground(Vector2 v, int layer) : RenderableEntity(v, layer){
    this->collisionLayer = layer;
}
*/

// what is going on with this initializer list?
Ground::Ground(Vector2 v, int layer, int width, int height) : RenderableEntity(v, layer) {
    this->collisionLayer = layer;
    this->width = width;
    this->height = height;
    this->oscillationX = 0;
    this->oscillationY = 0;
}

void Ground::ApplyForce(Vector2 force, float deltaTime ) {
    Vector2 accelVec = Vector2Scale(force, 1.0f / this->mass * deltaTime); 
    this->currentVelocity = Vector2Add(this->currentVelocity, accelVec);
}

void Ground::SetMovable(bool isMovable, float dirx, float diry, float osx, float osy)
{
    this->movable = isMovable;
    this->dirx = dirx;
    this->diry = diry;
    this->oscillationX = osx;
    this->oscillationY = osy;
};

float Ground::GetCurrentOscillationX(){
	return this->dirx * this->oscillationX;
}


float Ground::GetCurrentOscillationY(){
	return this->diry * this->oscillationY;
}

void Ground::update(Game *game){

    float deltaTime = game->GetLastFrameTime();

    // Apply large amounts of air friction for "locking"
    {
        float currentVelLen = Vector2Length(this->currentVelocity);
        if (currentVelLen != 0) {
            float accelFactor = (Ground::airFriction / this->mass) * currentVelLen;
            Vector2 accelVec = Vector2Scale( Vector2Negate(this->currentVelocity) , (accelFactor / currentVelLen * deltaTime) ); 
            this->currentVelocity = Vector2Add(this->currentVelocity, accelVec);
            if ( Vector2Length(this->currentVelocity) < powf(10, -10) ) {
                this->currentVelocity = Vector2Zero(); // snap the velocity to zero once we get below some super small threshold.
            } 
        }
    }

    // Move according to velocity
    this->pos = Vector2Add(this->pos, Vector2Scale(this->currentVelocity, deltaTime) ); 

    // Now move the platform according to the oscillation
    if (this->IsMovable())
    {    
        this->counter += 1;
        if (this->counter >= 120) {
            this->dirx = -(this->dirx);
            this->diry = -(this->diry);
            this->counter = 0;
        }
        // Modify the position
        {
            this->pos.x += deltaTime * this->oscillationX * this->dirx;
            this->pos.y += deltaTime * this->oscillationY * this->diry;
        }
    }

    Rectangle groundRectangle = (Rectangle){this->pos.x, this->pos.y, (float)this->width, (float)this->height};
    DrawTextureTiled(game->moonTexture, (Rectangle){0.0f, 0.0f, 16.0f, 16.0f}, groundRectangle, Vector2Zero(), 0.0f, 2.5f, RAYWHITE);
    //DrawRectangleRec(Rectangle{this->pos.x, this->pos.y, (float)this->width, (float)this->height}, RED);
}

/*
            how the player ground collision algorithm should work...

            we need to snap the player to the ground (on the top of the platform, and this puts us in the ref frame)
            then we need to not collide with the ground (so the sides plus bottom of the platform).

            touchground function that returns true if we are touching a ground platform.

            collision for bottom and sides is based on x position of player plus the 

            // so we distinugish between top and (sides + bottom) -> contols the inAir variazble.
            // the touchground function repositions the player.
            // but how does it reposition the player if they are on the platform?
            // it JUST snaps the y position, like it currently does.
            */  


// TODO(Noah): There exists a bug where the player will not be able to touch two ground at once.
// if yu are stwanding on top of a platform and a platform is at like waist level, you will fucking
// walk right through this shit.

// also I just had a revelation. Even in times of like, massive brain fart, we as humans are still coherent at language. Programming
// is literally just a language. So it's no wonder I can do this thing for such long hours...and still be proficient...

// issue is clipping due to high acceleration.

bool Ground::TouchGround(Player *target, float deltaTime, Vector2 *newPlayerPos){
    // TODO: Once again, account for the fact that we want variable sized grounds.
    Player *player = (Player *)target;
    Vector2 currentPlayerPos = player->GetPos();
    Rectangle playerColBounds = player->GetCollisionBounds();
    Rectangle groundRec = (Rectangle){this->pos.x, this->pos.y, (float)this->width, (float)this->height};
    //DrawRectangleRec(playerColBounds,RED);
    //DrawRectangleRec(groundRec, GREEN);
    bool result = CheckCollisionRecs(playerColBounds, groundRec); 
    bool isOnTop = false;

    // need to determine specifically wherre the player is relative to the ground...
    // are they on top of the ground? touching the side of the ground? or hitting the ground from the bottom?
    if (result) {
        bool bottom = false;
        bool top = false;
        bool leftSide =  false;
        bool rightSide = false;
        // If they have touched the bottom, the y-position of the player will be at a specific low point.
        // basically height is kind of around bottom of platform + player height. So we say withing some percentage of the player height.
        if ( currentPlayerPos.y < this->pos.y + this->height + playerColBounds.height ) {
            bottom = true; // GOOD
        } 
        if ( currentPlayerPos.x + playerColBounds.width * 0.95f < this->pos.x ) {
            leftSide = true; // GOOD
        }
        if ( currentPlayerPos.x > this->pos.x + this->width - 0.05f * playerColBounds.width ) {
            rightSide = true;
        }
        if ( currentPlayerPos.y < this->pos.y + this->height ) {
            top = true;
        }

        if (top) {

            if (leftSide && player->dir == 1.0f) {
                // get half the character on the platform, but only if the player 
                newPlayerPos->x = newPlayerPos->x + playerColBounds.width / 2.0f;
            }

            if (rightSide && player->dir == -1.0f) {
                // get half the character on the platform, but only if the player 
                newPlayerPos->x = newPlayerPos->x - playerColBounds.width / 2.0f;
            }

            return true;

        } 
        if (leftSide) {
            newPlayerPos->x = this->pos.x - playerColBounds.width;
        } 
        if (rightSide) {
            newPlayerPos->x = this->pos.x + this->width;
        }
        else if (bottom ) {
            newPlayerPos->y = this->pos.y + this->height + playerColBounds.height;
            player->currentVerticalSpeed = 0.0f;
        }

    } else {
        return false;
    }

    /*return ( this->collisionLayer  == player->GetLayer() &&
            this->pos.x           <= player->GetPos().x &&
            this->pos.x + this->width >= player->GetPos().x &&
            this->pos.y           <= player->GetPos().y &&
            this->pos.y + this->height > player->GetPos().y );
    */
}


float Ground::getWidth(){
    return this->width;
}
float Ground::getHeight(){
    return this->height;
}
