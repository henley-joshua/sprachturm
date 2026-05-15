#include "raylib.h"

int startGameLoop(void) {
	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(VIOLET);
		DrawText("This is a window", 190, 200, 20, LIGHTGRAY);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
