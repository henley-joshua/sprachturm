#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"

// Probably will add a velocity vector here
typedef struct {
	Vector3 position;
	Vector3 size;
	Color color;
} Entity;

#endif
