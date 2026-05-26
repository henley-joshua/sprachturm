#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "raylib.h"

typedef struct {
	bool forward;
	bool backward;
	bool left;
	bool right;
	bool up;
	bool down;
} Movement;

Vector3 getMovementVector(Movement movement);
Vector3 getPlayerMovementVector(Movement movement, Vector3 forward, Vector3 right);
Movement getPlayerInput();

#endif
