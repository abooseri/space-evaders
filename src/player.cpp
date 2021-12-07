#include "header/player.h"
#include "header/game.h"
#include "header/ground.h"
#include "header/enemy.h"
#include "animator.cpp"
#include <iostream>

Player::Player(Vector2 v, int layer) : Character(v, layer){
    this->currentVerticalSpeed = 0.0;
    this->inAir = true;
    this->runFlag = true;
    this->flipMultiplier = 1;
    this->score = 0;

    // Initialize Animator
    this->InitializeAnimations();                                                             
    // Tell the animator what is the first animation to play when the game started.
    this->myAnimator = new Animator(this, &(this->animations.at(PLAYER_ANIMATIONSTART_NAME)));
    this->SetConditions();  // Must be called after animator has been initialized!            
    this->SetTransitions();     

    // set up the collision bounds.
    this->_collisionBounds = (Rectangle){
        0, 0,
        76, 105
    };                                                              
}

Rectangle Player::GetCollisionBounds() {
    return (Rectangle){ this->pos.x, this->pos.y - this->_collisionBounds.height, 
        this->_collisionBounds.width, this->_collisionBounds.height };
}

void Player::update(Game *game){ 

    float deltaTime = game->GetLastFrameTime();

    if (this->runFlag) {
        
        // Handle the horizontal movement of player
        dir = 0.0f;
        if (game->getControlFlag()) {
            if (IsKeyDown(KEY_A)) {dir = -1.0f;}
            if (IsKeyDown(KEY_D)) {dir = 1.0f;}
        }
        else {
            if (IsKeyDown(KEY_LEFT)) {dir = -1.0f;}
            if (IsKeyDown(KEY_RIGHT)) {dir = 1.0f;}
        }

        if (dir == -1.0f) {
            this->myAnimator->FlipAnimation(LEFT);
            this->myAnimator->SetBool(PLAYER_ANIMATIONCONDITION_BOOL_RUNNING, true);
        }
        else if (dir == 1.0f) {
            this->myAnimator->FlipAnimation(RIGHT);
            this->myAnimator->SetBool(PLAYER_ANIMATIONCONDITION_BOOL_RUNNING, true); 
        }
        else {
            this->myAnimator->SetBool(PLAYER_ANIMATIONCONDITION_BOOL_RUNNING, false);
        }

        this->run(deltaTime, dir); 
    }

    

    // Code for the gravity flip routine.
    /*if (IsKeyDown(KEY_F) && !this->inAir) {
        this->gravityFlip();
    }*/

    // Player collision code
    {
        // Check if the player has hit any ground
        std::vector<Entity *> grounds = game->GetGrounds();
        for (unsigned int i = 0; i < grounds.size(); i++) { 
            Ground *ground = (Ground *)grounds[i];  
            Vector2 newPlayerPos = this->pos;
            if (ground->TouchGround(this, deltaTime, &newPlayerPos) && this->currentVerticalSpeed >= 0){
                this->pos = newPlayerPos;
                // The player has collided with the ground. Stop movement.
                this->inAir = false;
                this->currentVerticalSpeed = 0.0f;
                //this->pos.y = ground->GetPos().y - this->animations.at(PLAYER_ANIMATIONSTART_NAME).sprite.height; // snap the y position of the player.
                this->pos.y = ground->GetPos().y + 0.01f + deltaTime * ground->GetCurrentOscillationY();
                this->pos.x += deltaTime * ground->GetCurrentOscillationX();
                this->myAnimator->SetBool(PLAYER_ANIMATIONCONDITION_BOOL_TOUCH_GROUND, true);
                break;
            } else {
                this->pos = newPlayerPos;
                this->inAir = true;
                this->myAnimator->SetBool(PLAYER_ANIMATIONCONDITION_BOOL_TOUCH_GROUND, false);
            }
        }

        // Check if player has collided with any enemies
        std::vector<Entity *> enemies = game->GetCharacters();
		for (unsigned int i = 1; i < enemies.size(); i++) {
			Enemy *enemy = (Enemy *)enemies[i];
			if (enemy->EnemyCollide(this)){
                this->myAnimator->SetBool(PLAYER_ANIMATIONCONDITION_BOOL_DEATH, true);
                // Note: only after death animation finishes, it can call the switchGameOver function
				// game->switchGameOver(); // TODO: Maybe give the player like health or something!! Good animation on death, etc.
                this->game = game;
                //game->setDeathReason("You were killed by an alien!");
                this->runFlag = false;
				break;
			}
		}

        // TODO: Check if player is touching top or bottom of the screen, in which case, send them back to spawn (?).
        if (this->pos.y >= 750){ //player has fell off the world -> kill them
            this->myAnimator->SetBool(PLAYER_ANIMATIONCONDITION_BOOL_DEATH, true);
            // game->switchGameOver();
            this->game = game;
            //game->setDeathReason("You fell to your death!");
            this->runFlag = false;
        }
    }

    // Code for the jumping routine.
    // NOTE(Noah): Moved the jumping routine below so that we don't snap the player back to the platform right away...
    if (IsKeyDown(KEY_SPACE) && !this->inAir) {
        this->jump();
        this->myAnimator->SetTrigger(PLAYER_ANIMATIONCONDITION_TRIGGER_JUMP);
    }

    if (game->getControlFlag()){
        if(IsKeyDown(KEY_W) && !this->inAir){
            this->jump();
            this->myAnimator->SetTrigger(PLAYER_ANIMATIONCONDITION_TRIGGER_JUMP);
        }
    }
    else {
        if(IsKeyDown(KEY_UP) && !this->inAir){
            this->jump();
            this->myAnimator->SetTrigger(PLAYER_ANIMATIONCONDITION_TRIGGER_JUMP);
        }
    }

    // Update the vertical movement of the player, note the flip multiplier.
    if (this->runFlag) {
        if (this->inAir) {
            this->pos.y += this->currentVerticalSpeed * deltaTime * flipMultiplier;
            this->currentVerticalSpeed += Entity::gravity * deltaTime;
        }
    }

    // Gravity function of the player
    /*
    if (IsKeyDown(KEY_G)) {
        // Activate the black hole and move all platforms to the player.
        std::vector<Entity *> grounds = game->GetGrounds();
        for (unsigned int i = 0; i < grounds.size(); i++) { 
            Ground *ground = (Ground *)grounds[i];
            if (ground->IsMovable()) {

                Vector2 groundPos = ground->GetPos();

                // Generate vector from ground to player (this is the vector to apply gravity in).
                Vector2 gravityDirection;
                gravityDirection.x = this->pos.x - groundPos.x;
                gravityDirection.y = this->pos.y - groundPos.y;

                // Normalize the gravity direction vector.
                gravityDirection = Vector2Normalize(gravityDirection);

                Vector2 gravityForce = Vector2Scale(gravityDirection, Entity::gravity * 10.0f);
                 
                ground->ApplyForce(gravityForce, deltaTime);
            }
        }
    }
    */

    this->myAnimator->PlayAnimation();
}

void Player::run(float delta, float direction){
    this->pos.x += direction * delta * this->horSpeed;
}

void Player::jump(){
    this->currentVerticalSpeed = - sqrtf(Entity::gravity * 2 * 160); // 160 has units of pixels. Setting V0 for max jump height of 160.
    this->inAir = true;
}

float Player::GetCurrentVerticalSpeed(){
    return this->currentVerticalSpeed;
}

void Player::gravityFlip() {
    this->flipMultiplier = this->flipMultiplier * -1; // flip the multiplier
}

void Player::OnEndState(){
    this->myAnimator->SetBool(PLAYER_ANIMATIONCONDITION_BOOL_DEATH, false);
    if (this->game) {
        this->game->switchGameOver();
        this->runFlag = true;
        this->game = nullptr;
    }
    else {
        TraceLog(LOG_ERROR, "player.cpp Error: Game Not Initialized.");
    }
}

#pragma region Initialize Animator Components
void Player::InitializeAnimations(){
    /* NOTE(Noah): 
        Given that we are doing this type of hard coding for the initialization of animations
        I am going to continue on this trend. We need to subsample into each animation rectangle because each sprite is 
        a much smaller width than the actual rectangle.
        And we will need to do this for both the standing and walking animations.
    */

    // Stand
    Texture2D temp = LoadTexture(PLAYER_ANIMATIONSTART_PATH);
    this->animations.insert({PLAYER_ANIMATIONSTART_NAME, {ANIM, temp, Rectangle{0.0f, 0.0f, (float)temp.width, (float)temp.height}, // Size of one frame 
                             RIGHT,    // The direction that the image faces to
                             0,        // Which frame is the first frame of the animation
                             0,        // Which frame starts to play at the first round. Usually same as the last one.
                             1,       // The number of frames that one frame of the sprite can stay. So-called frame speed.
                             1,        // The total number of frames that the sprite has.
                             temp.width, // stride to step frame rectangle by 
                             0,
                             true}});

    // Walk
    temp = LoadTexture(PLAYER_ANIMATION_WALK_PATH);
    this->animations.insert({PLAYER_ANIMATION_WALK_NAME, {ANIM, temp, Rectangle{0.0f, 0.0f, (float)temp.width/4, (float)temp.height}, 
                             RIGHT, 
                             0, // first frame
                             0, // first frame, first round
                             15, // frame wait time (defines rate animation plays)
                             4, // frames
                             temp.width/4,
                             0,
                             true}});

    // Jump
    temp = LoadTexture(PLAYER_ANIMATION_JUMP_PATH);
    this->animations.insert({PLAYER_ANIMATION_JUMP_NAME, {ANIM, temp, Rectangle{0.0f, 0.0f, (float)temp.width/7, (float)temp.height}, 
                             RIGHT, 
                             0, 
                             0, 
                             10,
                             7, 
                             temp.width/7,
                             0,
                             true}});

    // Death
    temp = LoadTexture(PLAYER_ANIMATION_DEATH_PATH);
    this->animations.insert({PLAYER_ANIMATION_DEATH_NAME, {ANIM, temp, Rectangle{0.0f, 0.0f, (float)temp.width/13, (float)temp.height}, 
                             RIGHT, 
                             0, 
                             0, 
                             10,
                             13, 
                             temp.width/13,
                             0,
                             false}});

    // End State
    this->animations.insert({PLAYER_ANIMATION_END_NAME, {END}});
}

void Player::SetTransitions(){

#pragma region Transitions of Run Animation
    Animator::SetTransition(
        &this->animations[PLAYER_ANIMATIONSTART_NAME],    // From this animation
        &this->animations[PLAYER_ANIMATION_WALK_NAME],    // To this animation
        std::vector<TargetCondition>{                     // under these conditions
            Animator::SetBoolTargetCondition(PLAYER_ANIMATIONCONDITION_BOOL_RUNNING, true),
            Animator::SetBoolTargetCondition(PLAYER_ANIMATIONCONDITION_BOOL_DEATH, false)
        }
    );
    Animator::SetTransition( 
        &this->animations[PLAYER_ANIMATION_WALK_NAME],  
        &this->animations[PLAYER_ANIMATIONSTART_NAME],     
        std::vector<TargetCondition>{                     
            Animator::SetBoolTargetCondition(PLAYER_ANIMATIONCONDITION_BOOL_RUNNING, false),
            Animator::SetBoolTargetCondition(PLAYER_ANIMATIONCONDITION_BOOL_DEATH, false)
        }
    );
#pragma endregion

#pragma region Transitions of Jump Animation
    Animator::SetTransition( 
        &this->animations[PLAYER_ANIMATION_WALK_NAME],  
        &this->animations[PLAYER_ANIMATION_JUMP_NAME],     
        std::vector<TargetCondition>{                     
            Animator::SetTriggerTargetCondition(PLAYER_ANIMATIONCONDITION_TRIGGER_JUMP),
            Animator::SetBoolTargetCondition(PLAYER_ANIMATIONCONDITION_BOOL_DEATH, false)
        }
    );
    Animator::SetTransition( 
        &this->animations[PLAYER_ANIMATIONSTART_NAME],  
        &this->animations[PLAYER_ANIMATION_JUMP_NAME],     
        std::vector<TargetCondition>{                     
            Animator::SetTriggerTargetCondition(PLAYER_ANIMATIONCONDITION_TRIGGER_JUMP),
            Animator::SetBoolTargetCondition(PLAYER_ANIMATIONCONDITION_BOOL_DEATH, false)
        }
    );
    Animator::SetTransition( 
        &this->animations[PLAYER_ANIMATION_JUMP_NAME],  
        &this->animations[PLAYER_ANIMATION_JUMP_NAME],     
        std::vector<TargetCondition>{                     
            Animator::SetTriggerTargetCondition(PLAYER_ANIMATIONCONDITION_TRIGGER_JUMP),
            Animator::SetBoolTargetCondition(PLAYER_ANIMATIONCONDITION_BOOL_DEATH, false)
        }
    );
    Animator::SetTransition( 
        &this->animations[PLAYER_ANIMATION_JUMP_NAME],  
        &this->animations[PLAYER_ANIMATIONSTART_NAME],     
        std::vector<TargetCondition>{                     
            Animator::SetBoolTargetCondition(PLAYER_ANIMATIONCONDITION_BOOL_TOUCH_GROUND, true),
            Animator::SetBoolTargetCondition(PLAYER_ANIMATIONCONDITION_BOOL_RUNNING, false),
            Animator::SetBoolTargetCondition(PLAYER_ANIMATIONCONDITION_BOOL_DEATH, false)
        }
    );
    Animator::SetTransition( 
        &this->animations[PLAYER_ANIMATION_JUMP_NAME],  
        &this->animations[PLAYER_ANIMATION_WALK_NAME],     
        std::vector<TargetCondition>{                     
            Animator::SetBoolTargetCondition(PLAYER_ANIMATIONCONDITION_BOOL_TOUCH_GROUND, true),
            Animator::SetBoolTargetCondition(PLAYER_ANIMATIONCONDITION_BOOL_RUNNING, true),
            Animator::SetBoolTargetCondition(PLAYER_ANIMATIONCONDITION_BOOL_DEATH, false)
        }
    );
#pragma endregion

#pragma region Transitions of Death Animation
    Animator::SetTransition(   
        &this->animations[PLAYER_ANIMATIONSTART_NAME],
        &this->animations[PLAYER_ANIMATION_DEATH_NAME],     
        std::vector<TargetCondition>{
            Animator::SetBoolTargetCondition(PLAYER_ANIMATIONCONDITION_BOOL_DEATH, true)
        }
    );
    Animator::SetTransition(   
        &this->animations[PLAYER_ANIMATION_JUMP_NAME],
        &this->animations[PLAYER_ANIMATION_DEATH_NAME],     
        std::vector<TargetCondition>{
            Animator::SetBoolTargetCondition(PLAYER_ANIMATIONCONDITION_BOOL_DEATH, true)
        }
    );
    Animator::SetTransition(   
        &this->animations[PLAYER_ANIMATION_WALK_NAME],
        &this->animations[PLAYER_ANIMATION_DEATH_NAME],     
        std::vector<TargetCondition>{
            Animator::SetBoolTargetCondition(PLAYER_ANIMATIONCONDITION_BOOL_DEATH, true)
        }
    );
    Animator::SetTransition(
        &this->animations[PLAYER_ANIMATION_DEATH_NAME],  
        &this->animations[PLAYER_ANIMATION_END_NAME],     
        std::vector<TargetCondition>{}
    );
    Animator::SetTransition(
        &this->animations[PLAYER_ANIMATION_END_NAME],  
        &this->animations[PLAYER_ANIMATIONSTART_NAME],     
        std::vector<TargetCondition>{}
    );
#pragma endregion

}

// The player has these flags to control the animation flow
// Initialize flags
void Player::SetConditions(){
    this->myAnimator->SetBoolCondition(PLAYER_ANIMATIONCONDITION_BOOL_RUNNING);
    this->myAnimator->SetBoolCondition(PLAYER_ANIMATIONCONDITION_BOOL_TOUCH_GROUND);
    this->myAnimator->SetBoolCondition(PLAYER_ANIMATIONCONDITION_BOOL_DEATH);
    this->myAnimator->SetTriggerCondition(PLAYER_ANIMATIONCONDITION_TRIGGER_JUMP);
}

int Player::getScore() { return this->score; }
void Player::setScore(int newScore) { this->score = newScore; }
#pragma endregion
