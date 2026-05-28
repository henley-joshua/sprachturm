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

static CollisionResult collisionAabbAabb(PhysicsBody *bodyA, PhysicsBody *bodyB) {
	CollisionResult result;
	result.hit = false;

	float aMaxX = bodyA->position.x + bodyA->colliderExtents.x;
	float aMaxY = bodyA->position.y + bodyA->colliderExtents.y;
	float aMaxZ = bodyA->position.z + bodyA->colliderExtents.z;
	float aMinX = bodyA->position.x - bodyA->colliderExtents.x;
	float aMinY = bodyA->position.y - bodyA->colliderExtents.y;
	float aMinZ = bodyA->position.z - bodyA->colliderExtents.z;

	float bMaxX = bodyB->position.x + bodyB->colliderExtents.x;
	float bMaxY = bodyB->position.y + bodyB->colliderExtents.y;
	float bMaxZ = bodyB->position.z + bodyB->colliderExtents.z;
	float bMinX = bodyB->position.x - bodyB->colliderExtents.x;
	float bMinY = bodyB->position.y - bodyB->colliderExtents.y;
	float bMinZ = bodyB->position.z - bodyB->colliderExtents.z;

	if (aMaxX <= bMinX || aMinX >= bMaxX) return result;
	if (aMaxY <= bMinY || aMinY >= bMaxY) return result;
	if (aMaxZ <= bMinZ || aMinZ >= bMaxZ) return result;

	result.hit = true;
	float overlapX = (aMaxX < bMaxX ? aMaxX : bMaxX) - (aMinX > bMinX ? aMinX : bMinX);
	float overlapY = (aMaxY < bMaxY ? aMaxY : bMaxY) - (aMinY > bMinY ? aMinY : bMinY);
	float overlapZ = (aMaxZ < bMaxZ ? aMaxZ : bMaxZ) - (aMinZ > bMinZ ? aMinZ : bMinZ);
	
	// Use the axis with the smallest collision and apply result along that axis
	if (overlapY <= overlapX && overlapY <= overlapZ) {
		result.depth = overlapY;
		result.normal.x = 0;
		result.normal.z = 0;
		result.normal.y = (bodyA->position.y < bodyB->position.y) ? -1.0f : 1.0f;
	} else if (overlapX <= overlapY && overlapX <= overlapZ) {
		result.depth    = overlapX;
		result.normal.y = 0;
		result.normal.z = 0;
		result.normal.x = (bodyA->position.x < bodyB->position.x) ? -1.0f : 1.0f;
	} else {
		result.depth    = overlapZ;
		result.normal.x = 0;
		result.normal.y = 0;
		result.normal.z = (bodyA->position.z < bodyB->position.z) ? -1.0f : 1.0f;
	}

	result.point = (Vector3){
		(aMinX > bMinX ? aMinX : bMinX) + overlapX * 0.5f,
		(aMinY > bMinY ? aMinY : bMinY) + overlapY * 0.5f,
		(aMinZ > bMinZ ? aMinZ : bMinZ) + overlapZ * 0.5f
	};

	return result;
}

// Initializes 2D function pointer array to map body collision types to correct specific functions
void collisionInit(void) {
	collisionDispatch[COLLIDER_AABB][COLLIDER_AABB] = collisionAabbAabb;
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
