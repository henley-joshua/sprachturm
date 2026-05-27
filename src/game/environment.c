#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include "utils/macros.h"
#include "engine/entity/entity.h"
#include "engine/physics/physics_types.h"
#include "engine/physics/physics.h"

#define CUBE_COUNT 100

typedef struct {
	Entity base;
} StaticCube;

StaticCube cubes[CUBE_COUNT];
Matrix *cubeTransforms = NULL;
Mesh cubeMesh;
Material cubeMaterial;

void staticCubesInit(void) {
	cubeTransforms = malloc(CUBE_COUNT * sizeof(Matrix));

	cubeMesh = GenMeshCube(2.0f, 2.0f, 2.0f);
	UploadMesh(&cubeMesh, false);
	cubeMaterial = LoadMaterialDefault();
	cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = RED;

	for (int count = 0; count < CUBE_COUNT; count++) {
		Vector3 position = {
			(float)(count % 10) * 10.0f,
			1.0f,
			(float)(count / 10) * 10.0f
		};

		cubes[count].base.body = (PhysicsBody){
			.position = position,
			.bodyType = BODY_STATIC,
			.colliderType = COLLIDER_AABB,
			.colliderExtents = {2.0f, 2.0f, 2.0f},
			.isActive = true
		};

		physicsRegister(&cubes[count].base.body);
		cubeTransforms[count] = MatrixTranslate(position.x, position.y, position.z);
	}
}

static void staticCubesDraw(void) {

	for (int i = 0; i < CUBE_COUNT; i++) {
		DrawCube(cubes[i].base.body.position, 2.0f, 2.0f, 2.0f, RED);
	}

	//DrawMeshInstanced(cubeMesh, cubeMaterial, cubeTransforms, CUBE_COUNT);
}

void drawEnvironment(void) {
	DrawGrid(100, 10.0f);
	staticCubesDraw();
}

void staticCubesUnload(void) {
	UnloadMesh(cubeMesh);
	UnloadMaterial(cubeMaterial);
	free(cubeTransforms);
	cubeTransforms = NULL;
}
