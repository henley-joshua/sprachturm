#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"

// Probably will add a velocity vector here
typedef struct {
	Vector3 acceleration;
	Vector3 velocity;
	Vector3 position;
	Vector3 size;
	bool canJump;
	Color color;
} Entity;

#endif
