#include <math.h>
#include "raylib.h"
#include "utils/movement.h"
#include "engine/physics/collision.h"

Vector3 getMovementVector(Movement movement) {
	Vector3 movementVector;
	float xVelocity = 0;
	float yVelocity = 0;
	float zVelocity = 0;
	float speed = 0.5;

	if (IsKeyDown(KEY_LEFT_SHIFT)) speed *= 2.0;

	if (movement.forward) xVelocity += speed;
	if (movement.backward) xVelocity -= speed;
	if (movement.left) yVelocity -= speed;
	if (movement.right) yVelocity += speed;
	if (movement.up) zVelocity += speed;
	if (movement.down) zVelocity -= speed;

	movementVector.x = xVelocity;
	movementVector.y = yVelocity;
	movementVector.z = zVelocity;

	return movementVector;
}
