#include <math.h>
#include "raylib.h"
#include "utils/movement.h"
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

// This should be generalized. Also need variable input keys to make it customizeable in menu
Movement getPlayerInput() {
	Movement movementsMap;

	movementsMap.left = IsKeyDown(KEY_A);
	movementsMap.right = IsKeyDown(KEY_D);
	movementsMap.forward = IsKeyDown(KEY_W);
	movementsMap.backward = IsKeyDown(KEY_S);
	movementsMap.up = IsKeyPressed(KEY_SPACE);
	movementsMap.down = IsKeyDown(KEY_LEFT_CONTROL);
	return movementsMap;
}

// TODO: Change this to generic movement vector and take in button mappings
// or button mappings can be handled in input and this just keeps the bool checks
Vector3 getPlayerMovementVector(Movement movement, Vector3 forward, Vector3 right) {
	Vector3 movementVector = { 0 };

	if (movement.forward) {
		movementVector.x += forward.x;
		movementVector.z += forward.z;
	}
	if (movement.backward) {
		movementVector.x -= forward.x;
		movementVector.z -= forward.z;
	}
	if (movement.left) {
		movementVector.x -= right.x;
		movementVector.z -= right.z;
	}
	if (movement.right) {
		movementVector.x += right.x;
		movementVector.z += right.z;
	}

	float moveLength = sqrtf(movementVector.x * movementVector.x + movementVector.z * movementVector.z);
	if (moveLength > 0) {
		movementVector.x /= moveLength;
		movementVector.z /= moveLength;
	}

	return movementVector;
}
