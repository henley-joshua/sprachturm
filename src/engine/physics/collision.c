#include "raylib.h"
#include "engine/entity/entity.h"
#include "engine/physics/physics.h"

typedef struct {
	bool left, right, top, bottom;
} BorderCollision;

BorderCollision checkBorderCollision(Rectangle rec) {
	return (BorderCollision) {
		.left = rec.x <= 0,
		.right = rec.x + rec.width >= GetScreenWidth(),
		.top = rec.y <= 0,
		.bottom = rec.y + rec.height >= GetScreenHeight()
	};
}

float getGroundHeight(Vector3 position) {
	return 0.0f;
}

bool checkGroundCollision(Entity *entity) {
	float groundHeight = getGroundHeight(entity->position);
	float footPosition = entity->position.y - entity->model.size.y;
	if (footPosition <= groundHeight) {
		return true;
	}
	return false;
}

float getDistanceBelowGround(Entity *entity) {
	float distanceBelowGround = getGroundHeight(entity->position) - (entity->position.y - entity->model.size.y);
	return distanceBelowGround;
}

void enforceEntityGroundCollision(Entity *entity) {
	if (checkGroundCollision(entity)) {
		setEntityHeight(entity, getGroundHeight(entity->position));
		setEntityVerticalVelocity(entity, 0.0f);
		setEntityVerticalAcceleration(entity, 0.0f);
	}
}

float enforcePlayerGroundCollision(Entity *player) {
	float upDistance = getDistanceBelowGround(player);
	if (checkGroundCollision(player)) {
		if (player->velocity.z < 0.0f) {
			setEntityVerticalVelocity(player, 0.0f);
			setEntityVerticalAcceleration(player, 0.0f);
		}
		return upDistance;
	}
	return 0.0f;
}
