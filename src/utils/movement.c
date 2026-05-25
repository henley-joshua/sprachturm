#include "raylib.h"
#include "utils/movement.h"
#include "game/player.h"
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

// This will have to be updated when we add "platforming" because then the ground height
// will not be the same as the height of the object the player is on. Unless the calculation
// for ground height accounts for that, but that is a little misleading for the function name.
bool checkIfPlayerCanJump(Player *player) {
	float footLevel = player->base.position.y - player->base.model.size.y;
	if (footLevel == getGroundHeight(player->base.position)) return true;
	return false;
}
