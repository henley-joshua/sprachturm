#ifndef PHYSICS_H
#define PHYSICS_H

#include "engine/physics/collision.h"

typedef enum {
	PHYSICS_2D,
	PHYSICS_3D
} PhysicsDimensions;

typedef enum {
	BODY_DYNAMIC,     // full physics - affected by forces and gravity
	BODY_KINEMATIC,   // moved by code only, not forces
	BODY_STATIC       // never moves
} BodyType;

typedef struct {
	PhysicsDimensions dimensions;
	Vector3 gravity;
	float airDrag;
	float groundFriction;
	bool enableGravity;
	bool enableFriction;
	bool enableBuoyancy;
	bool enableWind;
	int substeps;
	float fixedTimestep;
} PhysicsConfig;

typedef struct {
	float maxHorizontalVelocity;
	float maxVelocityUp;
	float maxVelocityDown;
} VelocityLimits;

typedef struct {
	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	BodyType bodyType;
	float mass;
	float friction;
	float bounciness;
	float gravityScale;
	ColliderType colliderType;
	float colliderRadius;
	Vector3 colliderExtents;
	bool isGrouded;
	bool isActive;
	bool overrideDrag;
	bool overrideFriction;
	VelocityLimits velocityLimits;
} PhysicsBody;

#endif
