#include <stddef.h>
#include <math.h>
#include "raylib.h"
#include "engine/physics/physics_types.h"

// Defines a function pointer type to use for mapping functions to collision types
typedef CollisionResult (*CollisionFunc)(PhysicsBody *bodyA, PhysicsBody *bodyB);
static CollisionFunc collisionDispatch[COLLIDER_TYPE_COUNT][COLLIDER_TYPE_COUNT];


float heightmapGetHeight(Vector3 position) {
	return 0.0f;
}

Vector3 heightmapGetNormal(Vector3 position) {
	return (Vector3) { 0.0f, 1.0f, 0.0f };
}

// Initializes 2D function pointer array to map body collision types to correct specific functions
void collisionInit(void) {
	//collisionDispatch[COLLIDER_AABB][COLLIDER_AABB] = collisionAabbAabb;
	//collisionDispatch[COLLIDER_SPHERE][COLLIDER_SPHERE] = collisionSphereSphere;
	//collisionDispatch[COLLIDER_AABB][COLLIDER_SPHERE] = collisionAabbSphere;
	//collisionDispatch[COLLIDER_SPHERE][COLLIDER_AABB] = collisionSphereAabb;
	//collisionDispatch[COLLIDER_CAPSULE][COLLIDER_AABB] = collisionCapsuleAabb;
	//collisionDispatch[COLLIDER_AABB][COLLIDER_CAPSULE] = collisionAabbCapsule;
	//collisionDispatch[COLLIDER_AABB][COLLIDER_AABB] = collisionAabbAabb;
	// TODO: Add remaining mappings
}

CollisionResult collisionHeightmapCheck(PhysicsBody *body) {
	float bodyBottom;

	switch (body->colliderType) {
		case COLLIDER_AABB:
			bodyBottom = body->position.y - body->colliderExtents.y;
			break;
		case COLLIDER_SPHERE:
		case COLLIDER_CAPSULE:
			bodyBottom = body->position.y - body->colliderRadius;
			break;
		default:
			bodyBottom = body->position.y;
			break;
	}

	float groundY = heightmapGetHeight(body->position);
	CollisionResult result = { .hit = false };

	if (bodyBottom <= groundY) {
		result.hit = true;
		result.normal = heightmapGetNormal(body->position);
		result.point = (Vector3){ body->position.x, groundY, body->position.z };
		result.depth = groundY - bodyBottom;
	}
	return result;
}

// Performs a quick check to see if two bodies collide during the frame
bool collisionBroadPhase(PhysicsBody *bodyA, PhysicsBody *bodyB) {
	float dx = bodyB->position.x - bodyA->position.x;
	float dy = bodyB->position.y - bodyA->position.y;
	float dz = bodyB->position.z - bodyA->position.z;
	float distanceSquared = dx*dx + dy*dy + dz*dz;

	float radiusA, radiusB;

	// Only AABB collider types should have colliderExtents, spheres/capsules etc. have radius set
	if (bodyA->colliderType == COLLIDER_AABB) {
		radiusA = sqrtf(bodyA->colliderExtents.x * bodyA->colliderExtents.x +
			 	bodyA->colliderExtents.y * bodyA->colliderExtents.y +
			 	bodyA->colliderExtents.z * bodyA->colliderExtents.z);
	} else {
		radiusA = bodyA->colliderRadius;
	}

	if (bodyB->colliderType == COLLIDER_AABB) {
		radiusB = sqrtf(bodyB->colliderExtents.x * bodyB->colliderExtents.x +
			 	bodyB->colliderExtents.y * bodyB->colliderExtents.y +
			 	bodyB->colliderExtents.z * bodyB->colliderExtents.z);
	} else {
		radiusB = bodyB->colliderRadius;
	}

	float minDistance = radiusA + radiusB;
	// Minor performance consideration: do not perform an extra sqrtf by comparing the squares directly
	return distanceSquared < minDistance * minDistance;
}

// Should only be run against bodies that have been confirmed are colliding in
// collisionBroadPhase. Gets the result of the collision between the two bodies.
CollisionResult collisionNarrowPhase(PhysicsBody *bodyA, PhysicsBody *bodyB) {
	// Get collision function based on both body collision types
	CollisionFunc function = collisionDispatch[bodyA->colliderType][bodyB->colliderType];

	if (function == NULL) {
		// for non-supported collision, return no collision (for now)
		return (CollisionResult){ .hit = false };
	}
	return function(bodyA, bodyB);
}
