#ifndef OBJECT_IDS_H
#define OBJECT_IDS_H

//! ID's of objects that you can create from the object factory
//! NOTE: These are just examples for now, we'll replace them with real
//! objects later on.

enum OBJECTID {
 OBJECT_ID_BOUNCE = 1,
 OBJECT_ID_PLAYER,
 OBJECT_ID_BACKGROUND,
 OBJECT_ID_CONTROLLER,
 OBJECT_ID_STATIC,
 OBJECT_ID_SPRING,
 OBJECT_ID_COLLECTABLE,
 OBJECT_ID_FAN,
 OBJECT_ID_DOOR,
 OBJECT_ID_TXTOVERLAY
};

//! OBJECT animation ID's
#define PLAYER_WALKING 0
#define PLAYER_STANDING 1
#define PLAYER_JUMPING 2
#define PLAYER_LOOKUP 3
#define PLAYER_MAX_ANIMATIONS 4

#endif // OBJECT_IDS_H
