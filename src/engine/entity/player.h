#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "engine/entity/entity.h"

typedef struct {
	Camera3D camera;
	Entity base;
} Player;

Vector3 getCameraMovement(void);
void playerInit(Player *player);
void movePlayerCamera(Player *player, Vector3 distance);
void movePlayerBody(Player *player, Vector3 distance);
void updatePlayerMovement(Player *player);
void rotatePlayerBody(Player *player, Vector3 distance);
void drawPlayer(Player *player);

#endif
