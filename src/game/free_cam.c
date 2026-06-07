#include <stdio.h>
#include "raylib.h"
#include "game/player.h"
#include "utils/movement.h"
#include "game/input/game_actions.h"
#include "engine/input/input.h"

static Vector3 getFreeCamMovement(void) {
	Vector3 cameraMovement = { 0 };
	float speed = 0.5f;
	if (inputHeld(ACTION_SPRINT)) speed *= 2.0f;

	if (inputHeld(ACTION_MOVE_FORWARD)) cameraMovement.x += speed;
	if (inputHeld(ACTION_MOVE_BACKWARD)) cameraMovement.x -= speed;
	if (inputHeld(ACTION_MOVE_LEFT)) cameraMovement.y -= speed;
	if (inputHeld(ACTION_MOVE_RIGHT)) cameraMovement.y += speed;
	if (inputHeld(ACTION_JUMP)) cameraMovement.z += speed;
	if (inputHeld(ACTION_CROUCH)) cameraMovement.z -= speed;

	return cameraMovement;
}

static void triggerPlayerMode(Player *player) {
	physicsRegister(&player->base->body);
}

GamemodeState freeCamUpdate(Player *player, float deltaTime) {
	float sensitivity = 0.2;
	if (inputPressed(ACTION_FREE_CAM)) {
		triggerPlayerMode(player);
		return PLAYER_MODE;
	}

	Vector3 camRotation;
	Vector3 freeCamMovement = getFreeCamMovement();
	Vector2 mouseDelta = inputGetMouseDelta();
	camRotation.x = mouseDelta.x * sensitivity;
	camRotation.y = mouseDelta.y * sensitivity;
	camRotation.z = 0;

	UpdateCameraPro(&player->camera, freeCamMovement, camRotation, 0);
	return FREE_CAM_MODE;
}
