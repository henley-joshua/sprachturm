#include "raylib.h"
#include "utils/movement.h"
#include "engine/rendering/camera.h"
#include "engine/entity/player.h"
#include "engine/physics/collision.h"

Vector3 PLAYER_INIT_POSITION = { 0.0f, 10.0f, 10.0f };
Vector3 PLAYER_INIT_TARGET = { 0.0f, 0.0f, 0.0f };
Vector3 PLAYER_INIT_UP = { 0.0f, 1.0f, 0.0f };
Vector3 PLAYER_INIT_SIZE = { 2.0f, 3.0f, 1.5f };
Color PLAYER_INIT_COLOR = LIGHTGRAY;


Vector3 getCameraMovement(void) {
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

void playerInit(Player *player) {
	Camera3D playerCamera;

	playerCamera = playerCameraInit(PLAYER_INIT_POSITION, PLAYER_INIT_TARGET, PLAYER_INIT_UP);
	player->camera = playerCamera;
	player->base.position = PLAYER_INIT_POSITION;
	player->base.size = PLAYER_INIT_SIZE;
	player->base.color = PLAYER_INIT_COLOR;
}

void movePlayerCamera(Player *player, Vector3 distance) {
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
}

void movePlayerBody(Player *player, Vector3 position) {
	player->base.position.x = position.x;
	player->base.position.y = position.y;
	player->base.position.z = position.z;
}

void updatePlayerMovement(Player *player) {
	Vector3 distance;
	distance = getCameraMovement();

	float groundCollisionDistance = enforcePlayerGroundCollision(player->base);
	distance.z += groundCollisionDistance;
	movePlayerCamera(player, distance);
	movePlayerBody(player, player->camera.position);
}

void rotatePlayerBody(Player *player, Vector3 distance) {
	int x = 0;
}

void drawPlayer(Player *player) {
	DrawCubeV(player->base.position, player->base.size, player->base.color);
}
