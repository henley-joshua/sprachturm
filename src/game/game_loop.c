#include "raylib.h"
#include "engine/physics/collision.h"
#include "engine/physics/physics.h"
#include "engine/rendering/renderer.h"
#include "engine/rendering/camera.h"
#include "game/player.h"
#include "game/heightmap.h"
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

int startGameLoop(void) {

	Player player;

	float timer = 0.0f;
	collisionInit();
	playerInit(&player);
	physicsInit(physicsConfig);
	staticCubesInit();

	double accumulator = 0.0;
	double previousTime = GetTime();

	while (!WindowShouldClose()) {
		double now = GetTime();
		double deltaTime = now - previousTime;
		previousTime = now;
		if (deltaTime > 0.25) deltaTime = 0.25;
		accumulator += deltaTime;

		checkToggleFullscreen();

		// Begin main game state
		//inputUpdate();   // To be added
		playerUpdate(&player, deltaTime);
		physicsUpdate(deltaTime);
		playerSyncCamera(&player);
		// sceneUpdate();    // To be added

		BeginDrawing();
			ClearBackground(BLACK);
			DrawFPS(10, 10);

			BeginMode3D(player.camera);
				drawEnvironment();
				drawPlayer(&player);
			EndMode3D();
		EndDrawing();
		// End main game state




	}

	staticCubesUnload();
	CloseWindow();

	return 0;
}
