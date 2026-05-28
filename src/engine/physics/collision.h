#ifndef COLLISION_H
#define COLLISION_H

#include "raylib.h"
#include "engine/physics/physics_types.h"

float heightmapGetHeight(Vector3 position);
Vector3 heightmapGetNormal(Vector3 position);
void collisionInit(void);
CollisionResult collisionHeightmapCheck(PhysicsBody *body);
bool collisionBroadPhase(PhysicsBody *bodyA, PhysicsBody *bodyB);
CollisionResult collisionNarrowPhase(PhysicsBody *bodyA, PhysicsBody *bodyB);

#endif
