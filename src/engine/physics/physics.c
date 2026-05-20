#include "raylib.h"
#include "engine/entity/entity.h"

float GRAVITY = 0.1f;
float FRICTION = 0.20f;

void setEntityHeight(Entity *entity, float newHeight) {
	entity->position.y = newHeight;
}

void setEntityVerticalVelocity(Entity *entity, float newVelocity) {
	entity->velocity.z = newVelocity;
}

void setEntityVerticalAcceleration(Entity *entity, float newAcceleration) {
	entity->acceleration.z = newAcceleration;
}

void applyEntityGravity(Entity *entity) {
	float entityFootLevel = entity->position.y - entity->model.size.y;
	if (entityFootLevel > 0) {
		entity->acceleration.z = -GRAVITY;
	}
}

void applyEntityFriction(Entity *entity) {
	if (entity->velocity.y > 0) {
		entity->acceleration.y = entity->acceleration.y * FRICTION;
	}
	if (entity->velocity.x > 0) {
		entity->acceleration.x = entity->acceleration.x * FRICTION;
	}
}
