#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include "raylib.h"
#include "engine/entity/entity.h"


#define MAX_ENTITIES 1024

static Entity entityPool[MAX_ENTITIES];
static bool entityActive[MAX_ENTITIES];
static int entityCount = 0;
static uint32_t nextEntityId = 1;


void entitySystemInit(int maxEntites) {
	memset(entityPool, 0, sizeof(entityPool));
	memset(entityActive, 0, sizeof(entityActive));
	entityCount = 0;
	nextEntityId = 1;
}

void entitySystemShutdown(void) {
	for (int count = 0; count < MAX_ENTITIES; count++) {
		if (entityActive[count]) {
			entityDestroy(&entityPool[count]);
		}
	}

	memset(entityPool, 0, sizeof(entityPool));
	memset(entityActive, 0, sizeof(entityActive));
	entityCount = 0;
	nextEntityId = 1;
}

void entitySetActiveStatus(Entity *entity, bool active) {
	entity->active = active;
}

bool entityGetActiveStatus(Entity *entity) {
	return entity->active;
}

Entity *entityCreate(void) {
	for (int count = 0; count < MAX_ENTITIES; count++) {
		if (!entityActive[count]) {
			entityPool[count] = (Entity){ 0 };
			entityActive[count] = true;
			entityPool[count].id = nextEntityId++;
			entityPool[count].active = true;
			entityPool[count].model.visible = true;
			entityCount++;
			return &entityPool[count];
		}
	}
	TraceLog(LOG_WARNING, "<Entity> Pool full - cannot create entity");
	return NULL;
}

void entityDestroy(Entity *entity) {
	if (!entity) return;

	physicsUnregister(&entity->body);
	//rendererUnregister(entity);   will be added
	
	int index = (int)(entity - entityPool);
	if (index < 0 || index >= MAX_ENTITIES) return;

	entityActive[index] = false;
	entityCount--;
}

// Adds the entity body to the physics pool to have physics applied
// Note that after being registered, this file does not need to update the physics itself.
// Since the entity is in the physics pool, the physics loop will handle all registered bodies.
void entityEnablePhysics(Entity *entity) {
	physicsRegister(&entity->body);
}

// Removes an entity body from the physics pool, preventing physics from applying to it
void entityDisablePhysics(Entity *entity) {
	physicsUnregister(&entity->body);
}

// TODO: add enable/disable functions for rendering as well

// *** ENTITY SETTER FUNCTIONS ***

int entitySetBodyType(Entity *entity, BodyType bodyType) {
	entity->body.bodyType = bodyType;
	return 0;
}

int entitySetColliderType(Entity *entity, ColliderType colliderType) {
	entity->body.colliderType = colliderType;
	return 0;
}

int entitySetPosition(Entity *entity, Vector3 position) {
	entity->body.position = position;
	return 0;
}

int entitySetVelocity(Entity *entity, Vector3 velocity) {
	entity->body.velocity = velocity;
	return 0;
}

int entitySetAcceleration(Entity *entity, Vector3 acceleration) {
	entity->body.acceleration = acceleration;
	return 0;
}

int entitySetMass(Entity *entity, float mass) {
	if (mass <= 0) {
		TraceLog(LOG_WARNING, "<Entity> Attempted to set mass of entity %" PRIu32
			" with the value %f. Mass must be positive.", entity->id, mass);
		return 1;
	}
	entity->body.mass = mass;
	return 0;
}

int entitySetFriction(Entity *entity, float friction) {
	if (friction <= 0) {
		TraceLog(LOG_WARNING, "<Entity> Attempted to set friction of entity %" PRIu32
			" with the value %f. Friction must be positive.", entity->id, friction);
		return 1;
	}
	entity->body.friction = friction;
	return 0;
}

int entitySetBounciness(Entity *entity, float bounciness) {
	if (bounciness <= 0) {
		TraceLog(LOG_WARNING, "<Entity> Attempted to set bounciness of entity %" PRIu32
			" with the value %f. Bounciness must be positive.", entity->id, bounciness);
		return 1;
	}
	entity->body.bounciness = bounciness;
	return 0;
}

int entitySetGravityScale(Entity *entity, float gravityScale) {
	if (gravityScale <= 0) {
		TraceLog(LOG_WARNING, "<Entity> Attempted to set gravity scale of entity %" PRIu32
			" with the value %f. Gravity scale must be positive.", entity->id, gravityScale);
		return 1;
	}
	entity->body.gravityScale = gravityScale;
	return 0;
}

int entitySetDrag(Entity *entity, float drag) {
	if (drag <= 0) {
		TraceLog(LOG_WARNING, "<Entity> Attempted to set drag of entity %" PRIu32
			" with the value %f. Drag must be positive.", entity->id, drag);
		return 1;
	}
	entity->body.drag = drag;
	return 0;
}

int entitySetColliderRadius(Entity *entity, float radius) {
	if (radius <= 0) {
		TraceLog(LOG_WARNING, "<Entity> Attempted to set collider radius of entity %" PRIu32
			" with the value %f. Collider radius must be positive.", entity->id, radius);
		return 1;
	}
	entity->body.colliderRadius = radius;
	return 0;
}

int entitySetColliderExtents(Entity *entity, Vector3 extents) {
	if (extents.x <= 0) {
		TraceLog(LOG_WARNING, "<Entity> Attempted to set collider extents X of entity %" PRIu32
			" with the value %f. All collider extents values must be positive.", entity->id, extents.x);
		return 1;
	}
	if (extents.y <= 0) {
		TraceLog(LOG_WARNING, "<Entity> Attempted to set collider extents Y of entity %" PRIu32
			" with the value %f. All collider extents values must be positive.", entity->id, extents.y);
		return 1;
	}
	if (extents.z <= 0) {
		TraceLog(LOG_WARNING, "<Entity> Attempted to set collider extents Z of entity %" PRIu32
			" with the value %f. All collider extents values must be positive.", entity->id, extents.z);
		return 1;
	}
	entity->body.colliderExtents = extents;
	return 0;
}

int entitySetVelocityLimits(Entity *entity, VelocityLimits limits) {
	if (limits.maxHorizontalVelocity <= 0) {
		TraceLog(LOG_WARNING, "<Entity> Attempted to set horizontal velocity limit of entity %" PRIu32
			" with the value %f. All velocity limit values must be positive.", entity->id, limits.maxHorizontalVelocity);
		return 1;
	}
	if (limits.maxVelocityUp <= 0) {
		TraceLog(LOG_WARNING, "<Entity> Attempted to set velocity limit up of entity %" PRIu32
			" with the value %f. All velocity limit values must be positive.", entity->id, limits.maxVelocityUp);
		return 1;
	}
	if (limits.maxVelocityDown <= 0) {
		TraceLog(LOG_WARNING, "<Entity> Attempted to set velocity limit down of entity %" PRIu32
			" with the value %f. All velocity limit values must be positive.", entity->id, limits.maxVelocityDown);
		return 1;
	}
	entity->body.velocityLimits = limits;
	return 0;
}

int entitySetIsGrounded(Entity *entity, bool isGrounded) {
	entity->body.isGrounded = isGrounded;
	return 0;
}

int entitySetIsActive(Entity *entity, bool isActive) {
	entity->body.isActive = isActive;
	return 0;
}

int entitySetOverrideDrag(Entity *entity, bool override) {
	entity->body.overrideDrag = override;
	return 0;
}

int entitySetOverrideFriction(Entity *entity, bool override) {
	entity->body.overrideFriction = override;
	return 0;
}

// *** ENTITY GETTER FUNCTIONS ***

BodyType entityGetBodyType(Entity *entity) {
	return entity->body.bodyType;
}

ColliderType entityGetColliderType(Entity *entity) {
	return entity->body.colliderType;
}

Vector3 entityGetPosition(Entity *entity) {
	return entity->body.position;
}

Vector3 entityGetVelocity(Entity *entity) {
	return entity->body.velocity;
}

Vector3 entityGetAcceleration(Entity *entity) {
	entity->body.acceleration;
}

float entityGetMass(Entity *entity) {
	return entity->body.mass;
}

float entityGetFriction(Entity *entity) {
	return entity->body.friction;
}

float entityGetBounciness(Entity *entity) {
	return entity->body.bounciness;
}

float entityGetGravityScale(Entity *entity) {
	return entity->body.gravityScale;
}

float entityGetDrag(Entity *entity) {
	return entity->body.drag;
}

float entityGetColliderRadius(Entity *entity) {
	entity->body.colliderRadius;
}

Vector3 entityGetColliderExtents(Entity *entity) {
	return entity->body.colliderExtents;
}

VelocityLimits entityGetVelocityLimits(Entity *entity) {
	return entity->body.velocityLimits;
}

bool entityGetIsGrounded(Entity *entity) {
	return entity->body.isGrounded;
}

bool entityGetIsActive(Entity *entity) {
	return entity->body.isActive;
}

bool entityGetOverrideDrag(Entity *entity) {
	return entity->body.overrideDrag;
}

bool entityGetOverrideFriction(Entity *entity) {
	return entity->body.overrideFriction;
}
