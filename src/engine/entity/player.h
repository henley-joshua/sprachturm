#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef struct {
	Camera3D camera;
	Vector3 position;
	Vector3 size;
	Color color;
} Player;

Vector3 getCameraMovement(void);
void playerInit(Player *player);
void movePlayerCamera(Player *player, Vector3 distance);
void movePlayerBody(Player *player, Vector3 distance);
void updatePlayerMovement(Player *player);
void rotatePlayerBody(Player *player, Vector3 distance);
void drawPlayer(Player *player);

#endif
