#include "raylib.h"

Camera3D playerCameraInit(Vector3 position, Vector3 target, Vector3 up) {
	Camera3D playerCamera3D;

	playerCamera3D.position   = position;
	playerCamera3D.target     = target;
	playerCamera3D.up         = up;
	playerCamera3D.fovy       = 45.0f;
	playerCamera3D.projection = CAMERA_PERSPECTIVE;
	DisableCursor();

	return playerCamera3D;
}

void moveCamera3D(Camera3D *camera, Vector3 cameraMovement, Vector3 cameraRotate) {
	UpdateCameraPro(camera, cameraMovement, cameraRotate, 0);
}

void enterFreeCam(void) {
	int x = 0;
}
