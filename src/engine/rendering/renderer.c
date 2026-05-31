#include "raylib.h"

int createWindow(void) {
	const int screenWidth = 800;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "Basic Window");

	SetTargetFPS(60);
	return 0;
}

void checkToggleFullscreen(void) {
	bool toggleFullscreen = IsKeyPressed(KEY_F11);
	if (toggleFullscreen) ToggleFullscreen();
}
