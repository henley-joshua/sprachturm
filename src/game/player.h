#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "engine/entity/entity.h"

typedef struct {
	Camera3D camera;
	Entity base;
} Player;

Vector3 getFreeCamMovement(void);
Vector3 getPlayerMovement(Player *player);
void playerInit(Player *player);
void movePlayer(Player *player, Vector3 distance);
void updatePlayerAcceleration(Player *player);
void updatePlayerVelocity(Player *player);
void updatePlayerMovement(Player *player);
void rotatePlayerBody(Player *player, Vector3 distance);
void drawPlayer(Player *player);

#endif
