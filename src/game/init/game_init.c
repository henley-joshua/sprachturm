#include "raylib.h"
#include "engine/physics/physics.h"
#include "game/player.h"
#include "game/input/game_input.h"
#include "game/environment.h"


PhysicsConfig physicsConfig = {
	.dimensions = PHYSICS_3D,
	.gravity = { 0.0f, -3.5f, 0.0f },
	.enableGravity = true,
	.airDrag = 0.01,
	.groundFriction = 0.5f,
	.enableFriction = false,
	.substeps = 1,
	.fixedTimestep = 1.0f / 60.0f
};

void gameInit(Player *player) {
	collisionInit();
	playerInit(player);
	physicsInit(physicsConfig);
	staticCubesInit();
	dynamicCubesInit();
	gameInputInit();
}
