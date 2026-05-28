#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"

void enterFreeCam(void);
void moveCamera3D(Camera3D *camera, Vector3 cameraMovement, Vector3 cameraRotate);
void setCamera3DPosition(Camera3D *camera, Vector3 position);
Camera3D playerCameraInit(Vector3 position, Vector3 target, Vector3 up);

#endif
