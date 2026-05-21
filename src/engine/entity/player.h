#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "engine/entity/entity.h"

typedef struct {
	float height;
	Camera3D camera;
	Entity base;
} Player;

Vector3 getFreeCamMovement(void);
void playerInit(Player *player);
void movePlayer(Player *player, float deltaTime);
void rotatePlayerBody(Player *player, Vector3 distance);
void drawPlayer(Player *player);

#endif
