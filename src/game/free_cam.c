#include <stdio.h>
#include "raylib.h"
#include "game/player.h"
#include "utils/movement.h"

// TODO: use input keymappings when added to engine
static Vector3 getFreeCamMovement(void) {
	Vector3 cameraMovement;
	Movement movementsMap;

	movementsMap.left = IsKeyDown(KEY_A);
	movementsMap.right = IsKeyDown(KEY_D);
	movementsMap.forward = IsKeyDown(KEY_W);
	movementsMap.backward = IsKeyDown(KEY_S);
	movementsMap.up = IsKeyDown(KEY_SPACE);
	movementsMap.down = IsKeyDown(KEY_LEFT_CONTROL);

	cameraMovement = getMovementVector(movementsMap);
	return cameraMovement;
}

static void triggerPlayerMode(Player *player) {
	physicsRegister(&player->base.body);
}

GamemodeState freeCamUpdate(Player *player, float deltaTime) {
	float sensitivity = 0.2;
	if (IsKeyPressed(KEY_F1)) {
		triggerPlayerMode(player);
		return PLAYER_MODE;
	}

	Vector3 camRotation;
	Vector3 freeCamMovement = getFreeCamMovement();
	Vector2 mouseDelta = GetMouseDelta();
	camRotation.x = mouseDelta.x * sensitivity;
	camRotation.y = mouseDelta.y * sensitivity;
	camRotation.z = 0;

	UpdateCameraPro(&player->camera, freeCamMovement, camRotation, 0);
	return FREE_CAM_MODE;
}
