#include "raylib.h"
#include "utils/movement.h"
#include "utils/macros.h"
#include <stdio.h>


Camera3D CAMERA_3D;

void cameraInit(void) {
	CAMERA_3D.position   = (Vector3){ 0.0f, 10.0f, 10.0f };
	CAMERA_3D.target     = (Vector3){ 0.0f, 0.0f, 0.0f };
	CAMERA_3D.up         = (Vector3){ 0.0f, 1.0f, 0.0f };
	CAMERA_3D.fovy       = 45.0f;
	CAMERA_3D.projection = CAMERA_PERSPECTIVE;
	//DisableCursor();
}

Vector3 getCameraMovement(void) {
	Vector3 cameraMovement;
	Movement movements;

	movements.left = IsKeyDown(KEY_A);
	movements.right = IsKeyDown(KEY_D);
	movements.forward = IsKeyDown(KEY_W);
	movements.backward = IsKeyDown(KEY_S);
	movements.up = IsKeyDown(KEY_SPACE);
	movements.down = IsKeyDown(KEY_LEFT_CONTROL);

	cameraMovement = getMovementVector(movements);
	return cameraMovement;
}

void enterFreeCam(void) {
	Vector3 cameraRotate;
	Vector3 cameraMovement;
	Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
	Vector3 cubePosition2 = { 10.0f, 0.0f, 10.0f };
	Vector3 cubePosition3 = { 20.0f, 0.0f, 20.0f };
	Vector3 cubePosition4 = { 20.0f, 0.0f, 0.0f };
	Vector3 cubePosition5 = { 10.0f, 0.0f, 0.0f };
	Vector3 cubePosition6 = { 0.0f, 0.0f, 10.0f };
	Vector3 cubePosition7 = { 0.0f, 0.0f, 20.0f };
	Vector3 cubeList[7] = {cubePosition, cubePosition2,
			       cubePosition3, cubePosition4,
			       cubePosition5, cubePosition6,
			       cubePosition7};


	BeginMode3D(CAMERA_3D);
		DrawGrid(100, 10.0f);
		for (int i = 0; i < ARRAY_LEN(cubeList); i++) {
			DrawCube(cubeList[i], 2.0f, 2.0f, 2.0f, RED);
			DrawCubeWires(cubeList[i], 2.0f, 2.0f, 2.0f, LIGHTGRAY);
		}

		Vector2 mouseDelta = GetMouseDelta();
		cameraMovement = getCameraMovement();
		cameraRotate.x = mouseDelta.x / 5;
		cameraRotate.y = mouseDelta.y / 5;
		cameraRotate.z = 0;
		UpdateCameraPro(&CAMERA_3D, cameraMovement, cameraRotate, 0);
	EndMode3D();
}
