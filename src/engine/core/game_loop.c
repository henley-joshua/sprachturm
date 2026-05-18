#include "raylib.h"
#include "engine/physics/collision.h"
#include "engine/rendering/renderer.h"
#include "engine/rendering/camera.h"
#include "engine/entity/player.h"
#include "engine/scene/scene.h"
#include <stdio.h>

int startGameLoop(void) {

	Player player;

	float timer = 0.0f;
	playerInit(&player);

	while (!WindowShouldClose()) {
		timer += GetFrameTime();
		checkToggleFullscreen();

		// Begin main game state
		BeginDrawing();
			ClearBackground(BLACK);
			DrawFPS(10, 10);

			TraceLog(LOG_INFO, "RENDER target: %f %f %f", player.camera.target.x, player.camera.target.y, player.camera.target.z);
			BeginMode3D(player.camera);
				updatePlayerMovement(&player);
				drawEnvironment();
				drawPlayer(&player);
			EndMode3D();
		EndDrawing();
		// End main game state




	}

	CloseWindow();
	return 0;
}
