#ifndef OBJECT_IDS_H
#define OBJECT_IDS_H

//! ID's of objects that you can create from the object factory
//! NOTE: These are just examples for now, we'll replace them with real
//! objects later on.

enum ENGINE_OBJECTID {
 OBJECT_ID_BOUNCE = 1,
 OBJECT_ID_PLAYER,
 OBJECT_ID_BACKGROUND,
 OBJECT_ID_CONTROLLER,
 OBJECT_ID_STATIC,
 OBJECT_ID_SPRING,
 OBJECT_ID_ENEMY,
 OBJECT_ID_COLLECTABLE,
 OBJECT_ID_FAN,
 OBJECT_ID_DOOR,
 OBJECT_ID_TXTOVERLAY,
 // OBJECT_ID_3D // not yet.
 OBJECT_ID_CUTBARS
};

//! OBJECT animation ID's (WHY ARE THESE HERE?)

#define PLAYER_WALKING 0
#define PLAYER_STANDING 1
#define PLAYER_JUMPING 2
#define PLAYER_LOOKUP 3
#define PLAYER_SLIDING_DOWN_WALL 4

#define DOOR_CLOSED 0
#define DOOR_OPENING 1

#endif // OBJECT_IDS_H
