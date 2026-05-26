#ifndef PHYSICS_TYPES_H
#define PHYSICS_TYPES_H

typedef enum {
	PHYSICS_2D,
	PHYSICS_3D
} PhysicsDimensions;

typedef enum {
	BODY_DYNAMIC,     // full physics - affected by forces and gravity
	BODY_KINEMATIC,   // moved by code only, not forces
	BODY_STATIC       // never moves
} BodyType;

typedef enum {
	COLLIDER_NONE,
	COLLIDER_AABB,
	COLLIDER_SPHERE,
	COLLIDER_CAPSULE,
	COLLIDER_RAY,
	COLLIDER_TYPE_COUNT
} ColliderType;

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
	float drag;
	ColliderType colliderType;
	float colliderRadius;
	Vector3 colliderExtents;
	bool isGrounded;
	bool isActive;
	bool overrideDrag;
	bool overrideFriction;
	VelocityLimits velocityLimits;
} PhysicsBody;

typedef struct {
	bool hit;
	Vector3 normal;
	Vector3 point;
	float depth;
} CollisionResult;

typedef struct {
	PhysicsBody *bodyA;
	PhysicsBody *bodyB;
	CollisionResult result;
} CollisionPair;

#endif
