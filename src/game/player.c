#include <math.h>
#include "raylib.h"
#include "rcamera.h"
#include "utils/vector.h"
#include "engine/rendering/camera.h"
#include "game/player.h"
#include "engine/physics/collision.h"
#include "engine/physics/physics.h"
#include "engine/input/input.h"
#include "game/input/game_actions.h"

float JUMP_FORCE = 20.0;
float PLAYER_MASS = 20000.0;
float PLAYER_INIT_SENSITIVITY = 0.003;
float PLAYER_INIT_SPEED = 10.0;
float PLAYER_INIT_YAW = 0.0;
float PLAYER_INIT_PITCH = 0.0;
Vector3 PLAYER_INIT_POSITION = { 0.0f, 20.0f, 20.0f };
Vector3 PLAYER_INIT_VELOCITY = { 0.0f, 0.0f, 0.0f };
Vector3 PLAYER_INIT_ACCELERATION = { 0.0f, 0.0f, 0.0f };
Vector3 PLAYER_INIT_TARGET = { 0.0f, 0.0f, 0.0f };
Vector3 PLAYER_INIT_UP = { 0.0f, 1.0f, 0.0f };
Vector3 PLAYER_INIT_SIZE = { 2.0f, 3.0f, 1.5f };
Color PLAYER_INIT_COLOR = LIGHTGRAY;


void playerSyncCamera(Player *player) {
	Vector2 mouseDelta = inputGetMouseDelta();

	player->yaw -= mouseDelta.x * player->sensitivity;
	player->pitch += mouseDelta.y * player->sensitivity;

	if (player->pitch > 1.5f) player->pitch = 1.5f;
	if (player->pitch < -1.5f) player->pitch = -1.5f;

	Vector3 cameraPosition = {
		player->base.body.position.x,
		player->base.body.position.y + player->height,
		player->base.body.position.z
	};
	player->camera.position = cameraPosition;

	player->camera.target = (Vector3){
		cameraPosition.x + cosf(player->pitch) * sinf(player->yaw),
		cameraPosition.y - sinf(player->pitch),
		cameraPosition.z + cosf(player->pitch) * cosf(player->yaw)
	};
}

void playerInit(Player *player) {
	Camera3D playerCamera;

	VelocityLimits playerVelocityLimits = {
		.maxHorizontalVelocity = 50.0f,
		.maxVelocityUp = 20.0f,
		.maxVelocityDown = 100.0f
	};

	PhysicsBody playerPhysics = {
		.position = PLAYER_INIT_POSITION,
		.velocity = PLAYER_INIT_VELOCITY,
		.acceleration = PLAYER_INIT_ACCELERATION,
		.bodyType = BODY_DYNAMIC,
		.mass = PLAYER_MASS,
		.bounciness = 0.0f,
		.gravityScale = 10.0f,
		.colliderType = COLLIDER_AABB,
		.colliderExtents = { 0.5f, 1.0f, 0.5f },
		.isActive = true,
		.velocityLimits = playerVelocityLimits
	};

	playerCamera = playerCameraInit(PLAYER_INIT_POSITION, PLAYER_INIT_TARGET, PLAYER_INIT_UP);
	player->camera = playerCamera;
	player->height = PLAYER_INIT_SIZE.x;
	player->speed = PLAYER_INIT_SPEED;
	player->sensitivity = PLAYER_INIT_SENSITIVITY;
	player->pitch = PLAYER_INIT_PITCH;
	player->yaw = PLAYER_INIT_YAW;
	player->base.body = playerPhysics;
	player->base.model.size = PLAYER_INIT_SIZE;
	player->base.model.color = PLAYER_INIT_COLOR;

	playerSyncCamera(player);
	physicsRegister(&player->base.body);
}

static Vector3 playerGetForward(Player *player) {
	Vector3 right = GetCameraRight(&player->camera);

	return (Vector3){
		right.z,
		0,
		-right.x
	};
}

static Vector3 playerGetRight(Player *player) {
	Vector3 right = GetCameraRight(&player->camera);
	// Cross product of forward and up vector
	right.y = 0;
	float length = sqrtf(right.x * right.x + right.z * right.z);
	if (length > 0) {
		right.x /= length;
		right.z /= length;
	}
	return right;
}

// TODO: generalize movement in the engine
static Vector3 getPlayerMovementVector(Vector3 forward, Vector3 right, float speed) {
	Vector3 movementVector = { 0 };

	float forwardX = forward.x;
	float forwardZ = forward.z;
	float rightX = right.x;
	float rightZ = right.z;

	if (inputHeld(ACTION_MOVE_FORWARD)) {
		movementVector.x += forwardX;
		movementVector.z += forwardZ;
	}
	if (inputHeld(ACTION_MOVE_BACKWARD)) {
		movementVector.x -= forwardX;
		movementVector.z -= forwardZ;
	}
	if (inputHeld(ACTION_MOVE_LEFT)) {
		movementVector.x -= rightX;
		movementVector.z -= rightZ;
	}
	if (inputHeld(ACTION_MOVE_RIGHT)) {
		movementVector.x += rightX;
		movementVector.z += rightZ;
	}

	float moveLength = sqrtf(movementVector.x * movementVector.x + movementVector.z * movementVector.z);
	if (moveLength > 0) {
		movementVector.x /= moveLength;
		movementVector.z /= moveLength;
	}

	if (inputHeld(ACTION_SPRINT)) {
		speed *= 2.0f;
	}
	movementVector.x *= speed;
	movementVector.z *= speed;

	return movementVector;
}

Vector3 getPlayerMovement(Player *player) {
	Vector3 forward = playerGetForward(player);
	Vector3 right = playerGetRight(player);

	return getPlayerMovementVector(forward, right, player->speed);
}

static void triggerFreeCamMode(Player *player) {
	physicsUnregister(&player->base.body);
}

GamemodeState playerUpdate(Player *player, float deltaTime) {
	if (inputPressed(ACTION_FREE_CAM)) {
		triggerFreeCamMode(player);
		return FREE_CAM_MODE;
	}
	Vector3 playerMovement = getPlayerMovement(player);

	player->base.body.velocity.x = playerMovement.x;
	player->base.body.velocity.z = playerMovement.z;
	if (inputPressed(ACTION_JUMP) && player->base.body.isGrounded) physicsApplyImpulse(&player->base.body, (Vector3){ 0, JUMP_FORCE, 0 });
	return PLAYER_MODE;
}

void rotatePlayerBody(Player *player, Vector3 distance) {
	int x = 0;
}

void drawPlayer(Player *player) {
	Vector3 initPosLower = { player->base.body.position.x, player->base.body.position.y - player->height, player->base.body.position.z };
	Vector3 initPosUpper = { player->base.body.position.x, player->base.body.position.y + player->height, player->base.body.position.z };
	DrawCapsule(initPosLower, initPosUpper, player->base.body.colliderExtents.x, 10, 10, player->base.model.color);
	//DrawCubeV(player->base.body.position, player->base.model.size, player->base.model.color);
}

