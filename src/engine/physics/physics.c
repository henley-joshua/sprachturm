#include <stddef.h>
#include "raylib.h"
#include "engine/entity/entity.h"
#include "engine/physics/physics_types.h"

#define MAX_PHYSICS_BODIES    1024
#define MAX_COLLISION_RESULTS 512

// Defines a private variable for the 'global' physics config
static PhysicsConfig physicsConfig;                           // tracks global physics config locally
static PhysicsBody *bodies[MAX_PHYSICS_BODIES];               // tracks all registered physics bodies
static CollisionPair collisionPairs[MAX_COLLISION_RESULTS];   // tracks all calculated collision result pairs
static int bodyCount = 0;                                     // tracks physics total body count
static int collisionPairCount = 0;                            // tracks total collision pair count
static float *heightmapData;
static int heightmapWidth;
static int heightmapHeight;
static Vector3 heightmapScale;


// Initializes the global physics configuration
void physicsInit(PhysicsConfig config) {
	physicsConfig = config;
}

// Function for the game to register the heightmap once when loading the 'level'
void physicsSetHeightmap(float *data, int width, int height, Vector3 scale) {
	heightmapData = data;
	heightmapWidth = width;
	heightmapHeight = height;
	heightmapScale = scale;
}

// Registers a body to have physics applied every frame.
// This prevents calculating physics on all bodies at all times.
void physicsRegister(PhysicsBody *body) {
	if (bodyCount >= MAX_PHYSICS_BODIES) {
		TraceLog(LOG_WARNING, "<Physics> Body limit reached");
		return;
	}
	bodies[bodyCount++] = body;
}

// Unregisters a body by removing it from the body array.
void physicsUnregister(PhysicsBody *body) {
	for (int counter = 0; counter < bodyCount; counter++) {
		if (bodies[counter] == body) {
			// Swaps the match with the last body in the array and shrinks array
			bodies[counter] = bodies[--bodyCount];
			return;
		}
	}
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

// Checks for all registered physics body collisions and adds collisions to the collisionPairs array
static void physicsDetectCollisions(void) {
	collisionPairCount = 0;

	for (int count = 0; count < bodyCount; count++) {
		for (int count2 = count + 1; count2 < bodyCount; count2++) {
			PhysicsBody *bodyA = bodies[count];
			PhysicsBody *bodyB = bodies[count2];

			if (!bodyA->isActive || !bodyB->isActive) continue;
			if (bodyA->bodyType == BODY_STATIC && bodyB->bodyType == BODY_STATIC) continue;
			if (!collisionBroadPhase(bodyA, bodyB)) continue;

			TraceLog(LOG_INFO, "Collision moving to narrow phase");

			CollisionResult result = collisionNarrowPhase(bodyA, bodyB);

			if (result.hit) {
				if (collisionPairCount < MAX_COLLISION_RESULTS) {
					collisionPairs[collisionPairCount++] = (CollisionPair){
						.bodyA = bodyA,
						.bodyB = bodyB,
						.result = result
					};
				}
			}
		}
	}
}

static void physicsResolveCollisionVelocity(PhysicsBody *bodyA, PhysicsBody *bodyB, CollisionResult result) {
	Vector3 relativeVelocity = {
		bodyA->velocity.x - bodyB->velocity.x,
		bodyA->velocity.y - bodyB->velocity.y,
		bodyA->velocity.z - bodyB->velocity.z
	};

	float velocityAlongNormal =
		relativeVelocity.x * result.normal.x +
		relativeVelocity.y * result.normal.y +
		relativeVelocity.z * result.normal.z;

	// If they are moving away from each other, don't resolve velocity
	if (velocityAlongNormal > 0) return;

	float bounciness = bodyB->bounciness;
	if (bodyA->bounciness < bodyB->bounciness) bounciness = bodyA->bounciness;

	float impulseScalar;

	if (bodyA->bodyType == BODY_DYNAMIC && bodyB->bodyType == BODY_DYNAMIC) {
		// both get updated velocities along the normal proportionate to their mass
		impulseScalar = -(1.0f + bounciness) * velocityAlongNormal;
		impulseScalar /= (1.0f / bodyA->mass) + (1.0f / bodyB->mass);

		float impulseA = impulseScalar / bodyA->mass;
		float impulseB = impulseScalar / bodyB->mass;

		bodyA->velocity.x += impulseA * result.normal.x;
		bodyA->velocity.y += impulseA * result.normal.y;
		bodyA->velocity.z += impulseA * result.normal.z;

		bodyB->velocity.x -= impulseB * result.normal.x;
		bodyB->velocity.y -= impulseB * result.normal.y;
		bodyB->velocity.z -= impulseB * result.normal.z;
	} else if (bodyA->bodyType == BODY_DYNAMIC) {
		// only bodyA bounces, because bodyB is static
		impulseScalar = -(1.0f + bounciness) * velocityAlongNormal;

		bodyA->velocity.x += impulseScalar * result.normal.x;
		bodyA->velocity.y += impulseScalar * result.normal.y;
		bodyA->velocity.z += impulseScalar * result.normal.z;
	} else if (bodyB->bodyType == BODY_DYNAMIC) {
		// only bodyB bounces, because bodyA is static
		impulseScalar = -(1.0f + bounciness) * velocityAlongNormal;

		bodyB->velocity.x += impulseScalar * result.normal.x;
		bodyB->velocity.y += impulseScalar * result.normal.y;
		bodyB->velocity.z += impulseScalar * result.normal.z;
	}
}

// Simpler collision interaction between a dynamic body and static body, only applies to the dynamic body
static void physicsResolveStatic(PhysicsBody *body, CollisionResult result) {
	TraceLog(LOG_INFO, "BEFORE resolve pos: %f %f %f vel: %f %f %f",
		body->position.x, body->position.y, body->position.z,
		body->velocity.x, body->velocity.y, body->velocity.z);
	body->position.x += result.normal.x * result.depth;
	body->position.y += result.normal.y * result.depth;
	body->position.z += result.normal.z * result.depth;

	float velocityAlongNormal =
		body->velocity.x * result.normal.x +
		body->velocity.y * result.normal.y +
		body->velocity.z * result.normal.z;

	if (velocityAlongNormal <= 0) {
		float impulse = -(1.0f + body->bounciness) * velocityAlongNormal;

		body->velocity.x += impulse * result.normal.x;
		body->velocity.y += impulse * result.normal.y;
		body->velocity.z += impulse * result.normal.z;
	}

	if (result.normal.y > 0.7f) {
		body->isGrounded = true;
		body->velocity.y = 0;
		body->acceleration.y = 0;
	}

	TraceLog(LOG_INFO, "AFTER resolve pos: %f %f %f vel: %f %f %f",
		body->position.x, body->position.y, body->position.z,
		body->velocity.x, body->velocity.y, body->velocity.z);
}

// Applies the result of a collision to the two bodies involved
void physicsResolveCollision(PhysicsBody *bodyA, PhysicsBody *bodyB, CollisionResult result) {
	bool aMoveable = bodyA->bodyType == BODY_DYNAMIC;
	bool bMoveable = bodyB->bodyType == BODY_DYNAMIC;

	if (aMoveable && bMoveable) {
		float totalMass = bodyA->mass + bodyB->mass;
		float ratioA = bodyB->mass / totalMass;
		float ratioB = bodyA->mass / totalMass;

		bodyA->position.x += result.normal.x * result.depth * ratioA;
		bodyA->position.y += result.normal.y * result.depth * ratioA;
		bodyA->position.z += result.normal.z * result.depth * ratioA;

		bodyB->position.x += result.normal.x * result.depth * ratioB;
		bodyB->position.y += result.normal.y * result.depth * ratioB;
		bodyB->position.z += result.normal.z * result.depth * ratioB;

		physicsResolveCollisionVelocity(bodyA, bodyB, result);

		// If surface is within ~45 degrees, the player should be considered grounded and not slide
		if (result.normal.y > 0.7f) bodyA->isGrounded = true;
	} else if (aMoveable) {
		physicsResolveStatic(bodyA, result);
	} else if (bMoveable) {
		CollisionResult flipped = result;
		flipped.normal.x = -result.normal.x;
		flipped.normal.y = -result.normal.y;
		flipped.normal.z = -result.normal.z;
		physicsResolveStatic(bodyB, flipped);
	}
}

// Loop through collision pairs to update their values with collision results
static void physicsResolveCollisions(void) {
	for (int count = 0; count < collisionPairCount; count++) {
		CollisionPair *pair = &collisionPairs[count];
		physicsResolveCollision(pair->bodyA, pair->bodyB, pair->result);
	}
}

// Applies physics to all registered physics bodies
void physicsUpdate(float deltaTime) {

	// Reset isGrounded for all entities, otherwise they will never fall
	for (int count = 0; count < bodyCount; count++) {
		bodies[count]->isGrounded = false;
	}

	for (int counter = 0; counter < bodyCount; counter++) {
		PhysicsBody *body = bodies[counter];
		if (!body->isActive) continue;
		if (body->bodyType == BODY_STATIC) continue;
		if (body->bodyType == BODY_KINEMATIC) continue;

		physicsIntegrate(body, deltaTime);
		//physicsApplyFriction;    // to be added. Add if statement to check if enabled for entity?
	}

	// Temp heightmap collision, I have not filled out the heightmap yet
	for (int counter = 0; counter < bodyCount; counter++) {
		PhysicsBody *body = bodies[counter];
		if (!body->isActive) continue;
		if (body->bodyType == BODY_STATIC) continue;

		CollisionResult result = collisionHeightmapCheck(body);
		if (result.hit) physicsResolveStatic(body, result);
	}

	if (heightmapData != NULL) {
		for (int counter = 0; counter < bodyCount; counter++) {
			PhysicsBody *body = bodies[counter];
			if (!body->isActive) continue;
			if (body->bodyType == BODY_STATIC) continue;

			CollisionResult result = collisionHeightmapCheck(body);
			if (result.hit) physicsResolveStatic(body, result);
		}
	}

	physicsDetectCollisions();
	physicsResolveCollisions();
}
