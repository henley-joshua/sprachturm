#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include "utils/macros.h"
#include "engine/entity/entity.h"
#include "engine/physics/physics_types.h"
#include "engine/physics/physics.h"

#define STATIC_CUBE_COUNT 100
#define DYNAMIC_CUBE_COUNT 10

typedef struct {
	Entity base;
} StaticCube;

typedef struct {
	Entity base;
	float speed;
	float maxDist;
	Vector3 direction;
	Vector3 origin;
} DynamicCube;

StaticCube staticCubes[STATIC_CUBE_COUNT];
DynamicCube dynamicCubes[DYNAMIC_CUBE_COUNT];
Matrix *staticCubeTransforms = NULL;
Matrix *dynamicCubeTransforms = NULL;

void staticCubesInit(void) {
	staticCubeTransforms = malloc(STATIC_CUBE_COUNT * sizeof(Matrix));

	for (int count = 0; count < STATIC_CUBE_COUNT; count++) {
		Vector3 position = {
			(float)(count % 10) * 10.0f,
			1.0f,
			(float)(count / 10) * 10.0f
		};

		staticCubes[count].base.body = (PhysicsBody){
			.position = position,
			.bodyType = BODY_STATIC,
			.colliderType = COLLIDER_AABB,
			.colliderExtents = {2.0f, 2.0f, 2.0f},
			.isActive = true
		};

		physicsRegister(&staticCubes[count].base.body);
		staticCubeTransforms[count] = MatrixTranslate(position.x, position.y, position.z);
	}
}

void dynamicCubesInit(void) {
	dynamicCubeTransforms = malloc(DYNAMIC_CUBE_COUNT * sizeof(Matrix));

	for (int count = 0; count < DYNAMIC_CUBE_COUNT; count++) {
		Vector3 position = {
			(float)(count / 10) * -20.0f,
			2.0f,
			(float)(count % 10) * -20.0f
		};

		VelocityLimits velLims = {
			.maxHorizontalVelocity = 1.0f,
			.maxVelocityUp = 1.0f,
			.maxVelocityDown = 1.0f
		};
		dynamicCubes[count].base.body = (PhysicsBody){
			.position = position,
			.bodyType = BODY_KINEMATIC,
			.colliderType = COLLIDER_AABB,
			.colliderExtents = {1.0f, 1.0f, 1.0f},
			.isActive = true,
			.velocity = {0.2f, 0.0f, 0.0f},
			.acceleration = {0.0f, 0.0f, 0.0f},
			.velocityLimits = velLims
		};

		dynamicCubes[count].speed = 0.2f;
		dynamicCubes[count].maxDist = 20;
		dynamicCubes[count].origin = position;
		dynamicCubes[count].direction = (Vector3){ 1.0f, 0.0f, 0.0f };

		physicsRegister(&dynamicCubes[count].base.body);
		dynamicCubeTransforms[count] = MatrixTranslate(position.x, position.y, position.z);
	}
}

static void staticCubesDraw(void) {

	for (int i = 0; i < STATIC_CUBE_COUNT; i++) {
		DrawCube(staticCubes[i].base.body.position, 2.0f, 2.0f, 2.0f, RED);
	}
}

static void dynamicCubesDraw(void) {

	for (int i = 0; i < DYNAMIC_CUBE_COUNT; i++) {
		DrawCube(dynamicCubes[i].base.body.position, 1.0f, 1.0f, 1.0f, BLUE);
	}
}

static void dynamicCubesUpdate(void) {
	
	Vector3 velocity;
	for (int count = 0; count < DYNAMIC_CUBE_COUNT; count++) {
		float minDisplacementX = dynamicCubes[count].origin.x - dynamicCubes[count].maxDist;
		float maxDisplacementX = dynamicCubes[count].origin.x + dynamicCubes[count].maxDist;
		float minDisplacementY = dynamicCubes[count].origin.y - dynamicCubes[count].maxDist;
		float maxDisplacementY = dynamicCubes[count].origin.y + dynamicCubes[count].maxDist;
		float minDisplacementZ = dynamicCubes[count].origin.z - dynamicCubes[count].maxDist;
		float maxDisplacementZ = dynamicCubes[count].origin.z + dynamicCubes[count].maxDist;

		float posX = dynamicCubes[count].base.body.position.x;
		float posY = dynamicCubes[count].base.body.position.y;
		float posZ = dynamicCubes[count].base.body.position.z;

		if (posX >= maxDisplacementX || posX <= minDisplacementX) {
			dynamicCubes[count].direction.x = -(dynamicCubes[count].direction.x);
		}
		if (posY >= maxDisplacementY || posY <= minDisplacementY) {
			dynamicCubes[count].direction.y = -(dynamicCubes[count].direction.y);
		}
		if (posZ >= maxDisplacementX || posZ <= minDisplacementZ) {
			dynamicCubes[count].direction.z = -(dynamicCubes[count].direction.z);
		}

		velocity.x = dynamicCubes[count].speed * dynamicCubes[count].direction.x;
		velocity.y = dynamicCubes[count].speed * dynamicCubes[count].direction.y;
		velocity.z = dynamicCubes[count].speed * dynamicCubes[count].direction.z;
		physicsSetVelocity(&dynamicCubes[count].base.body, velocity);

		dynamicCubes[count].base.body.position.x += dynamicCubes[count].base.body.velocity.x;
		dynamicCubes[count].base.body.position.y += dynamicCubes[count].base.body.velocity.y;
		dynamicCubes[count].base.body.position.z += dynamicCubes[count].base.body.velocity.z;
	}
}

void updateEnvironment(void) {
	dynamicCubesUpdate();
}

void drawEnvironment(void) {
	DrawGrid(100, 10.0f);
	DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f}, (Vector2){1000.0f, 1000.0f}, GRAY);
	staticCubesDraw();
	dynamicCubesDraw();
}

void staticCubesUnload(void) {
	free(staticCubeTransforms);
	staticCubeTransforms = NULL;
}

void dynamicCubesUnload(void) {
	free(dynamicCubeTransforms);
	dynamicCubeTransforms = NULL;
}
