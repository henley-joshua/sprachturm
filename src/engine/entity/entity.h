#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"
#include "engine/rendering/model.h"
#include "engine/physics/physics.h"

typedef struct {
	EntityModel model;
	PhysicsBody body;
} Entity;

#endif
