#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"
#include "engine/physics/physics.h"

typedef struct {
	Mesh *mesh;
	Material *material;
	Vector3 size;
	Color tint;
	bool visible;
	bool castShadow;  // Will be used eventually
} EntityModel;


typedef struct {
	uint32_t id;
	bool active;
	EntityModel model;
	PhysicsBody body;
} Entity;

void entitySystemInit(int maxEntites);
void entitySystemShutdown(void);
void entitySetActiveStatus(Entity *entity, bool active);
Entity *entityCreate(void);
void entityDestroy(Entity *entity);

#endif
