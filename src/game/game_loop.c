#include "raylib.h"
#include "engine/physics/collision.h"
#include "engine/physics/physics.h"
#include "engine/rendering/renderer.h"
#include "engine/rendering/camera.h"
#include "engine/entity/player.h"
#include "engine/scene/scene.h"


PhysicsConfig physicsConfig = {
	.dimensions = PHYSICS_3D,
	.gravity = { 0.0f, -0.1f, 0.0f },
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
	playerInit(&player);

	while (!WindowShouldClose()) {
		timer += GetFrameTime();
		checkToggleFullscreen();

		// Begin main game state
		updatePlayerMovement(&player);
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

	CloseWindow();
	return 0;
}
