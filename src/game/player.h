#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "engine/entity/entity.h"

typedef struct {
	Camera3D camera;
	Entity base;
	float height;
	float speed;
	float sensitivity;
	float yaw;
	float pitch;
} Player;

typedef enum {
	FREE_CAM_MODE,
	PLAYER_MODE
} GamemodeState;

void playerInit(Player *player);
GamemodeState playerUpdate(Player *player, float deltaTime);
void rotatePlayerBody(Player *player, Vector3 distance);
void drawPlayer(Player *player);
void playerSyncCamera(Player *player);


#endif
