#include "raylib.h"
#include "engine/physics/physics.h"
#include "engine/entity/entity.h"

// Defines a private variable for the 'global' physics config
static PhysicsConfig physicsConfig;


void physicsInit(PhysicsConfig config) {
	physicsConfig = config;
}

// Apply a continuous force, such as gravity
void physicsApplyForce(PhysicsBody *body, Vector3 force) {
	body->acceleration.x += force.x / body->mass;
	body->acceleration.y += force.y / body->mass;
	body->acceleration.z += force.z / body->mass;
}

// Apply a sudden velocity change, such as an explosion
void physicsApplyImpulse(PhysicsBody *body, Vector3 impulse) {
	body->velocity.x += impulse.x;
	body->velocity.y += impulse.y;
	body->velocity.z += impulse.z;
}

// Set velocity of a body directly
void physicsSetVelocity(PhysicsBody *body, Vector3 velocity) {
	body->velocity.x = velocity.x;
	body->velocity.y = velocity.y;
	body->velocity.z = velocity.z;
}

// Prevents velocity from going above the maximum for a given body
void physicsClampVelocity(PhysicsBody *body) {
	if (body->velocity.x > body->velocityLimits.maxHorizontalVelocity) body->velocity.x = body->velocityLimits.maxHorizontalVelocity;
	if (body->velocity.x < -body->velocityLimits.maxHorizontalVelocity) body->velocity.x = -body->velocityLimits.maxHorizontalVelocity;

	if (body->velocity.z > body->velocityLimits.maxHorizontalVelocity) body->velocity.z = body->velocityLimits.maxHorizontalVelocity;
	if (body->velocity.z < -body->velocityLimits.maxHorizontalVelocity) body->velocity.z = -body->velocityLimits.maxHorizontalVelocity;

	if (body->velocity.y > body->velocityLimits.maxVelocityUp) body->velocity.y = body->velocityLimits.maxVelocityUp;
	if (body->velocity.y < -body->velocityLimits.maxVelocityDown) body->velocity.y = -body->velocityLimits.maxVelocityDown;
}

// Applies this frame's physics to a given body
void physicsIntegrate(PhysicsBody *body, float deltaTime) {
	// Applies gravity if body is not 'grounded'
	if (physicsConfig.enableGravity && !body->isGrounded) {
		body->acceleration.y += physicsConfig.gravity.y * body->gravityScale;
	}

	float drag = body->overrideDrag ? body->drag : physicsConfig.airDrag;
	body->velocity.x *= (1.0f - drag);
	body->velocity.y *= (1.0f - drag);
	body->velocity.z *= (1.0f - drag);

	body->velocity.x += body->acceleration.x * deltaTime;
	body->velocity.y += body->acceleration.y * deltaTime;
	body->velocity.z += body->acceleration.z * deltaTime;

	physicsClampVelocity(body);

	body->position.x += body->velocity.x * deltaTime;
	body->position.y += body->velocity.y * deltaTime;
	body->position.z += body->velocity.z * deltaTime;

	body->acceleration = (Vector3){ 0 };
}

void setEntityHeight(Entity *entity, float newHeight) {
	entity->position.y = newHeight;
}

void setEntityVerticalVelocity(Entity *entity, float newVelocity) {
	entity->velocity.z = newVelocity;
}

void setEntityVerticalAcceleration(Entity *entity, float newAcceleration) {
	entity->acceleration.z = newAcceleration;
}

void applyEntityGravity(Entity *entity) {
	float entityFootLevel = entity->position.y - entity->model.size.y;
	if (entityFootLevel > 0) {
		entity->acceleration.z = -GRAVITY;
	}
}

void applyEntityFriction(Entity *entity) {
	if (entity->velocity.y > 0) {
		entity->acceleration.y = entity->acceleration.y * FRICTION;
	}
	if (entity->velocity.x > 0) {
		entity->acceleration.x = entity->acceleration.x * FRICTION;
	}
}
