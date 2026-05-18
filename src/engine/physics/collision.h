#ifndef COLLISION_H
#define COLLISION_H

#include "raylib.h"
#include "engine/entity/entity.h"

typedef struct {
	bool left, right, top, bottom;
} BorderCollision;
BorderCollision checkBorderCollision(Rectangle rec);
float getGroundHeight(Vector3 position);
bool checkGroundCollision(Entity *entity);
void setEntityHeight(Entity *entity, float newHeight);
void enforceEntityGroundCollision(Entity *entity);
float getDistanceBelowGround(Entity *entity);
float enforcePlayerGroundCollision(Entity player);

#endif
