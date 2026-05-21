#include "raylib.h"
#include "utils/movement.h"
#include "engine/rendering/camera.h"
#include "engine/entity/player.h"
#include "engine/physics/collision.h"
#include "engine/physics/physics.h"

Vector3 PLAYER_MASS = 70.0f;
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

static void playerSyncCamera(Player *player) {
	Vector3 syncPosition;

	float adjustedHeight = player->base.body.position.y + player->height;
	syncPosition.x = player->base.body.position.x;
	syncPosition.z = player->base.body.position.z;
	syncPosition.y = adjustedHeight;
	setCamera3DPosition(Camera3D *camera, Vector3 position);
}

void playerInit(Player *player) {
	Camera3D playerCamera;

	VelocityLimits playerVelocityLimits = {
		.maxHorizontalVelocity = 1.5f,
		.maxVelocityUp = 2.0f,
		.maxVelocityDown = 1.0f
	};

	PhysicsBody playerPhysics = {
		.position = PLAYER_INIT_POSITION,
		.velocity = (Vector3){ 0 },
		.acceleration = (Vector3){ 0 },
		.bodyType = BODY_DYNAMIC,
		.mass = PLAYER_MASS,
		.bounciness = 0.0f,
		.gravityScale = 1.0f,
		.colliderType = COLLIDER_CAPSULE,
		.colliderRadius = 0.5f,
		.isActive = true

	};

	playerCamera = playerCameraInit(PLAYER_INIT_POSITION, PLAYER_INIT_TARGET, PLAYER_INIT_UP);
	player->camera = playerCamera;
	player->base.body.position = PLAYER_INIT_POSITION;
	player->base.model.size = PLAYER_INIT_SIZE;
	player->base.model.color = PLAYER_INIT_COLOR;

	playerSyncCamera(player);
}

void movePlayer(Player *player, float deltaTime) {
	Movement playerInput = getPlayerInput();
	Vector3 playerMovement = getPlayerMovementVector(playerInput, player);
	player->base.body.velocity.x += playerMovement.x;
	player->base.body.velocity.y += playerMovement.y;
	player->base.body.velocity.z += playerMovement.z;
	physicsIntegrate(&player->base.body, deltaTime);
	playerSyncCamera(player);
}

void rotatePlayerBody(Player *player, Vector3 distance) {
	int x = 0;
}

void drawPlayer(Player *player) {
	DrawCubeV(player->base.body.position, player->base.model.size, player->base.model.color);
}
