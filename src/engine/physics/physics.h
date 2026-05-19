#ifndef PHYSICS_H
#define PHYSICS_H

#include "engine/entity/entity.h"

void setEntityHeight(Entity *entity, float newHeight);
void setEntityVerticalVelocity(Entity *entity, float newVelocity);
void setEntityVerticalAcceleration(Entity *entity, float newAcceleration);
void applyEntityGravity(Entity *entity);
void applyEntityFriction(Entity *entity);

#endif
