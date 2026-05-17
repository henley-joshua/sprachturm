#include "raylib.h"
#include "utils/movement.h"
#include <stdio.h>


Camera3D CAMERA_3D = {
	.position = { 0.0f, 10.0f, 10.0f },
	.target = { 0.0f, 0.0f, 0.0f },
	.up = { 0.0f, 1.0f, 0.0f },
	.fovy = 45.0f,
	.projection = CAMERA_PERSPECTIVE
};

Vector3 getCameraMovement(void) {
	Vector3 cameraMovement;
	Movement movements;

	movements.left = IsKeyDown(KEY_A);
	movements.right = IsKeyDown(KEY_D);
	movements.forward = IsKeyDown(KEY_W);
	movements.backward = IsKeyDown(KEY_S);
	movements.up = IsKeyDown(KEY_SPACE);
	movements.down = IsKeyDown(KEY_C);

	cameraMovement = getMovementVector(movements);
	return cameraMovement;
}

void enterFreeCam(void) {
	Vector3 cameraRotate;
	Vector3 cameraMovement;
	Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

	BeginMode3D(CAMERA_3D);

		DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);

		cameraMovement = getCameraMovement();
		Vector2 mouseDelta = GetMouseDelta();
		cameraRotate.x = mouseDelta.x / 1.5;
		cameraRotate.y = mouseDelta.y / 1.5;
		cameraRotate.z = 0;
		UpdateCameraPro(&CAMERA_3D, cameraMovement, cameraRotate, 0);

	EndMode3D();
}
