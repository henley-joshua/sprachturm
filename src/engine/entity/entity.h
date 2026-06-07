#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
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
bool entityGetActiveStatus(Entity *entity);
Entity *entityCreate(void);
void entityDestroy(Entity *entity);
void entityEnablePhysics(Entity *entity);
void entityDisablePhysics(Entity *entity);

// ** ENTITY SETTER FUNCTIONS ***
// Returns 0 on successful set, 1 otherwise
int entitySetBodyType(Entity *entity, BodyType bodyType);
int entitySetColliderType(Entity *entity, ColliderType colliderType);
int entitySetPosition(Entity *entity, Vector3 position);
int entitySetVelocity(Entity *entity, Vector3 velocity);
int entitySetAcceleration(Entity *entity, Vector3 acceleration);
int entitySetMass(Entity *entity, float mass);
int entitySetFriction(Entity *entity, float friction);
int entitySetBounciness(Entity *entity, float bounciness);
int entitySetGravityScale(Entity *entity, float gravityScale);
int entitySetDrag(Entity *entity, float drag);
int entitySetColliderRadius(Entity *entity, float radius);
int entitySetColliderExtents(Entity *entity, Vector3 extents);
int entitySetVelocityLimits(Entity *entity, VelocityLimits limits);
int entitySetIsGrounded(Entity *entity, bool isGrounded);
int entitySetIsActive(Entity *entity, bool isActive);
int entitySetOverrideDrag(Entity *entity, bool override);
int entitySetOverrideFriction(Entity *entity, bool override);

// ** ENTITY GETTER FUNCTIONS ***
BodyType entityGetBodyType(Entity *entity);
ColliderType entityGetColliderType(Entity *entity);
Vector3 entityGetPosition(Entity *entity);
Vector3 entityGetVelocity(Entity *entity);
Vector3 entityGetAcceleration(Entity *entity);
float entityGetMass(Entity *entity);
float entityGetFriction(Entity *entity);
float entityGetBounciness(Entity *entity);
float entityGetGravityScale(Entity *entity);
float entityGetDrag(Entity *entity);
float entityGetColliderRadius(Entity *entity);
Vector3 entityGetColliderExtents(Entity *entity);
VelocityLimits entityGetVelocityLimits(Entity *entity);
bool entityGetIsGrounded(Entity *entity);
bool entityGetIsActive(Entity *entity);
bool entityGetOverrideDrag(Entity *entity);
bool entityGetOverrideFriction(Entity *entity);

#endif
