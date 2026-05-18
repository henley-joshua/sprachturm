#include "raylib.h"
#include "engine/entity/entity.h"

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
	if (entity->position.y <= groundHeight) return true;
	return false;
}

float getDistanceBelowGround(Entity *entity) {
	return (getGroundHeight(entity->position) - entity->position.y);
}

void setEntityHeight(Entity *entity, float newHeight) {
	entity->position.y = newHeight;
}

void enforceEntityGroundCollision(Entity *entity) {
	if (checkGroundCollision(entity)) setEntityHeight(entity, getGroundHeight(entity->position));
}

float enforcePlayerGroundCollision(Entity player) {
	float upDistance = getDistanceBelowGround(&player);
	if (checkGroundCollision(&player)) return upDistance;
	return 0.0f;
}
