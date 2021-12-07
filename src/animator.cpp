#include "header/animator.h"

/*
 * Purpose of each function and variable is stated in animator.h
 * Example of using Animatior is in player.cpp
 * Here explains the program in more details
 */

Animator::Animator(RenderableEntity* entity, Animation* entry){
    this->target = entity;
    this->currentAnimation = entry;
    this->dire = entry->dire;
    this->frameCount = 0;
}

#pragma region Play Animation
void Animator::FlipAnimation(DIRECTION d){
    this->dire = d;
}
void Animator::PlayAnimation() {
    
    // Decide which animation to play at this frame
    this->checkTransition();
    // Check if the animation is in end state or normal animation state
    if (!this->currentAnimation->animType) { // End State
        this->target->OnEndState();
        this->currentAnimation = this->currentAnimation->transitions[0].nextAnimation; // Usually, after end state, the player object should be deleted. But what changed is player's position...
        return;
    }
    // Flip the image if current animation direction is not the same as default animation of animator
    this->checkAnimationFlip();

    // One frame of the animation has stayed for one more unit time.
    this->frameCount++;

    if (this->frameCount >= (60/this->currentAnimation->frameSpeed))
    {
        // Time to play next frame of the animation
        this->frameCount = 0;
        this->currentAnimation->currentFrame++;

        // Go back to the start of animation
        if (this->currentAnimation->currentFrame > (this->currentAnimation->frameNum - 1)) {
            if (this->currentAnimation->repeatable) {
                this->currentAnimation->currentFrame = this->currentAnimation->startFrame; // NOTE(Noah): I reckon that this will not work if startFrame != 0.
            } 
            else {
                this->currentAnimation->currentFrame = this->currentAnimation->frameNum - 1;
            }
        }

        // // Move Frame rectangle to the next
        // int leftOffsetForReverseDir = this->currentAnimation->sprite.width - ( this->currentAnimation->frameLeftOffset + (this->currentAnimation->frameNum - 1) * this->currentAnimation->frameRecStride + this->currentAnimation->frameRec.width);
        // int offset = (float)this->currentAnimation->currentFrame * this->currentAnimation->frameRecStride;
        // if (this->dire == (DIRECTION)LEFT) {
        //     this->currentAnimation->frameRec.x = leftOffsetForReverseDir + offset;
        // } else { // direction is RIGHT
        //     this->currentAnimation->frameRec.x = this->currentAnimation->frameLeftOffset + offset;    
        // }
    }

    if (this->dire) { // Right
        this->currentAnimation->frameRec.x = (float)this->currentAnimation->currentFrame*(float)this->currentAnimation->sprite.width/this->currentAnimation->frameNum;
    } 
    else { // Left
        this->currentAnimation->frameRec.x = ((float)this->currentAnimation->frameNum - 1.0f - (float)this->currentAnimation->currentFrame)*(float)this->currentAnimation->sprite.width/this->currentAnimation->frameNum;
    }
    
    // Draw part of the texture
    Vector2 drawPos = Vector2Subtract(this->target->GetPos(), Vector2{0, (float)this->currentAnimation->sprite.height}); // NOTE(Noah): Defining position of player as bottom of sprite.
    DrawTextureRec(this->currentAnimation->sprite, this->currentAnimation->frameRec, drawPos, WHITE);
    
    // No matter the animation changed or not, reset all trigger if any has been set to be true.
    if (!this->openTrigger.empty()) {
        this->resetTrigger();
    }
}
void Animator::checkTransition(){

    for (Transition& t : this->currentAnimation->transitions) {

        // if no condition between these two animations, then directly go to the next animation.
        if (t.flags.empty()) {
            if (this->currentAnimation->currentFrame >= (this->currentAnimation->frameNum - 1)) {
                this->resetAnimationConfig();
                this->currentAnimation = t.nextAnimation;
                return;
            }
            return;
        }

        bool nextTransition = false;
        
        // Go through All target conditions to decide which animation to play.
        for (TargetCondition& targetCondition : t.flags) {
            switch (targetCondition.flag)
            {
                case INTEGER:
                {
                    IntCondition* ic = (IntCondition*)this->conditions[targetCondition.conditionName];
                    switch (targetCondition.ic) {
                        case LESS:
                            nextTransition = ic->value >= targetCondition.intValue;
                            break;
                        case EQUAL:
                            nextTransition = ic->value != targetCondition.intValue;
                            break;
                        case GREAT:
                            nextTransition = ic->value <= targetCondition.intValue;
                            break;
                        case LEQ:
                            nextTransition = ic->value > targetCondition.intValue;
                            break;
                        case GEQ:
                            nextTransition = ic->value < targetCondition.intValue;
                            break;
                    }
                    break;
                }
                case BOOLEAN:
                {
                    BoolCondition* bc = (BoolCondition*)this->conditions[targetCondition.conditionName];
                    nextTransition = bc->value != targetCondition.booleanValue;
                    break;
                }
                case TRIGGER:
                {
                    TriggerCondition* tc = (TriggerCondition*)this->conditions[targetCondition.conditionName];
                    nextTransition = !tc->value;
                    break;
                }
            }
            // Certain conditions are not the same then directly pass this animation
            if (nextTransition) {
                break;
            }
        }
        // Certain conditions are met then play the next target animation
        if (!nextTransition) {
            this->resetAnimationConfig();
            this->currentAnimation = t.nextAnimation;
            return;
        }
    }
}
void Animator::checkAnimationFlip(){
    if (this->dire == this->currentAnimation->dire){
        return;
    }
    Image tempImage = GetTextureData(this->currentAnimation->sprite);
    ImageFlipHorizontal(&tempImage);
    this->currentAnimation->sprite = LoadTextureFromImage(tempImage);
    this->currentAnimation->dire = this->dire;
    UnloadImage(tempImage);
}
void Animator::resetAnimationConfig(){
    this->frameCount = 0;
    this->currentAnimation->currentFrame = this->currentAnimation->startFrame;  // currentAnimation has not been changed by now
    // TODO: rectangle should be more flexible.
    this->currentAnimation->frameRec.x = this->currentAnimation->frameLeftOffset;
}
void Animator::resetTrigger(){
    for (std::string& s : this->openTrigger) {
        ((TriggerCondition*)(this->conditions.at(s)))->value = false;  // Reset value
    }
    this->openTrigger.clear();  // clear trigger cache
}
#pragma endregion

#pragma region Set Transition (static functions)
TargetCondition Animator::SetIntTargetCondition(const std::string& s, INTCONDITION ic, int value){
    return TargetCondition{s, INTEGER, ic, value, false}; // the last one is not used, so set it to default value
}
TargetCondition Animator::SetBoolTargetCondition(const std::string& s, bool value){
    return TargetCondition{s, BOOLEAN, NULLCONDITION, -1, value}; // the third and fourth one are not used
}
TargetCondition Animator::SetTriggerTargetCondition(const std::string& s){
    return TargetCondition{s, TRIGGER, NULLCONDITION, -1, false}; // the third and fourth one are not used
}
void Animator::SetTransition(Animation* preAnim, Animation* postAnim, std::vector<TargetCondition> flags){
    preAnim->transitions.push_back({postAnim, flags});
}
#pragma endregion

#pragma region Set Condition
void Animator::SetIntCondition(const std::string& s){
    this->conditions.insert({s, new IntCondition()});
}
void Animator::SetBoolCondition(const std::string& s){
    this->conditions.insert({s, new BoolCondition()});
}
void Animator::SetTriggerCondition(const std::string& s){
    this->conditions.insert({s, new TriggerCondition()});
}
#pragma endregion

#pragma region Set Flag
void Animator::SetInteger(const std::string& s, int v){
    ((IntCondition*)(this->conditions.at(s)))->value = v;
}
void Animator::SetBool(const std::string& s, bool v){
    ((BoolCondition*)(this->conditions.at(s)))->value = v;
}
void Animator::SetTrigger(const std::string& s){
    this->openTrigger.push_back(s);
    ((TriggerCondition*)(this->conditions.at(s)))->value = true;
}
#pragma endregion
