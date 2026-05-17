#include "raylib.h"
#include "engine/physics/collision.h"
#include "engine/rendering/renderer.h"
#include "engine/rendering/camera.h"
#include "utils/macros.h"
#include <stdio.h>

int startGameLoop(void) {

	float timer = 0.0f;
	cameraInit();

	while (!WindowShouldClose()) {
		timer += GetFrameTime();
		checkToggleFullscreen();

		BeginDrawing();
			ClearBackground(BLACK);
			enterFreeCam();
			DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
