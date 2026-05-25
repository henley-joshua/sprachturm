#include "raylib.h"
#include "utils/movement.h"
#include "engine/rendering/camera.h"
#include "game/player.h"
#include "engine/physics/collision.h"
#include "engine/physics/physics.h"

Vector3 PLAYER_INIT_POSITION = { 0.0f, 20.0f, 20.0f };
Vector3 PLAYER_INIT_TARGET = { 0.0f, 0.0f, 0.0f };
Vector3 PLAYER_INIT_UP = { 0.0f, 1.0f, 0.0f };
Vector3 PLAYER_INIT_SIZE = { 2.0f, 3.0f, 1.5f };
Color PLAYER_INIT_COLOR = LIGHTGRAY;


Vector3 getFreeCamMovement(void) {
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

Vector3 getPlayerMovement(Player *player) {
	Vector3 playerMovement;
	Movement movementsMap;

	movementsMap.left = IsKeyDown(KEY_A);
	movementsMap.right = IsKeyDown(KEY_D);
	movementsMap.forward = IsKeyDown(KEY_W);
	movementsMap.backward = IsKeyDown(KEY_S);
	movementsMap.up = IsKeyPressed(KEY_SPACE);
	movementsMap.down = IsKeyDown(KEY_LEFT_CONTROL);

	playerMovement = getPlayerMovementVector(movementsMap, player);
	return playerMovement;
}

void playerInit(Player *player) {
	Camera3D playerCamera;

	playerCamera = playerCameraInit(PLAYER_INIT_POSITION, PLAYER_INIT_TARGET, PLAYER_INIT_UP);
	player->camera = playerCamera;
	player->base.acceleration = (Vector3) { 0.0f, 0.0f, 0.0f };
	player->base.velocity = (Vector3) { 0.0f, 0.0f, 0.0f };
	player->base.position = PLAYER_INIT_POSITION;
	player->base.model.size = PLAYER_INIT_SIZE;
	player->base.model.color = PLAYER_INIT_COLOR;
}

void movePlayer(Player *player, Vector3 distance) {
	float sensitivity = 0.3f;
	Vector2 mouseDelta = GetMouseDelta();

	static Vector2 lastMousePos = {0};
	Vector2 currMousePos = GetMousePosition();
	Vector2 manMouseDelta = {
		currMousePos.x - lastMousePos.x,
		currMousePos.y - lastMousePos.y
	};
	lastMousePos = currMousePos;

	Vector3 cameraRotate = {
		mouseDelta.x * sensitivity,
		mouseDelta.y * sensitivity,
		0
	};

	moveCamera3D(&player->camera, distance, cameraRotate);
	// Also move player "body"
	player->base.position = player->camera.position;
}

void updatePlayerAcceleration(Player *player) {
	applyEntityGravity(&player->base);
	Vector3 playerInputAcceleration = getPlayerMovement(player);
	player->base.acceleration.z += playerInputAcceleration.z;
}

void updatePlayerVelocity(Player *player) {
	Vector3 playerInputVelocity = getPlayerMovement(player);
	player->base.velocity.x = playerInputVelocity.x;
	player->base.velocity.y = playerInputVelocity.y;
	player->base.velocity.z += player->base.acceleration.z;
}

void updatePlayerMovement(Player *player) {
	player->base.canJump = checkIfPlayerCanJump(player);
	updatePlayerAcceleration(player);
	updatePlayerVelocity(player);
	Vector3 displacement = player->base.velocity;

	float groundCollisionDistance = enforcePlayerGroundCollision(&player->base);
	displacement.z += groundCollisionDistance;
	movePlayer(player, displacement);
}

void rotatePlayerBody(Player *player, Vector3 distance) {
	int x = 0;
}

// Changes the y position of a given Vector3 by half of the player size
Vector3 getAdjustedPlayerPosition(Player *player, Vector3 position) {
	float playerHeight = player->base.model.size.y / 2.0f;
	position.y -= playerHeight;
	return position;
}

void drawPlayer(Player *player) {
	Vector3 adjustedPosition = getAdjustedPlayerPosition(player, player->base.position);
	DrawCubeV(adjustedPosition, player->base.model.size, player->base.model.color);
}
