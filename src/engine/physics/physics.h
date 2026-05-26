#ifndef PHYSICS_H
#define PHYSICS_H

#include "engine/physics/collision.h"
#include "engine/physics/physics_types.h"

void physicsInit(PhysicsConfig config);
void physicsRegister(PhysicsBody *body);
void physicsUnregister(PhysicsBody *body);
void physicsApplyForce(PhysicsBody *body, Vector3 force);
void physicsApplyImpulse(PhysicsBody *body, Vector3 impulse);
void physicsSetVelocity(PhysicsBody *body, Vector3 velocity);
void physicsClampVelocity(PhysicsBody *body);
void physicsIntegrate(PhysicsBody *body, float deltaTime);
void physicsResolveCollision(PhysicsBody *bodyA, PhysicsBody *bodyB, CollisionResult collisionResult);
void physicsUpdate(float deltaTime);

#endif
