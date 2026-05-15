#include "raylib.h"

int createWindow(void) {
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Basic Window");

	SetTargetFPS(60);
	return 0;
}
