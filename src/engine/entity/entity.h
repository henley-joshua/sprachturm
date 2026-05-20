#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"
#include "engine/rendering/model.h"

// Probably will add a velocity vector here
typedef struct {
	Vector3 acceleration;
	Vector3 velocity;
	Vector3 position;
	bool canJump;
	EntityModel model;
} Entity;

#endif
