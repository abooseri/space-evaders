#ifndef ANIMATION_H
#define ANIMATION_H

#include "raylib_wrapped.h"
#include <string>
#include <vector>

struct Animation;

enum ANIMATION_TYPE{
    END=0, ANIM=1
};

enum DIRECTION{
    LEFT=0, RIGHT=1
};

// Conditions used to decide which animation to play
enum FLAG{
    INTEGER=0, BOOLEAN=1, TRIGGER=2
};
// More specific Conditions used with INTEGER
enum INTCONDITION {
    LESS=0, EQUAL=1, GREAT=2, NULLCONDITION=4, LEQ=5, GEQ=6
};

// The condition to play an animation
struct TargetCondition{

    // To identify which flag it is
    std::string conditionName;
    // To identify which type of flag it is
    FLAG flag;

    // int flag value
    INTCONDITION ic;
    int intValue;
    // boolean and trigger flag values
    bool booleanValue;
};

// The next animation to play when these conditions satisfied
struct Transition{
    Animation* nextAnimation;
    std::vector<TargetCondition> flags;
};

struct Animation{
    // Type of Animation
    ANIMATION_TYPE animType;
    // Frames of the Animation
    Texture2D sprite;
    // To allocate which frame of the sprite is displayed
    Rectangle frameRec;
    // The direction that the image faces to
    DIRECTION dire;

    // Which Frame starts from
    int startFrame;
    // Which Frame is displayed now
    int currentFrame;
    // The number of frames that one frame of the sprite can stay.
    int frameSpeed;
    // The number of frames that the sprite has.
    int frameNum;
    // the pixel amount to step the frame rectangle by
    int frameRecStride;
    // the pixel offset from the left of the texture that the first sprite exists at.
    int frameLeftOffset;
    // does the image just play once or multiple times
    bool repeatable;

    // The group of animations to play 
    std::vector<Transition> transitions;
};

#endif