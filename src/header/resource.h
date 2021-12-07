#ifndef RESOURCE_H
#define RESOURCE_H

/*
 * Highly Recommand to define constants for each file resource and names of files and certain variables
 */

// Screen Size (Camera Size)
#define SCREENWIDTH 850
#define SCREENHEIGHT 600
// Because origin of the image is at upper-left corner, the offset value of cameras should be corrected.
#define OFFSETCORRECTVALUE 100.0f
// Animation file path and animation name
//#define DEBUG
#define PLAYER_ANIMATIONSTART_PATH "../arts/player_stand-halfSize.png"
#define PLAYER_ANIMATIONSTART_NAME "Stand"
#define PLAYER_ANIMATION_WALK_PATH "../arts/player_run_halfSize.png"
#define PLAYER_ANIMATION_WALK_NAME "Run"
#define PLAYER_ANIMATION_JUMP_PATH "../arts/player_jump_halfSize.png"
#define PLAYER_ANIMATION_JUMP_NAME "Jump"
#define PLAYER_ANIMATION_DEATH_PATH "../arts/player_death_halfSize.png"
#define PLAYER_ANIMATION_DEATH_NAME "Death"
#define PLAYER_ANIMATION_END_NAME "End"
// Flags used by the player
#define PLAYER_ANIMATIONCONDITION_BOOL_RUNNING "Running"
#define PLAYER_ANIMATIONCONDITION_BOOL_TOUCH_GROUND "TouchGround"
#define PLAYER_ANIMATIONCONDITION_BOOL_DEATH "Death"
#define PLAYER_ANIMATIONCONDITION_TRIGGER_JUMP "Jump"

#endif
