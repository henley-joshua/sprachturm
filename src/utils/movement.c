#include "raylib.h"
#include "utils/movement.h"

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
