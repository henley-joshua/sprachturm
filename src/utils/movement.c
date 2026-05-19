#include "raylib.h"
#include "utils/movement.h"
#include "engine/entity/player.h"
#include "engine/physics/collision.h"

Vector3 getMovementVector(Movement movement) {
	Vector3 movementVector;
	int xVelocity = 0;
	int yVelocity = 0;
	int zVelocity = 0;

	if (movement.forward) xVelocity += 1;
	if (movement.backward) xVelocity -= 1;
	if (movement.left) yVelocity -= 1;
	if (movement.right) yVelocity += 1;
	if (movement.up) zVelocity += 1;
	if (movement.down) zVelocity -= 1;

	movementVector.x = xVelocity;
	movementVector.y = yVelocity;
	movementVector.z = zVelocity;

	return movementVector;
}

Vector3 getPlayerMovementVector(Movement movement, Player *player) {
	Vector3 movementVector;
	int xVelocity = 0;
	int yVelocity = 0;
	int zAcceleration = 0;

	if (movement.forward) xVelocity = 1.0f;
	if (movement.backward) xVelocity = -1.0f;
	if (movement.left) yVelocity = -1.0f;
	if (movement.right) yVelocity = 1.0f;
	if (movement.up && player->base.canJump) zAcceleration += 1.0f;
	if (movement.down) zAcceleration -= 0.0f;

	movementVector.x = xVelocity;
	movementVector.y = yVelocity;
	movementVector.z = zAcceleration;

	return movementVector;
}

bool checkIfPlayerCanJump(Player *player) {
	if (player->base.position.y == getGroundHeight(player->base.position)) return true;
	return false;
}
